//------------------------------------------------------------------------------------------------
class GME_GroupHelper
{
	static const ResourceName GROUP_BASE_PREFAB = "{000CD338713F2B5A}Prefabs/AI/Groups/Group_Base.et";
	
	//------------------------------------------------------------------------------------------------
	static SCR_AIGroup GetGroup(GenericEntity entity)
	{
		PlayerManager playerManager = GetGame().GetPlayerManager();

		if (!playerManager)
			return null;

		SCR_GroupsManagerComponent groupManager = SCR_GroupsManagerComponent.Cast(GetGame().GetGameMode().FindComponent(SCR_GroupsManagerComponent));

		if (!groupManager)
			return null;

		int playerId = playerManager.GetPlayerIdFromControlledEntity(entity);

		return groupManager.GetPlayerGroup(playerId);
	}

	//------------------------------------------------------------------------------------------------
	static array<SCR_ChimeraCharacter> GetUnits(SCR_AIGroup group)
	{
		array<SCR_ChimeraCharacter> units = { };
		array<AIAgent> agents = { };
		group.GetAgents(agents);

		foreach (AIAgent agent : agents)
		{
			SCR_ChimeraCharacter char = SCR_ChimeraCharacter.Cast(agent.GetControlledEntity());
			if (char)
				units.Insert(char);
		}

		return units;
	}
	
	//------------------------------------------------------------------------------------------------
	//! Returns the unit count based on the prefab data of the group
	static int GetPrefabUnitCount(SCR_AIGroup group)
	{
		if (!group)
			return 0;
		
		EntityPrefabData data = group.GetPrefabData();
		if (!data)
			return 0;
		
		BaseContainer container = data.GetPrefab();
		array<ResourceName> units = {};
		container.Get("m_aUnitPrefabSlots", units);
		return units.Count();
	}
	
	//------------------------------------------------------------------------------------------------
	static SCR_AIGroup CreateEmptyGroup(vector pos, notnull SCR_Faction faction)
	{
		EntitySpawnParams params = new EntitySpawnParams();
			params.TransformMode = ETransformMode.WORLD;
			params.Transform[3] = pos;
		SCR_AIGroup group = SCR_AIGroup.Cast(GetGame().SpawnEntityPrefab(Resource.Load(GROUP_BASE_PREFAB), null, params));
		group.SetFaction(faction);
		return group;
	}

	//------------------------------------------------------------------------------------------------
	//! Disabling AI LODs means that AI will always be in LOD0 independent of how far players are from them.
	//! Hence, they will not get frozen. This is especially useful for QRFs
	static void EnableAILODs(SCR_AIGroup group, bool enabled)
	{
		group.GetOnAgentAdded().Remove(OnAgentAddedDisableLODs);
		int LOD = -1;

		if (!enabled)
		{
			LOD = 0;
			group.GetOnAgentAdded().Insert(OnAgentAddedDisableLODs)

		};

		group.SetPermanentLOD(LOD);
		array<AIAgent> agents = {};
		group.GetAgents(agents);

		foreach (AIAgent agent: agents)
		{
			agent.SetPermanentLOD(LOD);
		}
	}

	//------------------------------------------------------------------------------------------------
	static protected void OnAgentAddedDisableLODs(AIAgent agent)
	{
		agent.SetPermanentLOD(0);
	}
}
