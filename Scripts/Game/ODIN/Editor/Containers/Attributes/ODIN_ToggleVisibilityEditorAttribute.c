/**
Entity Attribute for toggle unit visiblity
*/
[BaseContainerProps(), SCR_BaseEditorAttributeCustomTitle()]
class ODIN_ToggleVisibilityEditorAttribute : SCR_BaseEditorAttribute
{
	override SCR_BaseEditorAttributeVar ReadVariable(Managed item, SCR_AttributesManagerEditorComponent manager)
	{
		
		SCR_EditableEntityComponent editableEntity = SCR_EditableEntityComponent.Cast(item);
		if (!editableEntity) return null;
		
		IEntity owner =  editableEntity.GetOwner();
		if (!owner) return null;
		
		int visible = owner.GetFlags() & EntityFlags.VISIBLE;
		
		return SCR_BaseEditorAttributeVar.CreateBool(visible);		
	}
	override void WriteVariable(Managed item, SCR_BaseEditorAttributeVar var, SCR_AttributesManagerEditorComponent manager, int playerID)
	{
		if (!var) return;
		SCR_EditableEntityComponent editableEntity = SCR_EditableEntityComponent.Cast(item);
		
		IEntity owner =  editableEntity.GetOwner();
		if (!owner) return;

		// if true, we gotta set the flags, otherwise we clear them. As set is OR action, so we can't remove flags by setting 0
		if (var.GetBool())
		{
			owner.SetFlags(EntityFlags.VISIBLE|EntityFlags.TRACEABLE, 1);
		} 
		else 
		{
			owner.ClearFlags(EntityFlags.VISIBLE|EntityFlags.TRACEABLE);
		}
	}
};