//------------------------------------------------------------------------------------------------
[BaseContainerProps(), SCR_BaseContainerCustomTitleUIInfo("m_Info")]
class GME_AddGarrisonContextAction : SCR_BaseContextAction
{
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
		EntitySpawnParams params = new EntitySpawnParams();
		params.TransformMode = ETransformMode.WORLD;
		m_pBuilding.GetWorldTransform(params.Transform);
		
		GME_Modules_AddGarrison module = GME_Modules_AddGarrison.Cast(GetGame().SpawnEntityPrefab(Resource.Load("{0B9374FB0BA34B16}PrefabsEditable/Modules/AI/E_GME_Modules_AddGarrison.et"), null, params));
		if (!module)
			return;
		
		module.SetBuilding(m_pBuilding);
		
		GME_Modules_EditableModuleComponent editableModuleComponent = GME_Modules_EditableModuleComponent.Cast(SCR_EditableEntityComponent.GetEditableEntity(module));
		if (!editableModuleComponent)
			return;
		
		editableModuleComponent.OnInitServer();
	}
}
