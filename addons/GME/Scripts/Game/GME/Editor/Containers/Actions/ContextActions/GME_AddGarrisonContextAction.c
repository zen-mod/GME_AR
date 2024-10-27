//------------------------------------------------------------------------------------------------
[BaseContainerProps(), SCR_BaseContainerCustomTitleUIInfo("m_Info")]
class GME_AddGarrisonContextAction : SCR_BaseContextAction
{
	[Attribute(defvalue: "{0B9374FB0BA34B16}PrefabsEditable/Modules/AI/E_GME_Modules_AddGarrison.et", uiwidget: UIWidgets.ResourcePickerThumbnail, desc: "Prefab name of the module to be spawned", params: "et")]
	protected ResourceName m_sModulePrefabName;
	
	protected SCR_DestructibleBuildingEntity m_pBuilding;
	
	//------------------------------------------------------------------------------------------------
	//! Available when the line from the camera to the cursor position intersects a building
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
		params.End = params.Start + 1.1 * (cursorWorldPosition - params.Start);
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
		vector transform[4];
		m_pBuilding.GetWorldTransform(transform);
		
		SCR_PlacingEditorComponent placingComponent = SCR_PlacingEditorComponent.Cast(SCR_PlacingEditorComponent.GetInstance(SCR_PlacingEditorComponent));
		if (!placingComponent)
			return;
		
		placingComponent.GME_RequestPlaceModule(m_sModulePrefabName, transform);
	}
}
