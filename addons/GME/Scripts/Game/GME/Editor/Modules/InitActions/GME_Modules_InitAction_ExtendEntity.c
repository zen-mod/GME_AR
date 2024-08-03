//------------------------------------------------------------------------------------------------
class GME_Modules_InitAction_ExtendEntity : GME_Modules_InitAction_EditorBrowserBase
{
	[Attribute(desc: "Getter on the module for the entity to extend")]
	protected string m_sEntityToExtendGetter;
	
	//------------------------------------------------------------------------------------------------
	override void OnInitServer()
	{
		IEntity entityToExtend = m_pModule.GetPlacingParamServer(m_sEntityToExtendGetter);
		m_pModule.RunInitActionOwner({entityToExtend});
	}
	
	//------------------------------------------------------------------------------------------------
	override void RunOwner(array<IEntity> params = null)
	{
		SCR_EditableEntityComponent entityToExtend = SCR_EditableEntityComponent.Cast(params[0].FindComponent(SCR_EditableEntityComponent));
		super.RunOwner(params);
		SCR_ContentBrowserEditorComponent contentBrowserManager = SCR_ContentBrowserEditorComponent.Cast(SCR_ContentBrowserEditorComponent.GetInstance(SCR_ContentBrowserEditorComponent, true));
		contentBrowserManager.OpenBrowserExtended(entityToExtend, m_ContentBrowserConfig);
	}
}
