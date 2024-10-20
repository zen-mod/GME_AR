//------------------------------------------------------------------------------------------------
class GME_Modules_InitAction_ExtendEntity : GME_Modules_InitAction_EditorBrowserBase
{
	[Attribute(desc: "Getter on the module for the entity to extend")]
	protected string m_sEntityToExtendGetter;
	protected IEntity m_pEntityToExtend;
	
	//------------------------------------------------------------------------------------------------
	void SetEntityToExtend(IEntity entity)
	{
		m_pEntityToExtend = entity;
	}
	
	//------------------------------------------------------------------------------------------------
	override void OnStartServer()
	{
		IEntity entity = IEntity.Cast(m_pModule.CallModuleMethod(m_sEntityToExtendGetter));
		m_pModule.RpcInitActionMethod(RplRcver.Owner, "SetEntityToExtend", entity);
		super.OnStartServer();
	}
	
	//------------------------------------------------------------------------------------------------
	override void OpenContentBrowser()
	{
		super.OpenContentBrowser();
		SCR_ContentBrowserEditorComponent contentBrowserManager = SCR_ContentBrowserEditorComponent.Cast(SCR_ContentBrowserEditorComponent.GetInstance(SCR_ContentBrowserEditorComponent, true));
		contentBrowserManager.OpenBrowserExtended(SCR_EditableEntityComponent.Cast(m_pEntityToExtend.FindComponent(SCR_EditableEntityComponent)), m_ContentBrowserConfig);
	}
}
