/**
Entity Attribute for toggle unit visiblity
*/
[BaseContainerProps(), SCR_BaseEditorAttributeCustomTitle()]
class GME_ToggleVisibilityVehicleEditorAttribute : SCR_ValidTypeBaseEditorAttribute
{
	override SCR_BaseEditorAttributeVar ReadVariable(Managed item, SCR_AttributesManagerEditorComponent manager)
	{
		
		SCR_EditableEntityComponent editableEntity = SCR_EditableEntityComponent.Cast(item);
		if (!editableEntity) 
			return null;
		
		GenericEntity owner =  editableEntity.GetOwner();
		if (!owner) 
			return null;
		
		if (!Vehicle.Cast(owner)) 
			return null;
		
		bool visible = GME_VisibilityHelper.GetVisibility(owner);
		
		return SCR_BaseEditorAttributeVar.CreateBool(visible);		
	}
	override void WriteVariable(Managed item, SCR_BaseEditorAttributeVar var, SCR_AttributesManagerEditorComponent manager, int playerID)
	{
		if (!var) 
			return;
		
		SCR_EditableEntityComponent editableEntity = SCR_EditableEntityComponent.Cast(item);
		
		GenericEntity owner =  editableEntity.GetOwner();
		if (!owner) 
			return;
		
		GME_VisibilityHelper.SetVisibility(owner, var.GetBool());
	}
};
