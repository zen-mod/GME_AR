//------------------------------------------------------------------------------------------------
modded class EditorBrowserDialogUI : EditorMenuBase
{
	//------------------------------------------------------------------------------------------------
	override protected void CloseWithoutPlacing()
	{
		SCR_PlacingEditorComponent placingManager = SCR_PlacingEditorComponent.Cast(SCR_PlacingEditorComponent.GetInstance(SCR_PlacingEditorComponent, true, true));
		if (placingManager)
			placingManager.GME_OnPlacingCanceled();
		
		super.CloseWithoutPlacing();
	}
}
