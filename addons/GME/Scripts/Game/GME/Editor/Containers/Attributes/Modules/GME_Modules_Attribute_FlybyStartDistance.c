//------------------------------------------------------------------------------------------------
[BaseContainerProps(), SCR_BaseEditorAttributeCustomTitle()]
class GME_Modules_Attribute_FlybyStartDistance : SCR_BaseValueListEditorAttribute
{
	//------------------------------------------------------------------------------------------------
	override SCR_BaseEditorAttributeVar ReadVariable(Managed item, SCR_AttributesManagerEditorComponent manager)
	{
		GME_Modules_EditableModuleComponent editableModuleComponent = GME_Modules_EditableModuleComponent.Cast(item);
		if (!editableModuleComponent)
			return null;

		GME_Modules_AmbientFlyby module = GME_Modules_AmbientFlyby.Cast(editableModuleComponent.GetOwner());
		if (!module)
			return null;
			
		return SCR_BaseEditorAttributeVar.CreateFloat(module.GetStartDistance());
	}
	
	//------------------------------------------------------------------------------------------------
	override void WriteVariable(Managed item, SCR_BaseEditorAttributeVar var, SCR_AttributesManagerEditorComponent manager, int playerID)
	{
		if (!var)
			return;
		
		GME_Modules_EditableModuleComponent editableModuleComponent = GME_Modules_EditableModuleComponent.Cast(item);
		if (!editableModuleComponent)
			return;

		GME_Modules_AmbientFlyby module = GME_Modules_AmbientFlyby.Cast(editableModuleComponent.GetOwner());
		if (!module)
			return;

		module.SetStartDistance(var.GetFloat());
	}
}
