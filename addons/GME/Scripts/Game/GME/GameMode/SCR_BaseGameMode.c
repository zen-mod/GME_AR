//------------------------------------------------------------------------------------------------
modded class SCR_BaseGameMode : BaseGameMode
{
	[Attribute(defvalue: "NONE", desc: "Global AI skill", uiwidget: UIWidgets.ComboBox, enums: ParamEnumArray.FromEnum(EAISkill))]
	protected EAISkill m_eGME_GlobalAISkill;
	
	//------------------------------------------------------------------------------------------------
	//! Load mission header settings
	override protected void OnGameStart()
	{
		super.OnGameStart();
		
		SCR_MissionHeader header = SCR_MissionHeader.Cast(GetGame().GetMissionHeader());
		if (header)
			GME_SetGlobalAISkill(header.m_eGME_GlobalAISkill);
	}
	
	//------------------------------------------------------------------------------------------------
	//! Sets skill for all placed entities and future ones
	void GME_SetGlobalAISkill(EAISkill skill)
	{
		m_eGME_GlobalAISkill = skill;
		bool applyDefault = (skill == EAISkill.NONE);
		
		SCR_EditableEntityCore core = SCR_EditableEntityCore.Cast(SCR_EditableEntityCore.GetInstance(SCR_EditableEntityCore));
		if (!core)
			return;
		
		set<SCR_EditableEntityComponent> entities = new set<SCR_EditableEntityComponent>();	
		core.GetAllEntities(entities);
		
		foreach (SCR_EditableEntityComponent entity : entities)
		{
			SCR_EditableCharacterComponent editableChar = SCR_EditableCharacterComponent.Cast(entity);
			if (!editableChar)
				continue;
		
			IEntity char = editableChar.GetOwner();
			if (!char)
				continue;
	
			SCR_AICombatComponent combatComponent = SCR_AICombatComponent.Cast(char.FindComponent(SCR_AICombatComponent));
			if (!combatComponent)
				continue;
			
			if (applyDefault)
			{
				IEntityComponentSource combatComponentSource = combatComponent.GetComponentSource(char);
				if (combatComponentSource)
					continue;
				
				EAISkill defaultSkill;
				if (!combatComponentSource.Get("m_eAISkillDefault", defaultSkill))
					continue;
				
				combatComponent.SetAISkill(defaultSkill);
			}
			else
			{
				combatComponent.SetAISkill(skill);
			}
		}
	}
		
	//------------------------------------------------------------------------------------------------
	EAISkill GME_GetGlobalAISkill()
	{
		return m_eGME_GlobalAISkill;
	}
}
