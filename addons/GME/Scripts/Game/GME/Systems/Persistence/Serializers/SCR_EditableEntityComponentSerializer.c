//------------------------------------------------------------------------------------------------
modded class SCR_EditableEntityComponentSerializer : ScriptedComponentSerializer
{
	//------------------------------------------------------------------------------------------------
	override protected ESerializeResult Serialize(notnull IEntity owner, notnull GenericComponent component, notnull BaseSerializationSaveContext context)
	{
		ESerializeResult result = super.Serialize(owner, component, context);
		
		SCR_EditableEntityComponent editable = SCR_EditableEntityComponent.Cast(component);
		if (result == ESerializeResult.DEFAULT && editable.GME_IsVisible())
			return result;
		
		context.WriteValue("version", 1);
		context.WriteValue("visible", editable.GME_IsVisible());
		return ESerializeResult.OK;
	}
	
	//------------------------------------------------------------------------------------------------
	override protected bool Deserialize(notnull IEntity owner, notnull GenericComponent component, notnull BaseSerializationLoadContext context)
	{
		if (!super.Deserialize(owner, component, context))
			return false;
		
		SCR_EditableEntityComponent editable = SCR_EditableEntityComponent.Cast(component);
		int version;
		context.Read(version);
		bool visible;
		context.Read(visible);
		editable.GME_SetIsVisible(visible);
		return true;
	}
}
