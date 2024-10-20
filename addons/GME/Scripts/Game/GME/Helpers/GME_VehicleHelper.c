//------------------------------------------------------------------------------------------------
class GME_VehicleHelper
{
	static const int PASSENGER_EJECT_DELAY_MS = 2500;
	const ref array<ECompartmentType> DEFAULT_CREW_COMPARTMENT_TYPES = {ECompartmentType.PILOT, ECompartmentType.TURRET};
	const ref array<ECompartmentType> DEFAULT_PASSENGER_COMPARTMENT_TYPES = {ECompartmentType.CARGO};
	const ref array<ECompartmentType> ALL_COMPARTMENT_TYPES = {ECompartmentType.PILOT, ECompartmentType.TURRET, ECompartmentType.CARGO};
	
	//------------------------------------------------------------------------------------------------
	static SCR_AIGroup SpawnCrew(Vehicle vehicle, array<ECompartmentType> compartmentTypes = GME_VehicleHelper.DEFAULT_CREW_COMPARTMENT_TYPES)
	{
		SCR_EditableEntityComponent editableEntity = SCR_EditableEntityComponent.Cast(vehicle.FindComponent(SCR_EditableEntityComponent));
		if (!editableEntity)
			return null;
	
		SCR_Faction faction = GME_EditableEntityHelper.GetFactionFromLabels(editableEntity);
		if (!faction)
			return null;
	
		SCR_EntityCatalog catalogue = faction.GetFactionEntityCatalogOfType(EEntityCatalogType.CHARACTER);
		if (!catalogue)
			return null;
	
		array<SCR_EntityCatalogEntry> entries = {};
		catalogue.GetEntityListWithLabel(EEditableEntityLabel.ROLE_RIFLEMAN, entries);
		if (entries.IsEmpty())
			return null;
	
		Resource res = Resource.Load(entries[0].GetPrefab());
		if (!res.IsValid())
			return null;
	
		EntitySpawnParams params = new EntitySpawnParams();
			params.TransformMode = ETransformMode.WORLD;
			params.Transform[3] = vehicle.GetOrigin();
	
		SCR_BaseCompartmentManagerComponent compartmentManager = SCR_BaseCompartmentManagerComponent.Cast(vehicle.FindComponent(SCR_BaseCompartmentManagerComponent));
		if (!compartmentManager)
			return null;
	
		array<BaseCompartmentSlot> slots = {};
		compartmentManager.GetCompartmentsOfTypes(slots, compartmentTypes);
	
		SCR_AIGroup group = GME_GroupHelper.CreateEmptyGroup(params.Transform[3], faction);
	
		foreach (BaseCompartmentSlot slot : slots)
		{
			SCR_ChimeraCharacter char = SCR_ChimeraCharacter.Cast(GetGame().SpawnEntityPrefab(res, vehicle.GetWorld(), params));
			if (!char)
				continue;
			
			AIControlComponent aiControlComponent = AIControlComponent.Cast(char.FindComponent(AIControlComponent));
			if (!aiControlComponent)
				continue;
			
			aiControlComponent.ActivateAI();
			group.AddAgent(aiControlComponent.GetAIAgent());
			
			SCR_CompartmentAccessComponent compartmentAccess = SCR_CompartmentAccessComponent.Cast(char.GetCompartmentAccessComponent());
			if (compartmentAccess)
				compartmentAccess.MoveInVehicle(vehicle, slot.GetType(), true, slot);
		}
	
		return group;
	}
	
	//------------------------------------------------------------------------------------------------
	static array<SCR_AIGroup> SpawnPassengers(Vehicle vehicle, array<ResourceName> passengerGroupPrefabNames, array<ECompartmentType> compartmentTypes = GME_VehicleHelper.DEFAULT_PASSENGER_COMPARTMENT_TYPES)
	{
		return GME_VehicleHelper_SpawnPassengersJob(vehicle, passengerGroupPrefabNames, compartmentTypes).GetGroups();
	}
	
