//------------------------------------------------------------------------------------------------
[BaseContainerProps(), SCR_BaseEditorAttributeCustomTitle()]
class GME_ModuleAttributes_VehicleBehavior : SCR_BasePresetsEditorAttribute
{
	//------------------------------------------------------------------------------------------------
	override SCR_BaseEditorAttributeVar ReadVariable(Managed item, SCR_AttributesManagerEditorComponent manager)
	{
		GME_EditableModuleComponent editableModuleComponent = GME_EditableModuleComponent.Cast(item);
		if (!editableModuleComponent)
			return null;
		
		GME_Modules_SpawnReinforcements module = GME_Modules_SpawnReinforcements.Cast(editableModuleComponent.GetOwner());
		if (!module)
			return null;
		
		return SCR_BaseEditorAttributeVar.CreateInt(module.GetVehicleBehavior());
	}
	
	//------------------------------------------------------------------------------------------------
	override void WriteVariable(Managed item, SCR_BaseEditorAttributeVar var, SCR_AttributesManagerEditorComponent manager, int playerID)
	{
		GME_EditableModuleComponent editableModuleComponent = GME_EditableModuleComponent.Cast(item);
		if (!editableModuleComponent)
			return;
		
		GME_Modules_SpawnReinforcements module = GME_Modules_SpawnReinforcements.Cast(editableModuleComponent.GetOwner());
		if (!module)
			return;
		
		module.SetVehicleBehavior(var.GetInt());
	}
}
