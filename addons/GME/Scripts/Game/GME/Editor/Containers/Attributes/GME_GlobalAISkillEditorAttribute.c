//------------------------------------------------------------------------------------------------
[BaseContainerProps(), SCR_BaseEditorAttributeCustomTitle()]
class GME_GlobalAISkillEditorAttribute : GME_BaseAISkillEditorAttribute
{
	//------------------------------------------------------------------------------------------------
	override protected bool GetSkill(Managed item, out EAISkill skill)
	{
		SCR_BaseGameMode gameMode = SCR_BaseGameMode.Cast(item);
		if (!gameMode)
			return false;
		
		skill = gameMode.GME_GetGlobalAISkill();
		return true;
	}
	
	//------------------------------------------------------------------------------------------------
	override protected void SetSkill(Managed item, EAISkill skill)
	{
		SCR_BaseGameMode gameMode = SCR_BaseGameMode.Cast(item);
		if (!gameMode)
			return;
		
		gameMode.GME_SetGlobalAISkill(skill);
	}
}
