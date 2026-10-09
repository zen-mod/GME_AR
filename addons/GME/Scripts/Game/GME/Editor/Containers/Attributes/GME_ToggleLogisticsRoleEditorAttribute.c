//------------------------------------------------------------------------------------------------
//! Entity Attribute for toggle logistics role for unit
[BaseContainerProps(), SCR_BaseEditorAttributeCustomTitle()]
class GME_ToggleLogisticsRoleEditorAttribute : GME_BaseToggleRoleEditorAttribute
{
	//------------------------------------------------------------------------------------------------
	override EEditableEntityLabel GetToggledRole()
	{
		return EEditableEntityLabel.TRAIT_VEHICLE_CREW;
	}
}
