//------------------------------------------------------------------------------------------------
[BaseContainerProps(), SCR_BaseEditorAttributeCustomTitle()]
class GME_AISkillEditorAttribute : GME_BaseAISkillEditorAttribute
{
	//------------------------------------------------------------------------------------------------
	override protected bool GetSkill(Managed item, out EAISkill skill)
	{
		SCR_EditableEntityComponent editableEntity = SCR_EditableEntityComponent.Cast(item);
		if (!editableEntity)
			return false;
		
		IEntity owner =  editableEntity.GetOwner();
		if (!owner || EntityUtils.IsPlayer(owner))
			return false;
		
		SCR_AICombatComponent combatComponent = SCR_AICombatComponent.Cast(owner.FindComponent(SCR_AICombatComponent));
		if (!combatComponent)
			return false;
		
		skill = combatComponent.GetAISkill();
		return true;
	}
	
	//------------------------------------------------------------------------------------------------
	override protected void SetSkill(Managed item, EAISkill skill)
	{
		SCR_EditableEntityComponent editableEntity = SCR_EditableEntityComponent.Cast(item);
		if (!editableEntity)
			return;
		
		IEntity owner = editableEntity.GetOwner();
		if (!owner)
			return;

		SCR_AICombatComponent combatComponent = SCR_AICombatComponent.Cast(owner.FindComponent(SCR_AICombatComponent));
		if (!combatComponent)
			return;
		
		combatComponent.SetAISkill(skill);
	}
	
	//------------------------------------------------------------------------------------------------
	//! Remove NONE value
	override protected void FillValues()
	{
		super.FillValues();
		m_aValues.RemoveOrdered(0);
	}
}
