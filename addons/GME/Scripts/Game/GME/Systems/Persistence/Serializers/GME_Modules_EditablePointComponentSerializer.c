//------------------------------------------------------------------------------------------------
class GME_Modules_EditablePointComponentSerializer : SCR_EditableEntityComponentSerializer
{
	//------------------------------------------------------------------------------------------------
	override static typename GetTargetType()
	{
		return GME_Modules_EditablePointComponent;
	}
	
	//------------------------------------------------------------------------------------------------
	override protected ESerializeResult Serialize(notnull IEntity owner, notnull GenericComponent component, notnull SaveContext context)
	{
		const GME_Modules_EditablePointComponent point = GME_Modules_EditablePointComponent.Cast(component);
		
        context.StartObject("base");
		const ESerializeResult baseResult = super.Serialize(owner, component, context);
        context.EndObject();
		
		if (baseResult == ESerializeResult.ERROR)
			return ESerializeResult.ERROR;
		
		const LocalizedString callsign = point.GetCallsign();
		
		context.WriteValue("version", 1);
		context.Write(callsign);
		return ESerializeResult.OK;
	}
	
	//------------------------------------------------------------------------------------------------
	override protected bool Deserialize(notnull IEntity owner, notnull GenericComponent component, notnull LoadContext context)
	{
		GME_Modules_EditablePointComponent point = GME_Modules_EditablePointComponent.Cast(component);
		
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
		
		string callsign;
		if (context.Read(callsign))
			point.SetCallsign(callsign);
		
		return true;
	}
}