	//------------------------------------------------------------------------------------------------
	static int GetPassengerCount(Vehicle vehicle, array<ECompartmentType> compartmentTypes = GME_VehicleHelper.DEFAULT_PASSENGER_COMPARTMENT_TYPES)
	{
		int count = 0;
		
		SCR_BaseCompartmentManagerComponent compartmentManager = SCR_BaseCompartmentManagerComponent.Cast(vehicle.FindComponent(SCR_BaseCompartmentManagerComponent));
		if (!compartmentManager)
			return 0;
		
		array<BaseCompartmentSlot> compartmentSlots = {};
		compartmentManager.GetCompartmentsOfTypes(compartmentSlots, compartmentTypes);
		
		foreach (BaseCompartmentSlot slot : compartmentSlots)
		{
			if (slot.IsOccupied())
				count++;
		}
		
		return count;
	}
	
	//------------------------------------------------------------------------------------------------
	//! Ejects only cargo by default
	static void EjectPassengers(Vehicle vehicle, array<ECompartmentType> compartmentTypes = GME_VehicleHelper.DEFAULT_PASSENGER_COMPARTMENT_TYPES)
	{
		SCR_BaseCompartmentManagerComponent compartmentManager = SCR_BaseCompartmentManagerComponent.Cast(vehicle.FindComponent(SCR_BaseCompartmentManagerComponent));
		if (!compartmentManager)
			return;
		
		array<SCR_ChimeraCharacter> toEject = {};
		array<BaseCompartmentSlot> compartmentSlots = {};
		compartmentManager.GetCompartments(compartmentSlots);
		
		foreach (BaseCompartmentSlot slot : compartmentSlots)
		{
			if (!compartmentTypes.Contains(slot.GetType()))
				continue;
			
			SCR_ChimeraCharacter char = SCR_ChimeraCharacter.Cast(slot.GetOccupant());
			if (!char)
				continue;
			
			toEject.Insert(char);
		}
		
		EjectPassengers(toEject);
	}
	
	//------------------------------------------------------------------------------------------------
	static void EjectPassengers(array<SCR_ChimeraCharacter> characters)
	{
		map<int, ref array<CompartmentAccessComponent>> doorGroups = new map<int, ref array<CompartmentAccessComponent>>();
		
		foreach (SCR_ChimeraCharacter char : characters)
		{
			CompartmentAccessComponent compartmentAccess = char.GetCompartmentAccessComponent();
			
			BaseCompartmentSlot compartmentSlot = compartmentAccess.GetCompartment();
			if (!compartmentSlot)
				continue;
			
			int doorIdx = compartmentAccess.GetCompartment().PickDoorIndexForPoint(char.GetOrigin());
			if (!compartmentAccess.CanGetOutVehicleViaDoor(doorIdx))
				doorIdx = -1;
			
			if (!doorGroups.Contains(doorIdx))
			{
				doorGroups[doorIdx] = {compartmentAccess};
			}
			else
			{
				doorGroups[doorIdx].Insert(compartmentAccess);
			}
		}
		
		foreach (int doorIdx, array<CompartmentAccessComponent> group : doorGroups)
		{
			GetGame().GetCallqueue().CallLater(EjectDoorGroupStep, PASSENGER_EJECT_DELAY_MS, false, doorIdx, group);
		}	
	}
	
	//------------------------------------------------------------------------------------------------
	//! Ejects one member of a door group
	protected static void EjectDoorGroupStep(int doorIdx, array<CompartmentAccessComponent> group)
	{
		CompartmentAccessComponent compartmentAccess = group[0];
		group.Remove(0);
		
		if (!compartmentAccess.GetCompartment())
			return;
		
		ECloseDoorAfterActions closeDoor = ECloseDoorAfterActions.LEAVE_OPEN;
		if (group.IsEmpty())
			closeDoor =  ECloseDoorAfterActions.CLOSE_DOOR;
		else
			GetGame().GetCallqueue().CallLater(EjectDoorGroupStep, PASSENGER_EJECT_DELAY_MS, false, doorIdx, group);
		
		compartmentAccess.GetOutVehicle(EGetOutType.ANIMATED, doorIdx, closeDoor, false);
	}
}

