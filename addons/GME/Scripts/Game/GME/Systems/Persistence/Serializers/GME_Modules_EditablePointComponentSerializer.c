//------------------------------------------------------------------------------------------------
class GME_Modules_EditablePointComponentSerializer : ScriptedComponentSerializer
{
	//------------------------------------------------------------------------------------------------
	override static typename GetTargetType()
	{
		return GME_Modules_EditablePointComponent;
	}
	
	//------------------------------------------------------------------------------------------------
	override protected ESerializeResult Serialize(notnull IEntity owner, notnull GenericComponent component, notnull SaveContext context)
	{

		GME_Modules_EditablePointComponent point = GME_Modules_EditablePointComponent.Cast(component);
		context.WriteValue("version", 1);
		context.WriteValue("callsign", point.GetCallsign());
		return ESerializeResult.OK;
	}
	
	//------------------------------------------------------------------------------------------------
	override protected bool Deserialize(notnull IEntity owner, notnull GenericComponent component, notnull LoadContext context)
	{
		GME_Modules_EditablePointComponent point = GME_Modules_EditablePointComponent.Cast(component);
		int version;
		context.Read(version);
		string callsign;
		context.Read(callsign);
		point.SetCallsign(callsign);
		return true;
	}
}
