//------------------------------------------------------------------------------------------------
class GME_HitZoneContainerComponentSerializer : HitZoneContainerComponentSerializer
{
	//------------------------------------------------------------------------------------------------
	override protected ESerializeResult Serialize(notnull IEntity owner, notnull GenericComponent component, notnull SaveContext context)
	{
		const SCR_DamageManagerComponent damageManager = SCR_DamageManagerComponent.Cast(component);
		
        context.StartObject("base");
		const ESerializeResult baseResult = super.Serialize(owner, component, context);
        context.EndObject();
		
		if (baseResult == ESerializeResult.ERROR)
			return ESerializeResult.ERROR;
		
		const bool damageHandlingEnabled = damageManager.IsDamageHandlingEnabled();
		
		if (baseResult == ESerializeResult.DEFAULT &&
			damageHandlingEnabled)
		{
			return ESerializeResult.DEFAULT;
		}
		
		context.WriteValue("version", 1);
		context.WriteDefault(damageHandlingEnabled, true);
		return ESerializeResult.OK;
	}
	
	//------------------------------------------------------------------------------------------------
	override protected bool Deserialize(notnull IEntity owner, notnull GenericComponent component, notnull LoadContext context)
	{
		SCR_DamageManagerComponent damageManager = SCR_DamageManagerComponent.Cast(component);

		if (context.DoesObjectExist("base"))
		{
			if (!context.StartObject("base") ||
				!super.Deserialize(owner, component, context) ||
				!context.EndObject())
			{
				return false;
			}
		}
		
		int version;
		context.Read(version);
		
		bool damageHandlingEnabled;
		if (context.Read(damageHandlingEnabled))
			damageManager.EnableDamageHandling(damageHandlingEnabled);

		return true;
	}
}
