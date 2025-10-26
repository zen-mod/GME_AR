//------------------------------------------------------------------------------------------------
modded class SCR_EditableEntityComponentSerializer : ScriptedComponentSerializer
{
	//------------------------------------------------------------------------------------------------
	override protected ESerializeResult Serialize(notnull IEntity owner, notnull GenericComponent component, notnull BaseSerializationSaveContext context)
	{
		ESerializeResult result = super.Serialize(owner, component, context);
		if (result == ESerializeResult.ERROR)
			return result;
		
		SCR_EditableEntityComponent editable = SCR_EditableEntityComponent.Cast(component);
		if (!editable.GME_IsVisible())
		{
			context.WriteValue("visible", false);
			result = ESerializeResult.OK;
		}
		
		return result;
	}
	
	//------------------------------------------------------------------------------------------------
	override protected bool Deserialize(notnull IEntity owner, notnull GenericComponent component, notnull BaseSerializationLoadContext context)
	{
		super.Deserialize(owner, component, context);
		SCR_EditableEntityComponent editable = SCR_EditableEntityComponent.Cast(component);
		
		bool visible;
		if (context.Read(visible))
			editable.GME_SetIsVisible(visible);
		
		return true;
	}
}
