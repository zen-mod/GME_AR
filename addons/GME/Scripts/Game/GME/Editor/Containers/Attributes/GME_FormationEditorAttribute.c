//------------------------------------------------------------------------------------------------
//! Entity Formation Attribute for getting and setting varriables in Editor Attribute window
[BaseContainerProps(), SCR_BaseEditorAttributeCustomTitle()]
class GME_FormationEditorAttribute : SCR_BasePresetsEditorAttribute
{
	//------------------------------------------------------------------------------------------------
	override SCR_BaseEditorAttributeVar ReadVariable(Managed item, SCR_AttributesManagerEditorComponent manager)
	{
		SCR_EditableGroupComponent editableGroup = SCR_EditableGroupComponent.Cast(item);
		if (!editableGroup)
			return null;
		
		AIGroup group =  AIGroup.Cast(editableGroup.GetOwner());
		if (!group)
			return null;
		
		AIFormationComponent formationComp = AIFormationComponent.Cast(group.FindComponent(AIFormationComponent));
		if (!formationComp)
			return null;
		
		int formationID = typename.StringToEnum(SCR_EAIGroupFormation, formationComp.GetFormation().GetName());
		if (formationID < 0)
			return null;
		
		return SCR_BaseEditorAttributeVar.CreateInt(formationID);
	}
	
	//------------------------------------------------------------------------------------------------
	override void WriteVariable(Managed item, SCR_BaseEditorAttributeVar var, SCR_AttributesManagerEditorComponent manager, int playerID)
	{
		if (!var)
			return;
		
		SCR_EditableGroupComponent editableGroup = SCR_EditableGroupComponent.Cast(item);
		if (!editableGroup)
			return;
		
		AIGroup group =  AIGroup.Cast(editableGroup.GetOwner());
		if (!group)
			return;
		
		AIFormationComponent formationComp = AIFormationComponent.Cast(group.FindComponent(AIFormationComponent));
		if (!formationComp)
			return;
		
		formationComp.SetFormation(SCR_Enum.GetEnumName(SCR_EAIGroupFormation, var.GetInt()));
	}
}
