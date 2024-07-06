//------------------------------------------------------------------------------------------------
[BaseContainerProps(), SCR_BaseContainerCustomTitleUIInfo("m_Info")]
class GME_GarrisonContextAction: SCR_BaseContextAction
{
	[Attribute(desc: "Active labels and white listed labels and label groups")]
	protected ref SCR_EditorContentBrowserDisplayConfig m_ContentBrowserConfig;
	
	protected SCR_DestructibleBuildingEntity m_pBuilding = null;
	
	//------------------------------------------------------------------------------------------------
	override bool CanBeShown(SCR_EditableEntityComponent hoveredEntity, notnull set<SCR_EditableEntityComponent> selectedEntities, vector cursorWorldPosition, int flags)
	{
		SCR_CameraEditorComponent editorCameraManager = SCR_CameraEditorComponent.Cast(SCR_CameraEditorComponent.GetInstance(SCR_CameraEditorComponent));
		if (!editorCameraManager)
			return false;
		
		SCR_ManualCamera camera = editorCameraManager.GetCamera();
		if (!camera)
			return false;
		
		TraceParam params = new TraceParam();
		params.Start = camera.GetOrigin();
		params.End = cursorWorldPosition;
		params.Flags = TraceFlags.ENTS;
		GetGame().GetWorld().TraceMove(params, null);
		
		m_pBuilding = SCR_DestructibleBuildingEntity.Cast(params.TraceEnt);
		return m_pBuilding;
	}
	
	//------------------------------------------------------------------------------------------------
	override bool CanBePerformed(SCR_EditableEntityComponent hoveredEntity, notnull set<SCR_EditableEntityComponent> selectedEntities, vector cursorWorldPosition, int flags)
	{
		return CanBeShown(hoveredEntity, selectedEntities, cursorWorldPosition, flags);
	}
	
	//------------------------------------------------------------------------------------------------
	override void Perform(SCR_EditableEntityComponent hoveredEntity, notnull set<SCR_EditableEntityComponent> selectedEntities, vector cursorWorldPosition,int flags, int param = -1)
	{
		SCR_PlacingEditorComponent placingManager = SCR_PlacingEditorComponent.Cast(SCR_PlacingEditorComponent.GetInstance(SCR_PlacingEditorComponent, true, true));
		SCR_ContentBrowserEditorComponent.OpenBrowserLabelConfigInstance(m_ContentBrowserConfig);
		vector transform[4];
		m_pBuilding.GetWorldTransform(transform);
		placingManager.SetInstantPlacing(SCR_EditorPreviewParams.CreateParams(transform));
	}
}
