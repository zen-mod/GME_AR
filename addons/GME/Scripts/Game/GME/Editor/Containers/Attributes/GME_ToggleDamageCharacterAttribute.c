//------------------------------------------------------------------------------------------------
//! Entity attribute for toggle damage on unit
[BaseContainerProps(), SCR_BaseEditorAttributeCustomTitle()]
class GME_ToggleDamageCharacterAttribute : GME_ToggleDamageEditorAttribute
{
	//------------------------------------------------------------------------------------------------
	override SCR_BaseEditorAttributeVar ReadVariable(Managed item, SCR_AttributesManagerEditorComponent manager)
	{
		if (!SCR_EditableCharacterComponent.Cast(item))
			return null;
		
		return super.ReadVariable(item, manager);
	}
}
