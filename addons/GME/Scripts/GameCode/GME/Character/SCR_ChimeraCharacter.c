//------------------------------------------------------------------------------------------------
modded class SCR_ChimeraCharacter : ChimeraCharacter
{
	//------------------------------------------------------------------------------------------------
	//! Apply global AI skill
	override void EOnInit(IEntity owner)
	{
		super.EOnInit(owner);
		
		if (!GetGame().InPlayMode() || !Replication.IsServer())
			return;
		
		SCR_BaseGameMode gameMode = SCR_BaseGameMode.Cast(GetGame().GetGameMode());
		if (!gameMode)
			return;
		
		SCR_AICombatComponent combatComponent = SCR_AICombatComponent.Cast(FindComponent(SCR_AICombatComponent));
		if (!combatComponent)
			return;
		
		EAISkill skill = gameMode.GME_GetGlobalAISkill();
		if (skill == EAISkill.NONE)
			return;
	
		combatComponent.SetAISkill(skill);
	}
}
