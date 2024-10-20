//------------------------------------------------------------------------------------------------
class GME_Modules_SpawnReinforcementsClass : GME_Modules_BaseClass
{
}

//------------------------------------------------------------------------------------------------
class GME_Modules_SpawnReinforcements : GME_Modules_Base
{
	[Attribute(uiwidget: UIWidgets.ResourcePickerThumbnail, desc: "Prefab name of the vehicle", params: "et", category: "Params")]
	protected ResourceName m_sVehiclePrefabName;
	protected Vehicle m_pVehicle;
	protected SCR_AIGroup m_pCrewGroup;
	
	[Attribute(uiwidget: UIWidgets.ResourcePickerThumbnail, desc: "Prefab names of the passenger groups", params: "et", category: "Params")]
	protected ref array<ResourceName> m_aPassengerGroupPrefabNames;
	protected ref array<SCR_AIGroup> m_pPassengerGroups = {};
	
	[Attribute(defvalue: "RTB_AND_DESPAWN", uiwidget: UIWidgets.SearchComboBox, desc: "Vehicle will return to spawn position and despawn if true. Stays at LZ if false.", enums: ParamEnumArray.FromEnum(GME_Reinforcements_EVehicleBehavior), category: "Params")]
	protected GME_Reinforcements_EVehicleBehavior m_eVehicleBehavior;
	
	protected ref EntitySpawnParams m_SpawnPointParams = new EntitySpawnParams();
	protected ref EntitySpawnParams m_RPParams = new EntitySpawnParams();
	protected ref EntitySpawnParams m_LZParams = new EntitySpawnParams();
	
	protected const int RTB_DELAY_MS = 5000;
	
	//------------------------------------------------------------------------------------------------
	override void Run()
	{
		m_SpawnPointParams.TransformMode = ETransformMode.WORLD;
		GetWorldTransform(m_SpawnPointParams.Transform);
		
		if (!m_pVehicle)
			m_pVehicle = Vehicle.Cast(GetGame().SpawnEntityPrefab(Resource.Load(m_sVehiclePrefabName), GetWorld(), m_SpawnPointParams));
		
		if (!m_pCrewGroup)
			m_pCrewGroup = GME_VehicleHelper.SpawnCrew(m_pVehicle);
		
		if (!m_pPassengerGroups)
			m_pPassengerGroups = GME_VehicleHelper.SpawnPassengers(m_pVehicle, m_aPassengerGroupPrefabNames);
				
		m_pCrewGroup.GetOnWaypointCompleted().Insert(OnLZReached);
		m_pCrewGroup.AddWaypoint(AIWaypoint.Cast(GetGame().SpawnEntityPrefab(Resource.Load("{3C790BB71C5CD960}PrefabsEditable/Auto/AI/Waypoints/E_AIWaypoint_ForcedMove.et"), GetWorld(), m_LZParams)));
		
		foreach (SCR_AIGroup group : m_pPassengerGroups)
		{
			group.AddWaypoint(AIWaypoint.Cast(GetGame().SpawnEntityPrefab(Resource.Load("{FFF9518F73279473}PrefabsEditable/Auto/AI/Waypoints/E_AIWaypoint_Move.et"), GetWorld(), m_RPParams)));
		}
	}
	
	//------------------------------------------------------------------------------------------------
	protected void OnLZReached(AIWaypoint wp)
	{
		GME_VehicleHelper.EjectPassengers(m_pVehicle);
		
		// RTB or complete module after passengers have been dropped
		switch (m_eVehicleBehavior)
		{
			case GME_Reinforcements_EVehicleBehavior.STAY_AT_LZ:
			{
				SCR_EntityHelper.DeleteEntityAndChildren(this);
				break;
			}
			
			case GME_Reinforcements_EVehicleBehavior.RTB_AND_DESPAWN:
			{
				GetGame().GetCallqueue().CallLater(DoRTB, GME_VehicleHelper.GetPassengerCount(m_pVehicle) * GME_VehicleHelper.PASSENGER_EJECT_DELAY_MS + RTB_DELAY_MS);
				break;
			}
		}
				
		m_pCrewGroup.GetOnWaypointCompleted().Remove(OnLZReached);
	}
	
	//------------------------------------------------------------------------------------------------
	protected void DoRTB()
	{
		m_pCrewGroup.AddWaypoint(AIWaypoint.Cast(GetGame().SpawnEntityPrefab(Resource.Load("{3C790BB71C5CD960}PrefabsEditable/Auto/AI/Waypoints/E_AIWaypoint_ForcedMove.et"), GetWorld(), m_SpawnPointParams)));
		m_pCrewGroup.GetOnWaypointCompleted().Insert(OnRTBCompleted);
	}
	
	//------------------------------------------------------------------------------------------------
	//! Delete vehicle after RTB has been completed
	protected void OnRTBCompleted(AIWaypoint wp)
	{
		SCR_EntityHelper.DeleteEntityAndChildren(m_pCrewGroup);
		SCR_EntityHelper.DeleteEntityAndChildren(m_pVehicle);
		SCR_EntityHelper.DeleteEntityAndChildren(this);
	}
	
	void SetVehiclePrefabName(ResourceName resName)
	{
		m_sVehiclePrefabName = resName;
	}
	
	//------------------------------------------------------------------------------------------------
	void SetVehicle(Vehicle vehicle)
	{
		m_pVehicle = vehicle;
	}
	
	//------------------------------------------------------------------------------------------------
	Vehicle GetVehicle()
	{
		return m_pVehicle;
	}
	
	//------------------------------------------------------------------------------------------------
	void SetCrewGroup(SCR_AIGroup group)
	{
		m_pCrewGroup = group;
	}
	
	//------------------------------------------------------------------------------------------------
	SCR_AIGroup GetCrewGroup()
	{
		return m_pCrewGroup;
	}
	
	void SetPassengerGroupPrefabNames(array<ResourceName> resNames)
	{
		m_aPassengerGroupPrefabNames = resNames;
	}
	
	//------------------------------------------------------------------------------------------------
	void AddPassengerGroup(SCR_AIGroup group)
	{
		m_pPassengerGroups.Insert(group);
	}
	
	//------------------------------------------------------------------------------------------------
	array<SCR_AIGroup> GetPassengerGroups()
	{
		return m_pPassengerGroups;
	}
	
	//------------------------------------------------------------------------------------------------
	void SetLZPosition(vector pos)
	{
		m_LZParams.TransformMode = ETransformMode.WORLD;
		m_LZParams.Transform[3] = pos;
	}
	
	//------------------------------------------------------------------------------------------------
	void SetRPPosition(vector pos)
	{
		m_RPParams.TransformMode = ETransformMode.WORLD;		
		m_RPParams.Transform[3] = pos;
	}
	
	//------------------------------------------------------------------------------------------------
	void SetVehicleBehavior(GME_Reinforcements_EVehicleBehavior behavior)
	{
		m_eVehicleBehavior = behavior;
	}
	
	//------------------------------------------------------------------------------------------------
	GME_Reinforcements_EVehicleBehavior GetVehicleBehavior()
	{
		return m_eVehicleBehavior;
	}
}

//------------------------------------------------------------------------------------------------
enum GME_Reinforcements_EVehicleBehavior
{
	STAY_AT_LZ,
	RTB_AND_DESPAWN
}
