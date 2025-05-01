//------------------------------------------------------------------------------------------------
//! Entity Attribute for toggling character visiblity
[BaseContainerProps(), SCR_BaseEditorAttributeCustomTitle()]
class GME_ToggleVisibilityCharacterEditorAttribute : GME_BaseToggleVisibilityEditorAttribute
{
	//------------------------------------------------------------------------------------------------
	override SCR_BaseEditorAttributeVar ReadVariable(Managed item, SCR_AttributesManagerEditorComponent manager)
	{
		SCR_EditableCharacterComponent editableEntity = SCR_EditableCharacterComponent.Cast(item);
		if (!editableEntity)
			return null;
		
		return super.ReadVariable(item, manager);
	}
	
	//------------------------------------------------------------------------------------------------
	override void WriteVariable(Managed item, SCR_BaseEditorAttributeVar var, SCR_AttributesManagerEditorComponent manager, int playerID)
	{
		SCR_EditableCharacterComponent editableEntity = SCR_EditableCharacterComponent.Cast(item);
		if (!editableEntity)
			return;
		
		super.WriteVariable(item, var, manager, playerID);
	}
}
