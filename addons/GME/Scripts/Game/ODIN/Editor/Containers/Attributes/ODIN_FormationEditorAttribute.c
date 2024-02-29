/**
Entity Formation Attribute for getting and setting varriables in Editor Attribute window
*/
[BaseContainerProps(), SCR_BaseEditorAttributeCustomTitle()]
class ODIN_FormationEditorAttribute : SCR_BasePresetsEditorAttribute
{
	ref const array<string> m_formationNames = {"Wedge", "Column", "StaggeredColumn", "Line"};
	
	override SCR_BaseEditorAttributeVar ReadVariable(Managed item, SCR_AttributesManagerEditorComponent manager)
	{
		SCR_EditableEntityComponent editableEntity = SCR_EditableEntityComponent.Cast(item);
		if (!editableEntity)
			return null;
		
		IEntity owner =  editableEntity.GetOwner();
		if (!owner)
			return null;
		
		AIFormationComponent formationComp = AIFormationComponent.Cast(owner.FindComponent(AIFormationComponent));
		if (!formationComp)
			return null;
		
		int formationIdx = m_formationNames.Find(formationComp.GetFormation().GetName());
		if (formationIdx < 0)
			return null;
		
		return SCR_BaseEditorAttributeVar.CreateInt(formationIdx);
	}
	
	override void WriteVariable(Managed item, SCR_BaseEditorAttributeVar var, SCR_AttributesManagerEditorComponent manager, int playerID)
	{
		if (!var)
			return;
		
		SCR_EditableEntityComponent editableEntity = SCR_EditableEntityComponent.Cast(item);
		if (!editableEntity)
			return;
		
		IEntity owner =  editableEntity.GetOwner();
		if (!owner)
			return;
		
		AIFormationComponent formationComp = AIFormationComponent.Cast(owner.FindComponent(AIFormationComponent));
		if (!formationComp)
			return;
		
		formationComp.SetFormation(m_formationNames[var.GetInt()]);
	}
};