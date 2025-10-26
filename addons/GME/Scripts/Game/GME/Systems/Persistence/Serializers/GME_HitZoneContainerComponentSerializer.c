//------------------------------------------------------------------------------------------------
class GME_HitZoneContainerComponentSerializer : HitZoneContainerComponentSerializer
{
	//------------------------------------------------------------------------------------------------
	override protected ESerializeResult Serialize(notnull IEntity owner, notnull GenericComponent component, notnull BaseSerializationSaveContext context)
	{
		ESerializeResult result = super.Serialize(owner, component, context);
		
		SCR_DamageManagerComponent damageManager = SCR_DamageManagerComponent.Cast(component);
		if (result == ESerializeResult.DEFAULT && damageManager.IsDamageHandlingEnabled())
			return result;
		
		context.WriteValue("version", 1);
		context.WriteValue("damageHandlingEnabled", damageManager.IsDamageHandlingEnabled());
		return ESerializeResult.OK;
	}
	
	//------------------------------------------------------------------------------------------------
	override protected bool Deserialize(notnull IEntity owner, notnull GenericComponent component, notnull BaseSerializationLoadContext context)
	{
		super.Deserialize(owner, component, context);
		
		SCR_DamageManagerComponent damageManager = SCR_DamageManagerComponent.Cast(component);
		
		int version;
		context.Read(version);

		bool damageHandlingEnabled;
		if (context.Read(damageHandlingEnabled))
			damageManager.EnableDamageHandling(false);

		return true;
	}
}
