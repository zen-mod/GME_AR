//------------------------------------------------------------------------------------------------
//! Entity attribute for toggle damage on vehicle
[BaseContainerProps(), SCR_BaseEditorAttributeCustomTitle()]
class GME_ToggleDamageVehicleEditorAttribute : GME_ToggleDamageEditorAttribute
{
	//------------------------------------------------------------------------------------------------
	override SCR_BaseEditorAttributeVar ReadVariable(Managed item, SCR_AttributesManagerEditorComponent manager)
	{
		if (!SCR_EditableVehicleComponent.Cast(item))
			return null;
		
		return super.ReadVariable(item, manager);
	}
}
