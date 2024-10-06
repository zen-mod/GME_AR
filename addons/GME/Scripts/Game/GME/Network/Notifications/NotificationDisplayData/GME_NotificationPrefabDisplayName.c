//------------------------------------------------------------------------------------------------
[BaseContainerProps(), SCR_BaseContainerCustomTitleEnum(ENotification, "m_NotificationKey")]
class GME_NotificationPrefabDisplayName : SCR_NotificationDisplayData
{
	//------------------------------------------------------------------------------------------------
	override string GetText(SCR_NotificationData data)
	{		
		int prefabID;
		data.GetParams(prefabID);
		
		SCR_PlacingEditorComponent placingManager = SCR_PlacingEditorComponent.Cast(SCR_PlacingEditorComponent.GetInstance(SCR_PlacingEditorComponent, true, true));
		if (!placingManager)
			return string.Empty;
		
		SCR_PlacingEditorComponentClass prefabData = SCR_PlacingEditorComponentClass.Cast(placingManager.GetEditorComponentData());
		if (!prefabData)
			return string.Empty;
		
		Resource res = Resource.Load(prefabData.GetPrefab(prefabID));
		if (!res.IsValid())
			return string.Empty;
		
		IEntitySource entitySource = SCR_BaseContainerTools.FindEntitySource(res);
		if (!entitySource)
			return string.Empty;
		
		IEntityComponentSource editableEntitySource = SCR_EditableEntityComponentClass.GetEditableEntitySource(entitySource);
		if (!editableEntitySource)
			return string.Empty;
		
		SCR_EditableEntityUIInfo editableEntityUiInfo = SCR_EditableEntityComponentClass.GetInfo(editableEntitySource);
		if (!editableEntityUiInfo)
			return string.Empty;
		
		data.SetNotificationTextEntries(editableEntityUiInfo.GetName());	
		return super.GetText(data);
	}
}
