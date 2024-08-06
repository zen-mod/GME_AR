//------------------------------------------------------------------------------------------------
[BaseContainerProps(), SCR_BaseContainerCustomTitleUIInfo("m_Info")]
class GME_GarrisonContextAction : SCR_BaseContextAction
{
	[Attribute(desc: "Active labels and white listed labels and label groups")]
	protected ref SCR_EditorContentBrowserDisplayConfig m_ContentBrowserConfig;
	
	protected SCR_DestructibleBuildingEntity m_pBuilding = null;
	protected ref GME_BuildingPosQuery m_BuildingPosQuery;
	protected ref array<vector> m_aPositions;
	protected ref array<vector> m_aDirections;
	protected ref array<vector> m_aCoverPostPositions;
	protected ref array<vector> m_aCoverPostDirections;
	
	protected static const string GROUP_PREFAB_NAME = "{000CD338713F2B5A}Prefabs/AI/Groups/Group_Base.et";
	
	//------------------------------------------------------------------------------------------------
	//! Available when the line from the camera to the cursor position intersects a building
	override bool CanBeShown(SCR_EditableEntityComponent hoveredEntity, notnull set<SCR_EditableEntityComponent> selectedEntities, vector cursorWorldPosition, int flags)
	{
		SCR_CameraEditorComponent editorCameraManager = SCR_CameraEditorComponent.Cast(SCR_CameraEditorComponent.GetInstance(SCR_CameraEditorComponent));
		if (!editorCameraManager)
			return false;
		
		SCR_ManualCamera camera = editorCameraManager.GetCamera();
		if (!camera)
			return false;
		
		TraceParam params = new TraceParam();
		params.Start = camera.GetOrigin();
		params.End = params.Start + 1.1 * (cursorWorldPosition - params.Start);
		params.Flags = TraceFlags.ENTS;
		GetGame().GetWorld().TraceMove(params, null);
		
		m_pBuilding = SCR_DestructibleBuildingEntity.Cast(params.TraceEnt);
		return m_pBuilding;
	}
	
	//------------------------------------------------------------------------------------------------
	override bool CanBePerformed(SCR_EditableEntityComponent hoveredEntity, notnull set<SCR_EditableEntityComponent> selectedEntities, vector cursorWorldPosition, int flags)
	{
		return CanBeShown(hoveredEntity, selectedEntities, cursorWorldPosition, flags);
	}
	
	//------------------------------------------------------------------------------------------------
	override void Perform(SCR_EditableEntityComponent hoveredEntity, notnull set<SCR_EditableEntityComponent> selectedEntities, vector cursorWorldPosition,int flags, int param = -1)
	{
		SCR_PlacingEditorComponent placingManager = SCR_PlacingEditorComponent.Cast(SCR_PlacingEditorComponent.GetInstance(SCR_PlacingEditorComponent, true, true));
		SCR_ContentBrowserEditorComponent.OpenBrowserLabelConfigInstance(m_ContentBrowserConfig);
		vector transform[4];
		m_pBuilding.GetWorldTransform(transform);
		placingManager.SetInstantPlacing(SCR_EditorPreviewParams.CreateParams(transform));
		placingManager.GetOnPlaceEntityServer().Insert(OnPlacingConfirmed);
		placingManager.GME_GetOnPlacingCanceledServer().Insert(DetachPlacingHandlers);
		
		array<Managed> components = {};
		m_pBuilding.FindComponents(SCR_AISmartActionSentinelComponent, components);
		m_aCoverPostPositions = {};
		m_aCoverPostDirections = {};
		
		foreach (Managed component : components)
		{
			SCR_AISmartActionSentinelComponent sentinel = SCR_AISmartActionSentinelComponent.Cast(component);
			vector pos = m_pBuilding.CoordToParent(sentinel.GetActionOffset());
			bool isNew = true;
			
			foreach (vector otherPos : m_aCoverPostPositions)
			{
				if (vector.DistanceSq(pos, otherPos) < 0.1)
				{
					isNew = false;
					break;
				}
			}
			
			if (!isNew)
				continue;
			
			m_aCoverPostPositions.Insert(pos);
			vector dir = m_pBuilding.CoordToParent(sentinel.GetLookPosition()) - m_pBuilding.CoordToParent(sentinel.GetActionOffset());
			m_aCoverPostDirections.Insert(dir.Normalized());
		}
		
		m_BuildingPosQuery = new GME_BuildingPosQuery(m_pBuilding);
		m_BuildingPosQuery.RunRandomQueries(10);
	}
	
