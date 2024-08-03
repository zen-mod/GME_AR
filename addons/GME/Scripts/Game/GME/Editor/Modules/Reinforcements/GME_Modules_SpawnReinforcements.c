//------------------------------------------------------------------------------------------------
class GME_Modules_SpawnReinforcementsClass : GME_Modules_BaseClass
{
}

//------------------------------------------------------------------------------------------------
class GME_Modules_SpawnReinforcements : GME_Modules_Base
{
	[Attribute(defvalue: "RTB_AND_DESPAWN", uiwidget: UIWidgets.SearchComboBox, desc: "Vehicle will return to spawn position and despawn if true. Stays at LZ if false.", enums: ParamEnumArray.FromEnum(GME_Reinforcements_EVehicleBehavior))]
	protected GME_Reinforcements_EVehicleBehavior m_eVehicleBehavior;
	
	protected ref EntitySpawnParams m_SpawnPointParams = new EntitySpawnParams();
	protected ref EntitySpawnParams m_RPParams = new EntitySpawnParams();
	protected ref EntitySpawnParams m_LZParams = new EntitySpawnParams();
	
	// Placing parameters
	protected Vehicle m_pVehicle;
	protected SCR_AIGroup m_pCrewGroup;
	protected SCR_AIGroup m_pPassengerGroup;
	
	protected const int RTB_DELAY_MS = 15000;
	
	
	//------------------------------------------------------------------------------------------------
	override void Run()
	{
		if (!m_pVehicle)
			return; // To Do: Handle non-interactive case
		
		if (!m_pCrewGroup)
			return; // To Do: Handle non-interactive case
		
		if (!m_pPassengerGroup)
			return; // To Do: Handle non-interactive case
				
		m_pCrewGroup.GetOnWaypointCompleted().Insert(OnLZReached);
		m_pCrewGroup.AddWaypoint(AIWaypoint.Cast(GetGame().SpawnEntityPrefab(Resource.Load("{3C790BB71C5CD960}PrefabsEditable/Auto/AI/Waypoints/E_AIWaypoint_ForcedMove.et"), GetWorld(), m_LZParams)));
		m_pPassengerGroup.AddWaypoint(AIWaypoint.Cast(GetGame().SpawnEntityPrefab(Resource.Load("{FFF9518F73279473}PrefabsEditable/Auto/AI/Waypoints/E_AIWaypoint_Move.et"), GetWorld(), m_RPParams)));
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
				GetGame().GetCallqueue().CallLater(DoRTB, RTB_DELAY_MS);
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
	
	//------------------------------------------------------------------------------------------------
	void SetPassengerGroup(SCR_AIGroup group)
	{
		m_pPassengerGroup = group;
	}
	
	//------------------------------------------------------------------------------------------------
	SCR_AIGroup GetPassengerGroup()
	{
		return m_pPassengerGroup;
	}
	
	//------------------------------------------------------------------------------------------------
	void SetLZPosition(vector pos)
	{
		m_LZParams.Transform[3] = pos;
	}
	
	//------------------------------------------------------------------------------------------------
	void SetRPPosition(vector pos)
	{
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
	
	//------------------------------------------------------------------------------------------------
	//! Temporary
	override protected void EOnInit(IEntity owner)
	{
		super.EOnInit(owner);
		
		if (!GetGame().InPlayMode() || !Replication.IsServer())
			return;
		
		SCR_EditableEntityCore core = SCR_EditableEntityCore.Cast(SCR_EditableEntityCore.GetInstance(SCR_EditableEntityCore));
		set<SCR_EditableEntityComponent> entities = new set<SCR_EditableEntityComponent>();
		core.GetAllEntities(entities, true);
		
		foreach (SCR_EditableEntityComponent entity : entities)
		{
			if (GME_Modules_LZ.Cast(entity.GetOwner()))
			{
				m_LZParams.TransformMode = ETransformMode.WORLD;
				entity.GetTransform(m_LZParams.Transform);
			}
			else if (GME_Modules_RallyPoint.Cast(entity.GetOwner()))
			{
				m_RPParams.TransformMode = ETransformMode.WORLD;
				entity.GetTransform(m_RPParams.Transform);
			}
		}
		
		GME_Modules_EditableModuleComponent edit = GME_Modules_EditableModuleComponent.Cast(FindComponent(GME_Modules_EditableModuleComponent));
		
		m_SpawnPointParams.TransformMode = ETransformMode.WORLD;
		edit.GetTransform(m_SpawnPointParams.Transform);
		float yaw = (m_LZParams.Transform[3] - m_SpawnPointParams.Transform[3]).ToYaw();
		Math3D.AnglesToMatrix(Vector(yaw, 0, 0), m_SpawnPointParams.Transform);
		edit.SetTransform(m_SpawnPointParams.Transform);
	}
}

//------------------------------------------------------------------------------------------------
enum GME_Reinforcements_EVehicleBehavior
{
	STAY_AT_LZ,
	RTB_AND_DESPAWN
}
