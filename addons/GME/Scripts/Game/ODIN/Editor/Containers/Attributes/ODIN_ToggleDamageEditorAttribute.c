/**
Entity Attribute for toggle damage on unit
*/
[BaseContainerProps(), SCR_BaseEditorAttributeCustomTitle()]
class ODIN_ToggleDamageEditorAttribute : SCR_BaseEditorAttribute
{
	override SCR_BaseEditorAttributeVar ReadVariable(Managed item, SCR_AttributesManagerEditorComponent manager)
	{
		
		SCR_EditableEntityComponent editableEntity = SCR_EditableEntityComponent.Cast(item);
		if (!editableEntity) 
			return null;
		
		IEntity owner =  editableEntity.GetOwner();
		if (!owner) 
			return null;
		
		if (!SCR_ChimeraCharacter.Cast(owner))
			return null;
		
		SCR_DamageManagerComponent damageComponent = SCR_DamageManagerComponent.Cast(owner.FindComponent(SCR_DamageManagerComponent));
		if (!damageComponent) 
			return null;
		
		return SCR_BaseEditorAttributeVar.CreateBool(damageComponent.ODIN_IsDamageEnabled());		
	}
	override void WriteVariable(Managed item, SCR_BaseEditorAttributeVar var, SCR_AttributesManagerEditorComponent manager, int playerID)
	{
		if (!var) 
			return;
		
		SCR_EditableEntityComponent editableEntity = SCR_EditableEntityComponent.Cast(item);
		
		IEntity owner =  editableEntity.GetOwner();
		if (!owner) 
			return;
		
		// todo, move it to helper when verified it works 
		SCR_DamageManagerComponent damageComponent = SCR_DamageManagerComponent.Cast(owner.FindComponent(SCR_DamageManagerComponent));
		if (!damageComponent) 
			return;
		
		damageComponent.ODIN_SetDamageEnabled(var.GetBool());;
	}
};