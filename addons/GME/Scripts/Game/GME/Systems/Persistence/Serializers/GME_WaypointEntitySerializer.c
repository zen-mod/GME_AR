//------------------------------------------------------------------------------------------------
class GME_WaypointEntitySerializer : GenericEntitySerializer
{
	//------------------------------------------------------------------------------------------------
	override static typename GetTargetType()
	{
		return AIWaypoint;
	}
	
	//------------------------------------------------------------------------------------------------
	override protected ESerializeResult Serialize(notnull IEntity entity, notnull BaseSerializationSaveContext context)
	{
		ESerializeResult result = super.Serialize(entity, context);
		if (result == ESerializeResult.ERROR)
			return result;
		
		AIWaypoint waypoint = AIWaypoint.Cast(entity);
		
		BaseContainer container = waypoint.GetPrefabData().GetPrefab();
		if (!container)
			return ESerializeResult.ERROR;
		
		float defaultRadius;
		container.Get("CompletionRadius", defaultRadius);
		EAIWaypointCompletionType defaultType;
		container.Get("CompletionType", defaultType);
		
		if (defaultRadius != waypoint.GetCompletionRadius())
		{
			context.WriteValue("completionRadius", waypoint.GetCompletionRadius());
			result = ESerializeResult.OK;
		}
		
		if (defaultType != waypoint.GetCompletionType())
		{
			context.WriteValue("completionType", waypoint.GetCompletionType());
			result = ESerializeResult.OK;
		}
		
		return result;
	}
	
	//------------------------------------------------------------------------------------------------
	override protected bool Deserialize(notnull IEntity entity, notnull BaseSerializationLoadContext context)
	{
		super.Deserialize(entity, context);
		AIWaypoint waypoint = AIWaypoint.Cast(entity);
		
		float completionRadius;
		if (context.Read(completionRadius))
			waypoint.SetCompletionRadius(completionRadius);
		
		EAIWaypointCompletionType completionType;
		if (context.Read(completionType))
			waypoint.SetCompletionType(completionType);
		
		return true;
	}
}
