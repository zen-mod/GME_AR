[BaseContainerProps(), SCR_BaseEditorAttributeCustomTitle()]
class ODIN_CharacterSkillEditorAttribute : SCR_BaseValueListEditorAttribute
{
	override SCR_BaseEditorAttributeVar ReadVariable(Managed item, SCR_AttributesManagerEditorComponent manager)
	{
		SCR_EditableEntityComponent editableEntity = SCR_EditableEntityComponent.Cast(item);
		if (!editableEntity)
			return null;
		
		IEntity owner =  editableEntity.GetOwner();
		if (!owner)
			return null;
		
		SCR_AICombatComponent combatComponent = SCR_AICombatComponent.Cast(owner.FindComponent(SCR_AICombatComponent));
		if (!combatComponent)
			return null;
			
		return SCR_BaseEditorAttributeVar.CreateInt(combatComponent.GetAISkill());
	}
	
	override void WriteVariable(Managed item, SCR_BaseEditorAttributeVar var, SCR_AttributesManagerEditorComponent manager, int playerID)
	{
		if (!var)
			return;
		
		SCR_EditableEntityComponent editableEntity = SCR_EditableEntityComponent.Cast(item);
		
		IEntity owner =  editableEntity.GetOwner();
		if (!owner)
			return;

		SCR_AICombatComponent combatComponent = SCR_AICombatComponent.Cast(owner.FindComponent(SCR_AICombatComponent));
		if (!combatComponent)
			return;
	
		combatComponent.SetAISkill(var.GetInt());
	}
};