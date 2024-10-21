[BaseContainerProps(), SCR_BaseEditorAttributeCustomTitle()]
class GME_Modules_Attribute_PointCallsign : SCR_BasePresetsEditorAttribute
{
	override SCR_BaseEditorAttributeVar ReadVariable(Managed item, SCR_AttributesManagerEditorComponent manager)
	{
		GME_Modules_EditablePointComponent editablePoint = GME_Modules_EditablePointComponent.Cast(item);
		if (!editablePoint)
			return null;
			
		return SCR_BaseEditorAttributeVar.GME_CreateString(editablePoint.GetCallsign());
	}
	
	override void WriteVariable(Managed item, SCR_BaseEditorAttributeVar var, SCR_AttributesManagerEditorComponent manager, int playerID)
	{
		if (!var)
			return;
		
		GME_Modules_EditablePointComponent editablePoint = GME_Modules_EditablePointComponent.Cast(item);
		if (!editablePoint)
			return;

		editablePoint.SetCallsign(var.GME_GetString());
	}
}
