//------------------------------------------------------------------------------------------------
[ComponentEditorProps(category: "GameScripted/Editor (Editables)", description: "", icon: "WBData/ComponentEditorProps/componentEditor.png")]
class GME_EditablePointComponentClass : GME_EditableModuleComponentClass
{
	protected SCR_Faction m_pCallsignFaction;
	protected ref array<int> m_aCallsignIdcs = {};
	
	//------------------------------------------------------------------------------------------------
	LocalizedString GetNextCallsign()
	{
		if (m_aCallsignIdcs.IsEmpty())
		{
			FactionManager factionManager = GetGame().GetFactionManager();
			m_pCallsignFaction = SCR_Faction.Cast(factionManager.GetFactionByIndex(0));
			if (!m_pCallsignFaction)
				return string.Empty;
			
			m_aCallsignIdcs = m_pCallsignFaction.GetBaseCallsignIndexes();
			SCR_ArrayHelperT<int>.Shuffle(m_aCallsignIdcs);
		}
		
		if (m_aCallsignIdcs.IsEmpty())
			return string.Empty;
		
		int idx = m_aCallsignIdcs[0];
		m_aCallsignIdcs.Remove(0);
				
		SCR_MilitaryBaseCallsign callsign = m_pCallsignFaction.GetBaseCallsignByIndex(idx);
		if (callsign)
			return callsign.GetCallsignShort();
		else
			return string.Empty;
	}
}

//------------------------------------------------------------------------------------------------
class GME_EditablePointComponent  : GME_EditableModuleComponent
{
	protected ref GME_EditablePointUIInfo m_CustomUIInfo = new GME_EditablePointUIInfo();
	
	//------------------------------------------------------------------------------------------------
	override void OnPostInit(IEntity owner)
	{
		super.OnPostInit(owner);
		
		if (!GetGame().InPlayMode())
			return;
				
		m_CustomUIInfo.CopyFrom(GetInfo());
		SetInfoInstance(m_CustomUIInfo);
		GME_EditablePointComponentClass prefabData = GME_EditablePointComponentClass.Cast(GetEditableEntityData());
		m_CustomUIInfo.SetCallsign(prefabData.GetNextCallsign());
		Event_OnUIRefresh.Invoke();
	}
}
