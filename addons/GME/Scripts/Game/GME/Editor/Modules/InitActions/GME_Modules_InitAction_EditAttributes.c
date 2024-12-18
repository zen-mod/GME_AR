//------------------------------------------------------------------------------------------------
class GME_Modules_InitAction_EditAttributes : GME_Modules_InitAction_Base
{
	//------------------------------------------------------------------------------------------------
	override void OnStartServer()
	{
		m_pModule.RpcInitActionMethod(RplRcver.Owner, "StartEditingOwner");
	}
	
	//------------------------------------------------------------------------------------------------
	void StartEditingOwner()
	{
		SCR_AttributesManagerEditorComponent attributesManager = SCR_AttributesManagerEditorComponent.Cast(SCR_AttributesManagerEditorComponent.GetInstance(SCR_AttributesManagerEditorComponent));
		attributesManager.StartEditing(m_pModule);
		attributesManager.GetOnAttributesConfirm().Insert(OnEditingCompleted);
		attributesManager.GetOnAttributesCancel().Insert(OnEditingCompleted);
	}
	
	//------------------------------------------------------------------------------------------------
	void OnEditingCompleted()
	{
		SCR_AttributesManagerEditorComponent attributesManager = SCR_AttributesManagerEditorComponent.Cast(SCR_AttributesManagerEditorComponent.GetInstance(SCR_AttributesManagerEditorComponent));
		attributesManager.GetOnAttributesConfirm().Remove(OnEditingCompleted);
		attributesManager.GetOnAttributesCancel().Remove(OnEditingCompleted);
		m_pModule.OnInitActionCompleted();
	}
}