	protected SCR_AIGroup m_pGroup;
	
	//------------------------------------------------------------------------------------------------
	protected void OnPlacingConfirmed(RplId prefabID, SCR_EditableEntityComponent entity, int playerID)
	{
		m_aPositions = m_BuildingPosQuery.GetPositions();
		m_aDirections = m_BuildingPosQuery.GetDirections();
		
		m_pGroup = SCR_AIGroup.Cast(entity.GetOwner());
		m_pGroup.GetOnAgentAdded().Insert(OnCharacterSpawned);
		
		DetachPlacingHandlers();
	}
		
	//------------------------------------------------------------------------------------------------
	protected void OnCharacterSpawned(AIAgent agent)
	{
		GetGame().GetCallqueue().CallLater(OnCharacterSpawnedDelayed, 1000, false, agent);
	}
	
	//------------------------------------------------------------------------------------------------
	protected void OnCharacterSpawnedDelayed(AIAgent agent)
	{
		EntitySpawnParams params = new EntitySpawnParams();
		params.TransformMode = ETransformMode.WORLD;
		float yaw;
		float waypointRadius;
		
		if (!m_aCoverPostPositions.IsEmpty())
		{
			int idx = m_aCoverPostPositions.GetRandomIndex();
			params.Transform[3] = m_aCoverPostPositions[idx];
			yaw = m_aCoverPostDirections[idx].ToYaw();
			m_aCoverPostPositions.Remove(idx);
			m_aCoverPostDirections.Remove(idx);
			waypointRadius = 0;
		}
		else if (!m_aPositions.IsEmpty())
		{
			int idx = m_aPositions.GetRandomIndex();
			params.Transform[3] = m_aPositions[idx];
			yaw = m_aDirections[idx].ToYaw();
			m_aPositions.Remove(idx);
			m_aDirections.Remove(idx);
			waypointRadius = 3;
		}
		else
		{
			SCR_EntityHelper.DeleteEntityAndChildren(agent.GetControlledEntity());
			return;
		}
		
		Math3D.AnglesToMatrix(Vector(yaw, 0, 0), params.Transform);
		SCR_EditableEntityComponent entity = SCR_EditableEntityComponent.Cast(agent.GetControlledEntity().FindComponent(SCR_EditableEntityComponent));
		
		SCR_AIGroup newGroup = SCR_AIGroup.Cast(GetGame().SpawnEntityPrefab(Resource.Load(GROUP_PREFAB_NAME), null, params));
		newGroup.SetFaction(m_pGroup.GetFaction());
		m_pGroup.RemoveAgent(agent);
		newGroup.AddAgent(agent);
		SCR_AIWaypoint waypoint = SCR_AIWaypoint.Cast(GetGame().SpawnEntityPrefab(Resource.Load("{93291E72AC23930F}Prefabs/AI/Waypoints/AIWaypoint_Defend.et"), null, params));
		waypoint.SetCompletionRadius(waypointRadius);
		newGroup.AddWaypoint(waypoint);
		entity.SetTransform(params.Transform);
	}
	
	//------------------------------------------------------------------------------------------------
	protected void DetachPlacingHandlers()
	{
		SCR_PlacingEditorComponent placingManager = SCR_PlacingEditorComponent.Cast(SCR_PlacingEditorComponent.GetInstance(SCR_PlacingEditorComponent, true, true));
		placingManager.GetOnPlaceEntityServer().Remove(OnPlacingConfirmed);
		placingManager.GME_GetOnPlacingCanceledServer().Remove(DetachPlacingHandlers);
	}
}