//------------------------------------------------------------------------------------------------
//! Helper class for GME_VehicleHelper.SpawnPassengers
class GME_VehicleHelper_SpawnPassengersJob : Managed
{
	protected static ref array<ref GME_VehicleHelper_SpawnPassengersJob> s_aActiveJobs = {};
	
	protected Vehicle m_pVehicle;
	protected int m_iGroupCount;
	protected int m_iGroupCounter = 0;
	protected int m_iCompartmentSlotCount;
	protected ref array<SCR_AIGroup> m_aGroups = {};
	protected ref array<AIAgent> m_aMemberAgents = {};
	protected ref array<BaseCompartmentSlot> m_aCompartmentSlots = {};
	
	void GME_VehicleHelper_SpawnPassengersJob(notnull Vehicle vehicle, notnull array<ResourceName> passengerGroupPrefabNames, notnull array<ECompartmentType> compartmentTypes)
	{
		if (passengerGroupPrefabNames.IsEmpty())
			return;
		
		s_aActiveJobs.Insert(this);
		m_pVehicle = vehicle;
		
		SCR_BaseCompartmentManagerComponent compartmentManager = SCR_BaseCompartmentManagerComponent.Cast(vehicle.FindComponent(SCR_BaseCompartmentManagerComponent));
		if (!compartmentManager)
			return;
		
		compartmentManager.GetCompartmentsOfTypes(m_aCompartmentSlots, compartmentTypes);
		m_iCompartmentSlotCount = m_aCompartmentSlots.Count();
		
		EntitySpawnParams params = new EntitySpawnParams();
		params.TransformMode = ETransformMode.WORLD;
		vehicle.GetWorldTransform(params.Transform);
		m_iGroupCount = passengerGroupPrefabNames.Count();
		
		foreach (ResourceName resName : passengerGroupPrefabNames)
		{
			SCR_AIGroup group = SCR_AIGroup.Cast(GetGame().SpawnEntityPrefab(Resource.Load(resName), vehicle.GetWorld(), params));
			group.GME_GetOnDoneSpawningMembers().Insert(OnPassengersSpawned);
			m_aGroups.Insert(group);
		}
	}
	
	//------------------------------------------------------------------------------------------------
	protected void OnPassengersSpawned(SCR_AIGroup group)
	{
		group.GME_GetOnDoneSpawningMembers().Remove(OnPassengersSpawned);
		m_iGroupCounter++;
		array<AIAgent> agents = {};
		group.GetAgents(agents);
		m_aMemberAgents.InsertAll(agents);
		
		if (m_iGroupCounter < m_iGroupCount)
			return;
		
		foreach (int i, AIAgent agent : m_aMemberAgents)
		{
			SCR_ChimeraCharacter char = SCR_ChimeraCharacter.Cast(agent.GetControlledEntity());
			if (!char)
				continue;
			
			if (i < m_iCompartmentSlotCount)
			{
				SCR_CompartmentAccessComponent compartmentAccess = SCR_CompartmentAccessComponent.Cast(char.GetCompartmentAccessComponent());
				if (!compartmentAccess)
					continue;
				
				BaseCompartmentSlot slot = m_aCompartmentSlots[i];
				compartmentAccess.MoveInVehicle(m_pVehicle, slot.GetType(), true, slot);
			}
			// Delete members that don't fit in
			else
			{
				SCR_EntityHelper.DeleteEntityAndChildren(char);
			}
		}
		
		s_aActiveJobs.RemoveItem(this);
	}
	
	//------------------------------------------------------------------------------------------------
	array<SCR_AIGroup> GetGroups()
	{
		return m_aGroups;
	}
}
