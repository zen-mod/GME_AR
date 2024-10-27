//------------------------------------------------------------------------------------------------
class GME_Modules_InitAction_ChoosePassengers : GME_Modules_InitAction_ExtendEntity
{
	// The setter is actually an adder here, but we cannot update the description in derived classes
	// [Attribute(desc: "Setter on the module for the spawned entity")]
	// protected string m_sSpawnedEntitySetter;

	protected bool m_bSpawnedGroups = false;
		
	//------------------------------------------------------------------------------------------------
	override void OnStartServer()
	{
		SetEntityToExtend(IEntity.Cast(m_pModule.CallModuleMethod(m_sEntityToExtendGetter)));
		
		if (GetFreeVehicleCargoSlotCount() == 0)
		{
			m_pModule.OnInitActionCanceled();
			return;
		}

		super.OnStartServer();
	}
	
	//------------------------------------------------------------------------------------------------
	protected int GetFreeVehicleCargoSlotCount()
	{
		int count = 0;
		
		SCR_BaseCompartmentManagerComponent compartmentManager = SCR_BaseCompartmentManagerComponent.Cast(m_pEntityToExtend.FindComponent(SCR_BaseCompartmentManagerComponent));
		if (!compartmentManager)
			return 0;
		
		array<BaseCompartmentSlot> compartments = {};
		compartmentManager.GetCompartmentsOfType(compartments, ECompartmentType.CARGO);
		
		foreach (BaseCompartmentSlot compartment: compartments)
		{
			if (!compartment.IsOccupied())
				count++;
		}

		return count;
	}
	
	//------------------------------------------------------------------------------------------------
	override void OnPlacingConfirmedServer(RplId prefabID, SCR_EditableEntityComponent entity, int playerID)
	{
		SCR_AIGroup group;
		
		if (SCR_EditableGroupComponent.Cast(entity))
			group = SCR_AIGroup.Cast(entity.GetOwner());
		else
			group = GME_GroupHelper.GetGroup(SCR_ChimeraCharacter.Cast(entity.GetOwner()));
		
		m_pModule.CallModuleMethod(m_sSpawnedEntitySetter, group);
		m_bSpawnedGroups = true;
		
		m_pPlacingManager.GetOnPlaceEntityServer().Remove(OnPlacingConfirmedServer);
		m_pPlacingManager.GME_GetOnPlacingCanceledServer().Remove(OnPlacingCanceledServer);
		
		// The members of the new group are not spawned yet, so we have to subtract them manually from the prefab data
		if (GetFreeVehicleCargoSlotCount() - GME_GroupHelper.GetPrefabUnitCount(group) > 0)
			GetGame().GetCallqueue().CallLater(m_pModule.RpcInitActionMethod, 100, false, RplRcver.Owner, "OpenContentBrowser", null);
		else
			m_pModule.OnInitActionCompleted();
	}
	
	//------------------------------------------------------------------------------------------------
	override void OnPlacingCanceledServer()
	{
		m_pPlacingManager.GetOnPlaceEntityServer().Remove(OnPlacingConfirmedServer);
		m_pPlacingManager.GME_GetOnPlacingCanceledServer().Remove(OnPlacingCanceledServer);
		
		// Action is considered completed when at least one group has been spawned
		if (m_bSpawnedGroups)
			m_pModule.OnInitActionCompleted();
		else
			m_pModule.OnInitActionCanceled();
	}
}
