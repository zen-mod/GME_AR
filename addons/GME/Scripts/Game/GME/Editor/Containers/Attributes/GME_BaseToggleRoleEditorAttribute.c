//------------------------------------------------------------------------------------------------
//! Entity Attribute for toggle role for unit
[BaseContainerProps(), SCR_BaseEditorAttributeCustomTitle()]
class GME_BaseToggleRoleEditorAttribute : SCR_BaseEditorAttribute
{
	//------------------------------------------------------------------------------------------------
	EEditableEntityLabel GetToggledRole();
	
	//------------------------------------------------------------------------------------------------
	override SCR_BaseEditorAttributeVar ReadVariable(Managed item, SCR_AttributesManagerEditorComponent manager)
	{
		SCR_EditableCharacterComponent editableChar = SCR_EditableCharacterComponent.Cast(item);
		if (!editableChar) 
			return null;
		
		array<EEditableEntityLabel> allLabels = {};
		editableChar.GetAllCharacterLabels(allLabels);
		
		array<EEditableEntityLabel> customLabels = {};
		editableChar.GetCustomCharacterLabels(customLabels);
		
		bool hasRole = allLabels.Contains(GetToggledRole());
		
		// Lock attribute if character got the role from prefab
		Enable(!hasRole || customLabels.Contains(GetToggledRole()));
		
		return SCR_BaseEditorAttributeVar.CreateBool(hasRole);
	}
	
	//------------------------------------------------------------------------------------------------
	override void WriteVariable(Managed item, SCR_BaseEditorAttributeVar var, SCR_AttributesManagerEditorComponent manager, int playerID)
	{
		if (!var) 
			return;
		
		SCR_EditableCharacterComponent editableChar = SCR_EditableCharacterComponent.Cast(item);
		if (!editableChar) 
			return;
		
		array<EEditableEntityLabel> customLabels = {};
		editableChar.GetCustomCharacterLabels(customLabels);
		
		if (var.GetBool() && !customLabels.Contains(GetToggledRole()))
			customLabels.Insert(GetToggledRole());
		else
			customLabels.RemoveItem(GetToggledRole());
		
		editableChar.SetCustomCharacterLabels_S(customLabels);
	}
}
