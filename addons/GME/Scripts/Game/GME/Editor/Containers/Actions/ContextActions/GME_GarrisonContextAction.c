//------------------------------------------------------------------------------------------------
[BaseContainerProps(), SCR_BaseContainerCustomTitleUIInfo("m_Info")]
class GME_GarrisonContextAction : SCR_BaseContextAction
{
	[Attribute(desc: "Active labels and white listed labels and label groups")]
	protected ref SCR_EditorContentBrowserDisplayConfig m_ContentBrowserConfig;
	
	protected SCR_DestructibleBuildingEntity m_pBuilding = null;
	protected ref GME_BuildingPosQuery m_BuildingPosQuery;
	
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
		m_BuildingPosQuery = new GME_BuildingPosQuery(m_pBuilding);
		m_BuildingPosQuery.RunRandomQuery(10);
	}
	
	protected SCR_AIGroup m_pGroup;
	
	//------------------------------------------------------------------------------------------------
	protected void OnPlacingConfirmed(RplId prefabID, SCR_EditableEntityComponent entity, int playerID)
	{
		m_pGroup = SCR_AIGroup.Cast(entity.GetOwner());
		m_iCurPosIdx = 0;
		m_pGroup.GetOnAgentAdded().Insert(OnCharacterSpawned);
		
		DetachPlacingHandlers();
	}
	
	protected int m_iCurPosIdx;
	
	protected void OnCharacterSpawned(AIAgent agent)
	{
		GetGame().GetCallqueue().CallLater(OnCharacterSpawnedDelayed, 1000, false, agent);
	}
	
	//------------------------------------------------------------------------------------------------
	protected void OnCharacterSpawnedDelayed(AIAgent agent)
	{
		EntitySpawnParams params = new EntitySpawnParams();
		params.TransformMode = ETransformMode.WORLD;
		
		float yaw = m_BuildingPosQuery.GetDirections()[m_iCurPosIdx].ToYaw();
		Math3D.AnglesToMatrix(Vector(yaw, 0, 0), params.Transform);
		params.Transform[3] = m_BuildingPosQuery.GetPositions()[m_iCurPosIdx];
		SCR_EditableEntityComponent entity = SCR_EditableEntityComponent.Cast(agent.GetControlledEntity().FindComponent(SCR_EditableEntityComponent));
		
		SCR_AIGroup newGroup = SCR_AIGroup.Cast(GetGame().SpawnEntityPrefab(Resource.Load(GROUP_PREFAB_NAME), null, params));
		newGroup.SetFaction(m_pGroup.GetFaction());
		m_pGroup.RemoveAgent(agent);
		newGroup.AddAgent(agent);
		SCR_AIWaypoint waypoint = SCR_AIWaypoint.Cast(GetGame().SpawnEntityPrefab(Resource.Load("{93291E72AC23930F}Prefabs/AI/Waypoints/AIWaypoint_Defend.et"), null, params));
		waypoint.SetCompletionRadius(0);
		newGroup.AddWaypoint(waypoint);
		entity.SetTransform(params.Transform);
		m_iCurPosIdx++;
	}
	
	//------------------------------------------------------------------------------------------------
	protected void DetachPlacingHandlers()
	{
		SCR_PlacingEditorComponent placingManager = SCR_PlacingEditorComponent.Cast(SCR_PlacingEditorComponent.GetInstance(SCR_PlacingEditorComponent, true, true));
		placingManager.GetOnPlaceEntityServer().Remove(OnPlacingConfirmed);
		placingManager.GME_GetOnPlacingCanceledServer().Remove(DetachPlacingHandlers);
	}
}
