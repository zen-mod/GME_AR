//------------------------------------------------------------------------------------------------
[BaseContainerProps(), SCR_BaseEditorAttributeCustomTitle()]
class GME_BaseAISkillEditorAttribute : SCR_BaseFloatValueHolderEditorAttribute
{
	//------------------------------------------------------------------------------------------------
	override SCR_BaseEditorAttributeVar ReadVariable(Managed item, SCR_AttributesManagerEditorComponent manager)
	{
		EAISkill skill;
		if (!GetSkill(item, skill))
			return null;
		
		if (m_aValues.IsEmpty())
			FillValues();
		
		foreach (int i, SCR_EditorAttributeFloatStringValueHolder valueHolder : m_aValues)
		{
			if ((int)valueHolder.GetFloatValue() == skill)
				return SCR_BaseEditorAttributeVar.CreateInt(i);
		}
			
		return null;
	}
	
	//------------------------------------------------------------------------------------------------
	override void WriteVariable(Managed item, SCR_BaseEditorAttributeVar var, SCR_AttributesManagerEditorComponent manager, int playerID)
	{
		if (!var)
			return;
		
		if (m_aValues.IsEmpty())
			FillValues();
		
		SCR_EditorAttributeFloatStringValueHolder valueHolder = m_aValues[var.GetInt()];
		SetSkill(item, valueHolder.GetFloatValue());
	}
	
	//------------------------------------------------------------------------------------------------
	//! To be implemented by derived
	protected bool GetSkill(Managed item, out EAISkill skill);
	
	//------------------------------------------------------------------------------------------------
	//! To be implemented by derived
	protected void SetSkill(Managed item, EAISkill skill);
	
	//------------------------------------------------------------------------------------------------
	override int GetEntries(notnull array<ref SCR_BaseEditorAttributeEntry> outEntries)
	{
		if (m_aValues.IsEmpty())
			FillValues();
		
		return super.GetEntries(outEntries);
	}
	
	//------------------------------------------------------------------------------------------------
	protected void FillValues()
	{
		array<EAISkill> skills = {};
		SCR_Enum.GetEnumValues(EAISkill, skills);
		m_aValues = {};
		
		foreach (EAISkill skill : skills)
		{
			SCR_EditorAttributeFloatStringValueHolder valueHolder = new SCR_EditorAttributeFloatStringValueHolder();
			valueHolder.SetFloatValue(skill);
			valueHolder.SetName(string.Format("#GME-Editor_Attribute_Skill_%1_Name", SCR_Enum.GetEnumName(EAISkill, skill)));
			m_aValues.Insert(valueHolder);
		}		
	}
}
