//------------------------------------------------------------------------------------------------
[BaseContainerProps(), SCR_BaseEditorAttributeCustomTitle()]
class GME_Modules_Attribute_SelectRallyPoint : GME_Modules_Attribute_SelectPointBase
{
	//------------------------------------------------------------------------------------------------
	override SCR_BaseEditorAttributeVar ReadVariable(Managed item, SCR_AttributesManagerEditorComponent manager)
	{
		GME_Modules_EditableModuleComponent editableModuleComponent = GME_Modules_EditableModuleComponent.Cast(item);
		if (!editableModuleComponent)
			return null;
		
		GME_Modules_SpawnReinforcements module = GME_Modules_SpawnReinforcements.Cast(editableModuleComponent.GetOwner());
		if (!module)
			return null;
		
		if (module.IsInitDone())
			return null;
		
		return SCR_BaseEditorAttributeVar.CreateInt(0);
	}
	
	//------------------------------------------------------------------------------------------------
	override void WriteVariable(Managed item, SCR_BaseEditorAttributeVar var, SCR_AttributesManagerEditorComponent manager, int playerID)
	{
		super.WriteVariable(item, var, manager, playerID);
		
		GME_Modules_EditableModuleComponent editableModuleComponent = GME_Modules_EditableModuleComponent.Cast(item);
		if (!editableModuleComponent)
			return;
		
		GME_Modules_SpawnReinforcements module = GME_Modules_SpawnReinforcements.Cast(editableModuleComponent.GetOwner());
		if (!module)
			return;
		
		module.SetRPPosition(m_aAllPoints[var.GetInt()].GetOwner().GetOrigin());
	}
	
	//------------------------------------------------------------------------------------------------
	override protected bool IsPoint(SCR_EditableEntityComponent entity)
	{
		return GME_Modules_RallyPoint.Cast(entity.GetOwner());
	}
}
