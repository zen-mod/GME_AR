//------------------------------------------------------------------------------------------------
class GME_HitZoneContainerComponentSerializer : HitZoneContainerComponentSerializer
{
	//------------------------------------------------------------------------------------------------
	override protected ESerializeResult Serialize(notnull IEntity owner, notnull GenericComponent component, notnull BaseSerializationSaveContext context)
	{
		ESerializeResult result = super.Serialize(owner, component, context);
		if (result == ESerializeResult.ERROR)
			return result;
		
		SCR_DamageManagerComponent damageManager = SCR_DamageManagerComponent.Cast(component);
		if (!damageManager.IsDamageHandlingEnabled())
		{
			context.WriteValue("damageHandlingEnabled", false);
			result = ESerializeResult.OK;
		}
		
		return result;
	}
	
	//------------------------------------------------------------------------------------------------
	override protected bool Deserialize(notnull IEntity owner, notnull GenericComponent component, notnull BaseSerializationLoadContext context)
	{
		super.Deserialize(owner, component, context);
		SCR_DamageManagerComponent damageManager = SCR_DamageManagerComponent.Cast(component);
		
		bool damageHandlingEnabled;
		if (context.Read(damageHandlingEnabled))
			damageManager.EnableDamageHandling(damageHandlingEnabled);

		return true;
	}
}
