/**
Entity Stance Attribute for getting and setting varriables in Editor Attribute window
*/

[BaseContainerProps(), SCR_BaseEditorAttributeCustomTitle()]
class ODIN_StanceEditorAttribute : SCR_BasePresetsEditorAttribute
{
	override SCR_BaseEditorAttributeVar ReadVariable(Managed item, SCR_AttributesManagerEditorComponent manager)
	{
		SCR_EditableEntityComponent editableEntity = SCR_EditableEntityComponent.Cast(item);
		if (!editableEntity)
			return null;
		
		IEntity owner = editableEntity.GetOwner();
		if (!owner)
			return null;
		
		CharacterControllerComponent charCtrlComponent = CharacterControllerComponent.Cast(owner.FindComponent(CharacterControllerComponent));
		if (!charCtrlComponent)
			return null;
				
		return SCR_BaseEditorAttributeVar.CreateInt(charCtrlComponent.GetStance());
	}
	
	override void WriteVariable(Managed item, SCR_BaseEditorAttributeVar var, SCR_AttributesManagerEditorComponent manager, int playerID)
	{
		if (!var)
			return;
		
		ECharacterStance stance = var.GetInt();
		ECharacterStanceChange stanceChange =  var.GetInt() + 1;
				
		SCR_EditableEntityComponent editableEntity = SCR_EditableEntityComponent.Cast(item);
		if (!editableEntity)
			return;
			
		IEntity owner = editableEntity.GetOwner();
		if (!owner)
			return;
		
		AIControlComponent aiCtrlComponent = AIControlComponent.Cast(owner.FindComponent(AIControlComponent));
		if (!aiCtrlComponent)
			return;
		
		AIAgent agent = aiCtrlComponent.GetControlAIAgent();
		if (!agent)
			return;
		
		SCR_AIInfoComponent infoComponent = SCR_AIInfoComponent.Cast(agent.FindComponent(SCR_AIInfoComponent));
		if (!infoComponent)
			return;
			
		CharacterControllerComponent charCtrlComponent = CharacterControllerComponent.Cast(owner.FindComponent(CharacterControllerComponent));
		if (!charCtrlComponent)
			return;
		
		infoComponent.SetStance(stance);
		charCtrlComponent.SetStanceChange(stanceChange);
	}
};
