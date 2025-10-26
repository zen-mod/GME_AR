[BaseContainerProps(), SCR_BaseContainerCustomTitleUIInfo("m_Info")]
modded class SCR_SaveSessionToolbarAction : SCR_EditorToolbarAction
{
	//------------------------------------------------------------------------------------------------
	//! Enable saving for clients
	override bool CanBeShown(SCR_EditableEntityComponent hoveredEntity, notnull set<SCR_EditableEntityComponent> selectedEntities, vector cursorWorldPosition, int flags)
	{
		return GetGame().GetSaveGameManager().IsSavingAllowed();
	}
}
