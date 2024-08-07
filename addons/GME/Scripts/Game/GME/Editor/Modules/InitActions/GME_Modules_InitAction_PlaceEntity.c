//------------------------------------------------------------------------------------------------
class GME_Modules_InitAction_PlaceEntity : GME_Modules_InitAction_EditorBrowserBase
{
	//------------------------------------------------------------------------------------------------
	override void RunOwner(array<IEntity> params = null)
	{
		super.RunOwner(params);
		vector transform[4];
		m_pModule.GetTransform(transform);
		m_pPlacingManager.SetInstantPlacing(SCR_EditorPreviewParams.CreateParams(transform));
		SCR_ContentBrowserEditorComponent.OpenBrowserLabelConfigInstance(m_ContentBrowserConfig);
	}
}
