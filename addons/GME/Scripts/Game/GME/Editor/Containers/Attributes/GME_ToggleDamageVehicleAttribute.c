/**
Entity Attribute for toggle damage on unit
*/
[BaseContainerProps(), SCR_BaseEditorAttributeCustomTitle()]
class GME_ToggleDamageVehicleEditorAttribute : SCR_BaseEditorAttribute
{
	override SCR_BaseEditorAttributeVar ReadVariable(Managed item, SCR_AttributesManagerEditorComponent manager)
	{
		
		SCR_EditableEntityComponent editableEntity = SCR_EditableEntityComponent.Cast(item);
		if (!editableEntity) 
			return null;
		
		IEntity owner =  editableEntity.GetOwner();
		if (!owner) 
			return null;
		
		if (!Vehicle.Cast(owner))
			return null;
		
		DamageManagerComponent damageComponent = DamageManagerComponent.Cast(owner.FindComponent(DamageManagerComponent));
		
		if (!damageComponent) 
			return null;
		
		return SCR_BaseEditorAttributeVar.CreateBool(damageComponent.IsDamageHandlingEnabled());		
	}
	override void WriteVariable(Managed item, SCR_BaseEditorAttributeVar var, SCR_AttributesManagerEditorComponent manager, int playerID)
	{
		if (!var) 
			return;
		
		SCR_EditableEntityComponent editableEntity = SCR_EditableEntityComponent.Cast(item);
		
		IEntity owner =  editableEntity.GetOwner();
		if (!owner) 
			return;
		
		DamageManagerComponent damageComponent = DamageManagerComponent.Cast(owner.FindComponent(DamageManagerComponent));
		if (!damageComponent) 
			return;
			
		// toggle damage handling 
		damageComponent.EnableDamageHandling(var.GetBool());
	}
};