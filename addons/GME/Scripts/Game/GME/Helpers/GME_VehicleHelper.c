//------------------------------------------------------------------------------------------------
class GME_VehicleHelper
{
	protected static const int PASSENGER_EJECT_DELAY_MS = 2500;
	static const ref array<ECompartmentType> PASSENGER_EJECT_DEFAULT_COMPARTMENT_TYPES = {ECompartmentType.CARGO};
	static const ref array<ECompartmentType> ALL_COMPARTMENT_TYPES = {ECompartmentType.PILOT, ECompartmentType.TURRET, ECompartmentType.CARGO};
	
	//------------------------------------------------------------------------------------------------
	//! Ejects only cargo by default
	static void EjectPassengers(Vehicle vehicle, array<ECompartmentType> compartmentTypes = GME_VehicleHelper.PASSENGER_EJECT_DEFAULT_COMPARTMENT_TYPES)
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