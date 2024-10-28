//------------------------------------------------------------------------------------------------
[BaseContainerProps(), SCR_BaseEditorAttributeCustomTitle()]
class GME_IntelTitleEditorAttribute : SCR_BasePresetsEditorAttribute
{
	//------------------------------------------------------------------------------------------------
	override SCR_BaseEditorAttributeVar ReadVariable(Managed item, SCR_AttributesManagerEditorComponent manager)
	{
		SCR_EditableEntityComponent editableEntity = SCR_EditableEntityComponent.Cast(item);
		if (!editableEntity)
			return null;
		
		IEntity owner =  editableEntity.GetOwner();
		if (!owner)
			return null;
		
		GME_IntelComponent intelComponent = GME_IntelComponent.Cast(owner.FindComponent(GME_IntelComponent));
		if (!intelComponent)
			return null;
			
		return SCR_BaseEditorAttributeVar.GME_CreateString(intelComponent.GetTitle());
	}
	
	//------------------------------------------------------------------------------------------------
	override void WriteVariable(Managed item, SCR_BaseEditorAttributeVar var, SCR_AttributesManagerEditorComponent manager, int playerID)
	{
		if (!var)
			return;
				
		SCR_EditableEntityComponent editableEntity = SCR_EditableEntityComponent.Cast(item);
		
		IEntity owner =  editableEntity.GetOwner();
		if (!owner)
			return;
		
		GME_IntelComponent intelComponent = GME_IntelComponent.Cast(owner.FindComponent(GME_IntelComponent));
		if (!intelComponent)
			return;

		intelComponent.SetTitle(var.GME_GetString());
	}
}
