[BaseContainerProps(), SCR_BaseContainerCustomTitleUIInfo("m_Info")]
modded class SCR_LoadSessionToolbarAction : SCR_EditorToolbarAction
{
	//---------------------------------------------------------------------------------------------
	//! Enable loading for clients on PC (disabled on Xbox, since broken)
	override bool CanBeShown(SCR_EditableEntityComponent hoveredEntity, notnull set<SCR_EditableEntityComponent> selectedEntities, vector cursorWorldPosition, int flags)
	{
		//--- Disallow if editor save struct is not configured
		SCR_SaveLoadComponent saveLoadComponent = SCR_SaveLoadComponent.GetInstance();
		if (!saveLoadComponent || !saveLoadComponent.ContainsStruct(SCR_EditorStruct))
			return false;
		
		if (Replication.IsServer())
		{
			//--- Loading is always available for the host and in singleplayer
			return true;
		}
		else
		{
			//--- Loading is only available on PC in MP
			return (System.GetPlatform() == EPlatform.WINDOWS);
		}
	}
}