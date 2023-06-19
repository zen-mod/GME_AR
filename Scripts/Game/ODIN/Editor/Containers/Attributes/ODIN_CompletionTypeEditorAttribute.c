[BaseContainerProps(), SCR_BaseEditorAttributeCustomTitle()]
class ODIN_CompletionTypeEditorAttribute : SCR_BasePresetsEditorAttribute
{
	override SCR_BaseEditorAttributeVar ReadVariable(Managed item, SCR_AttributesManagerEditorComponent manager)
	{
		SCR_EditableWaypointComponent editableWpComponent = SCR_EditableWaypointComponent.Cast(item);
		if (!editableWpComponent)
			return null;
		
		AIWaypoint wp =  AIWaypoint.Cast(editableWpComponent.GetOwner());
		if (!wp)
			return null;
			
		return SCR_BaseEditorAttributeVar.CreateInt(wp.GetCompletionType());
	}
	
	override void WriteVariable(Managed item, SCR_BaseEditorAttributeVar var, SCR_AttributesManagerEditorComponent manager, int playerID)
	{
		if (!var)
			return;
		
		SCR_EditableWaypointComponent editableWpComponent = SCR_EditableWaypointComponent.Cast(item);
		if (!editableWpComponent)
			return;
		
		AIWaypoint wp =  AIWaypoint.Cast(editableWpComponent.GetOwner());
		if (!wp)
			return;
		
		wp.SetCompletionType(var.GetInt());
	}
};