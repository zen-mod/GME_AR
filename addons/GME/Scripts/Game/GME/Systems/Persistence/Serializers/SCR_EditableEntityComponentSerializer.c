//------------------------------------------------------------------------------------------------
modded class SCR_EditableEntityComponentSerializer : ScriptedComponentSerializer
{
	//------------------------------------------------------------------------------------------------
	override protected ESerializeResult Serialize(notnull IEntity owner, notnull GenericComponent component, notnull SaveContext context)
	{
		const SCR_EditableEntityComponent editable = SCR_EditableEntityComponent.Cast(component);

        context.StartObject("base");
		const ESerializeResult baseResult = super.Serialize(owner, component, context);
        context.EndObject();
		
		if (baseResult == ESerializeResult.ERROR)
			return ESerializeResult.ERROR;
		
		const bool visible = editable.GME_IsVisible();
		
		if (baseResult == ESerializeResult.DEFAULT &&
			visible)
		{
			return ESerializeResult.DEFAULT;
		}
		
		context.WriteValue("version", 1);
		context.WriteDefault(visible, true);
		return ESerializeResult.OK;
	}
	
	//------------------------------------------------------------------------------------------------
	override protected bool Deserialize(notnull IEntity owner, notnull GenericComponent component, notnull LoadContext context)
	{
		SCR_EditableEntityComponent editable = SCR_EditableEntityComponent.Cast(component);
		
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
		
		bool visible;
		if (context.Read(visible))
			editable.GME_SetIsVisible(visible);
		
		return true;
	}
}
