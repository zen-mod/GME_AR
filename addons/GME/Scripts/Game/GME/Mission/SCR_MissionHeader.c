//------------------------------------------------------------------------------------------------
modded class SCR_MissionHeader : MissionHeader
{
	[Attribute(defvalue: "NONE", desc: "Global AI skill", uiwidget: UIWidgets.ComboBox, enums: ParamEnumArray.FromEnum(EAISkill))]
	EAISkill m_eGME_GlobalAISkill;
}
