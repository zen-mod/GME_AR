//------------------------------------------------------------------------------------------------
//! Entity Attribute for toggle medic role for unit
[BaseContainerProps(), SCR_BaseEditorAttributeCustomTitle()]
class GME_ToggleMedicRoleEditorAttribute : GME_BaseToggleRoleEditorAttribute
{
	//------------------------------------------------------------------------------------------------
	override EEditableEntityLabel GetToggledRole()
	{
		return EEditableEntityLabel.ROLE_MEDIC;
	}
}
