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
		
		SCR_AIGroup group = SCR_AIGroup.Cast(owner);
		if (!group)
			return null;
		
		
		AIAgent leader = group.GetLeaderAgent();
		if (!leader)
			return null;
		
		CharacterControllerComponent charCtrlComp = CharacterControllerComponent.Cast(leader.GetControlledEntity().FindComponent(CharacterControllerComponent));
		if (!charCtrlComp)
			return null;
				
		return SCR_BaseEditorAttributeVar.CreateInt(charCtrlComp.GetStance());
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
		
		SCR_AIGroup group = SCR_AIGroup.Cast(owner);
		if (!group)
			return;
		
		array<AIAgent> agents = {};
		group.GetAgents(agents);
		foreach (AIAgent agent : agents)
		{
			SCR_AIInfoComponent infoComp = SCR_AIInfoComponent.Cast(agent.FindComponent(SCR_AIInfoComponent));
			if (!infoComp)
				continue;
			
			CharacterControllerComponent charCtrlComp = CharacterControllerComponent.Cast(agent.GetControlledEntity().FindComponent(CharacterControllerComponent));
			if (!charCtrlComp)
				continue;
			
			infoComp.SetStance(stance);
			charCtrlComp.SetStanceChange(stanceChange);
		};
	}
};
