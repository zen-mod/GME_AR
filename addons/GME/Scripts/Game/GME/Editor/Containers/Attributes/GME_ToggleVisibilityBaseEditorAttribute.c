//------------------------------------------------------------------------------------------------
//! Entity Attribute for toggling visiblity
[BaseContainerProps(), SCR_BaseEditorAttributeCustomTitle()]
class GME_BaseToggleVisibilityEditorAttribute : SCR_ValidTypeBaseEditorAttribute
{
	//------------------------------------------------------------------------------------------------
	override SCR_BaseEditorAttributeVar ReadVariable(Managed item, SCR_AttributesManagerEditorComponent manager)
	{
		SCR_EditableEntityComponent editableEntity = SCR_EditableEntityComponent.Cast(item);
		if (!editableEntity)
			return null;
		
		return SCR_BaseEditorAttributeVar.CreateBool(editableEntity.GME_IsVisible());
	}
	
	//------------------------------------------------------------------------------------------------
	override void WriteVariable(Managed item, SCR_BaseEditorAttributeVar var, SCR_AttributesManagerEditorComponent manager, int playerID)
	{
		if (!var)
			return;
		
		SCR_EditableEntityComponent editableEntity = SCR_EditableEntityComponent.Cast(item);
		if (!editableEntity)
			return;
		
		editableEntity.GME_SetIsVisible(var.GetBool());
	}
}
