//------------------------------------------------------------------------------------------------
class GME_IntelComponentSerializer : ScriptedComponentSerializer
{
	//------------------------------------------------------------------------------------------------
	override static typename GetTargetType()
	{
		return GME_IntelComponent;
	}
	
	//------------------------------------------------------------------------------------------------
	override protected ESerializeResult Serialize(notnull IEntity owner, notnull GenericComponent component, notnull SaveContext context)
	{

		GME_IntelComponent intel = GME_IntelComponent.Cast(component);
		if (!intel.HasIntel())
			return ESerializeResult.DEFAULT;
		
		context.WriteValue("version", 1);
		context.WriteValue("title", intel.GetTitle());
		context.WriteValue("content", intel.GetContent());
		return ESerializeResult.OK;
	}
	
	//------------------------------------------------------------------------------------------------
	override protected bool Deserialize(notnull IEntity owner, notnull GenericComponent component, notnull LoadContext context)
	{
		GME_IntelComponent intel = GME_IntelComponent.Cast(component);
		int version;
		context.Read(version);
		string title;
		context.Read(title);
		intel.SetTitle(title);
		string content;
		context.Read(content);
		intel.SetContent(content);
		return true;
	}
}
