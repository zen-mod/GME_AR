//------------------------------------------------------------------------------------------------
class GME_ModulePlacingAction_EditAttributes : GME_ModulePlacingAction_Base
{
	//------------------------------------------------------------------------------------------------
	override void OnInitServer()
	{
		m_pModule.RunPlacingActionOwner();
	}
	
	//------------------------------------------------------------------------------------------------
	override void RunOwner(array<IEntity> params = null)
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
		m_pModule.OnPlacingActionCompleted();
	}
}
