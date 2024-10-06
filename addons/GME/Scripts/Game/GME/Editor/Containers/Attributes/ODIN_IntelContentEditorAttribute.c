[BaseContainerProps(), SCR_BaseEditorAttributeCustomTitle()]
class ODIN_IntelContentAttribute : SCR_BasePresetsEditorAttribute
{
	override SCR_BaseEditorAttributeVar ReadVariable(Managed item, SCR_AttributesManagerEditorComponent manager)
	{
		SCR_EditableEntityComponent editableEntity = SCR_EditableEntityComponent.Cast(item);
		if (!editableEntity)
			return null;
		
		IEntity owner =  editableEntity.GetOwner();
		if (!owner)
			return null;
		
		ODIN_IntelManagerComponent intelManager = ODIN_IntelManagerComponent.Cast(owner.FindComponent(ODIN_IntelManagerComponent));
		if (!intelManager)
			return null;
			
		return ODIN_StringEditorAttributeVar.CreateString(intelManager.GetIntelContent());
	}
	
	override void WriteVariable(Managed item, SCR_BaseEditorAttributeVar var, SCR_AttributesManagerEditorComponent manager, int playerID)
	{
		if (!var) return;
		ODIN_StringEditorAttributeVar stringVar = ODIN_StringEditorAttributeVar.Cast(var);
		
		SCR_EditableEntityComponent editableEntity = SCR_EditableEntityComponent.Cast(item);
		
		IEntity owner =  editableEntity.GetOwner();
		if (!owner) return;
		
		ODIN_IntelManagerComponent intelManager = ODIN_IntelManagerComponent.Cast(owner.FindComponent(ODIN_IntelManagerComponent));
		if (!intelManager) return;

		intelManager.SetIntelContent(stringVar.GetString());
	}
};