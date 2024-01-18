[BaseContainerProps(), SCR_BaseContainerCustomTitleUIInfo("m_Info")]
modded class SCR_SaveSessionToolbarAction : SCR_EditorToolbarAction
{
	//---------------------------------------------------------------------------------------------
	//! Enable saving for clients on PC (disabled on Xbox, since broken)
	override bool CanBeShown(SCR_EditableEntityComponent hoveredEntity, notnull set<SCR_EditableEntityComponent> selectedEntities, vector cursorWorldPosition, int flags)
	{
		//--- Disallow on client if not PC, or in MP for "Save" version ("Save As" is allowed in MP)
		if ((!Replication.IsServer() && System.GetPlatform() != EPlatform.WINDOWS) || (!m_bSaveAs && Replication.IsRunning()))
			return false;
		
		//--- Disallow if editor save struct is not configured
		SCR_SaveLoadComponent saveLoadComponent = SCR_SaveLoadComponent.GetInstance();
		return saveLoadComponent && saveLoadComponent.ContainsStruct(SCR_EditorStruct);
	}
};