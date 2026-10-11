//------------------------------------------------------------------------------------------------
//! Entity Attribute for toggle eingineer role for unit
[BaseContainerProps(), SCR_BaseEditorAttributeCustomTitle()]
class GME_ToggleEngineerRoleEditorAttribute : GME_BaseToggleRoleEditorAttribute
{
	//------------------------------------------------------------------------------------------------
	override EEditableEntityLabel GetToggledRole()
	{
		return EEditableEntityLabel.ROLE_SAPPER;
	}
}
