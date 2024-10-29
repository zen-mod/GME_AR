//------------------------------------------------------------------------------------------------
[BaseContainerProps(), SCR_BaseContainerCustomTitleUIInfo("m_Info")]
modded class SCR_DisembarkContextAction : SCR_SelectedEntitiesContextAction
{
	//------------------------------------------------------------------------------------------------
	override void Perform(SCR_EditableEntityComponent selectedEntity, vector cursorWorldPosition)
	{
		if (!selectedEntity)
			return;
		
		Vehicle vehicle = Vehicle.Cast(selectedEntity.GetOwner());
		if (vehicle)
		{
			GME_VehicleHelper.EjectPassengers(vehicle, GME_VehicleHelper.ALL_COMPARTMENT_TYPES);
			return;
		}
		
		SCR_ChimeraCharacter char = SCR_ChimeraCharacter.Cast(selectedEntity.GetOwner());
		if (char)
		{
			GME_VehicleHelper.EjectPassengers({char});
			return;
		}
	}
}
