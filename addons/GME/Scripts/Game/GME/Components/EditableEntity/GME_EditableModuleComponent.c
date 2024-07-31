//------------------------------------------------------------------------------------------------
[ComponentEditorProps(category: "GameScripted/Editor (Editables)", description: "", icon: "WBData/ComponentEditorProps/componentEditor.png")]
class GME_EditableModuleComponentClass : SCR_EditableSystemComponentClass
{
}

//------------------------------------------------------------------------------------------------
class GME_EditableModuleComponent : SCR_EditableSystemComponent
{
	protected GME_Modules_Base m_pModule;
	
	//------------------------------------------------------------------------------------------------
	override void OnPostInit(IEntity owner)
	{
		super.OnPostInit(owner);
		m_pModule = GME_Modules_Base.Cast(owner);
	}
	
	//------------------------------------------------------------------------------------------------
	override void SetTransform(vector transform[4], bool changedByUser = false)
	{
		vector oldTransform[4];
		GetTransform(oldTransform);
		m_pModule.OnMoved(oldTransform, transform);
		super.SetTransform(transform, changedByUser);
	}
	
	//------------------------------------------------------------------------------------------------
	override bool Delete(bool changedByUser = false, bool updateNavmesh = false)
	{
		m_pModule.OnDelete();
		return super.Delete(changedByUser, updateNavmesh);
	}
}
