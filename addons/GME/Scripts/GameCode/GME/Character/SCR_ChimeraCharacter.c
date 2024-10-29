//------------------------------------------------------------------------------------------------
modded class SCR_ChimeraCharacter : ChimeraCharacter
{
	[RplProp(onRplName: "GME_OnVisibilityValueUpdated")]
	protected bool m_bGME_isVisible = true;
	
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
	
	//------------------------------------------------------------------------------------------------
	void GME_SetVisibility(bool visible)
	{
		m_bGME_isVisible = visible;
		Replication.BumpMe();
		GME_OnVisibilityValueUpdated();
	}
	
	//------------------------------------------------------------------------------------------------
	protected void GME_OnVisibilityValueUpdated()
	{
		if (m_bGME_isVisible)
			SetFlags(EntityFlags.VISIBLE | EntityFlags.TRACEABLE);
		else
			ClearFlags(EntityFlags.VISIBLE | EntityFlags.TRACEABLE);
	}
}
