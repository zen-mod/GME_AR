//------------------------------------------------------------------------------------------------
class GME_Modules_AddGarrisonClass : GME_Modules_BaseClass
{
}

//------------------------------------------------------------------------------------------------
class GME_Modules_AddGarrison : GME_Modules_Base
{
	[Attribute(uiwidget: UIWidgets.ResourcePickerThumbnail, desc: "Prefab name of the group", params: "et", category: "Params")]
	protected ResourceName m_sGroupPrefabName;
	protected SCR_AIGroup m_pGroup;
	
	protected SCR_DestructibleBuildingEntity m_pBuilding;
	protected ref GME_BuildingPosQuery m_BuildingPosQuery;
	protected ref array<vector> m_aCoverPostPositions;
	protected ref array<vector> m_aCoverPostDirections;
	
	//------------------------------------------------------------------------------------------------
	//! Spawn group if needed and wait for members to spawn
	override void Run()
	{
		if (!m_pBuilding)
		{
			GME_QueryNearestEntity query = new GME_QueryNearestEntity(0.01);
			m_pBuilding = SCR_DestructibleBuildingEntity.Cast(query.GetEntity(GetOrigin()));
		}
		
		if (!m_pBuilding)
			return;
		
		if (!m_pGroup)
		{
			EntitySpawnParams params = new EntitySpawnParams();
			params.TransformMode = ETransformMode.WORLD;
			m_pBuilding.GetTransform(params.Transform);
			m_pGroup = SCR_AIGroup.Cast(GetGame().SpawnEntityPrefab(Resource.Load(m_sGroupPrefabName), GetWorld(), params));
		}
		
		if (!m_pGroup)
			return;
		
		if (m_pGroup.GME_IsMemberSpawningDone())
			OnGroupMembersSpawned(m_pGroup);
		else
			m_pGroup.GME_GetOnDoneSpawningMembers().Insert(OnGroupMembersSpawned);
	}
	
	//------------------------------------------------------------------------------------------------
	//! Query positions after members are spawned
	protected void OnGroupMembersSpawned(SCR_AIGroup group)
	{
		group.GME_GetOnDoneSpawningMembers().Remove(OnGroupMembersSpawned);
		int posCount = group.GetAgentsCount();
		
		vector transform[4];
		m_pBuilding.GetWorldTransform(transform);
		
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
		
		posCount -= m_aCoverPostPositions.Count();
		if (posCount <= 0)
			return OnBuildingPosQueryDone({}, {});
		
		m_BuildingPosQuery = new GME_BuildingPosQuery(m_pBuilding);
		m_BuildingPosQuery.GetOnQueryDone().Insert(OnBuildingPosQueryDone);
		m_BuildingPosQuery.RunRandomQueries(posCount);
	}
	
	//------------------------------------------------------------------------------------------------
	//! Assign building positions that were found
	protected void OnBuildingPosQueryDone(array<vector> positions, array<vector> directions)
	{
		array<AIAgent> agents = {};
		m_pGroup.GetAgents(agents);
		
		foreach (AIAgent agent : agents)
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
			else if (!positions.IsEmpty())
			{
				int idx = positions.GetRandomIndex();
				params.Transform[3] = positions[idx];
				yaw = directions[idx].ToYaw();
				positions.Remove(idx);
				directions.Remove(idx);
				waypointRadius = 3;
			}
			else
			{
				SCR_EntityHelper.DeleteEntityAndChildren(agent.GetControlledEntity());
				continue;
			}
			
			Math3D.AnglesToMatrix(Vector(yaw, 0, 0), params.Transform);
			SCR_EditableEntityComponent entity = SCR_EditableEntityComponent.Cast(agent.GetControlledEntity().FindComponent(SCR_EditableEntityComponent));
			
			SCR_AIGroup newGroup = GME_GroupHelper.CreateEmptyGroup(params.Transform[3], SCR_Faction.Cast(m_pGroup.GetFaction()));
			m_pGroup.RemoveAgent(agent);
			newGroup.AddAgent(agent);
			SCR_AIWaypoint waypoint = SCR_AIWaypoint.Cast(GetGame().SpawnEntityPrefab(Resource.Load("{93291E72AC23930F}Prefabs/AI/Waypoints/AIWaypoint_Defend.et"), null, params));
			waypoint.SetCompletionRadius(waypointRadius);
			newGroup.AddWaypoint(waypoint);
			entity.SetTransform(params.Transform);
		}
		
		SCR_EntityHelper.DeleteEntityAndChildren(this);
	}
	
	//------------------------------------------------------------------------------------------------
	protected void OnCharacterSpawned(AIAgent agent)
	{

	}
	
	//------------------------------------------------------------------------------------------------
	void SetBuilding(notnull SCR_DestructibleBuildingEntity building)
	{
		m_pBuilding = building;
	}
	
	//------------------------------------------------------------------------------------------------
	void SetGroup(notnull IEntity entity)
	{
		SCR_AIGroup group = SCR_AIGroup.Cast(entity);
		if (group)
		{
			m_pGroup = group;
			return;
		}
		
		SCR_ChimeraCharacter char = SCR_ChimeraCharacter.Cast(entity);
		if (char)
		{
			m_pGroup = GME_GroupHelper.GetGroup(char);
			return;
		}
	}
}
