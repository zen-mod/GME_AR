//------------------------------------------------------------------------------------------------
class GME_WaypointEntitySerializer : GenericEntitySerializer
{
	//------------------------------------------------------------------------------------------------
	override static typename GetTargetType()
	{
		return AIWaypoint;
	}
	
	//------------------------------------------------------------------------------------------------
	override protected ESerializeResult Serialize(notnull IEntity entity, notnull SaveContext context)
	{
		const AIWaypoint waypoint = AIWaypoint.Cast(entity);
		
        context.StartObject("base");
        const ESerializeResult baseResult = super.Serialize(entity, context);
        context.EndObject();
		
		if (baseResult == ESerializeResult.ERROR)
			return ESerializeResult.ERROR;
				
		BaseContainer container = waypoint.GetPrefabData().GetPrefab();
		if (!container)
			return ESerializeResult.ERROR;
		
		const float completionRadius = waypoint.GetCompletionRadius();
		const EAIWaypointCompletionType completionType = waypoint.GetCompletionType();
		
		float defaultCompletionRadius;
		container.Get("CompletionRadius", defaultCompletionRadius);
		EAIWaypointCompletionType defaultCompletionType;
		container.Get("CompletionType", defaultCompletionType);
		
		if (baseResult == ESerializeResult.DEFAULT &&
			defaultCompletionRadius == completionRadius &&
			defaultCompletionType == completionType)
		{
			return ESerializeResult.DEFAULT;
		}
		
		context.WriteValue("version", 1);
		context.WriteDefault(completionRadius, defaultCompletionRadius);
		context.WriteDefault(completionType, defaultCompletionType);
		return ESerializeResult.OK;
	}
	
	//------------------------------------------------------------------------------------------------
	override protected bool Deserialize(notnull IEntity entity, notnull LoadContext context)
	{
		AIWaypoint waypoint = AIWaypoint.Cast(entity);
		
		if (context.DoesObjectExist("base"))
		{
			if (!context.StartObject("base") ||
				!super.Deserialize(entity, context) ||
				!context.EndObject())
			{
				return false;
			}
		}
		
		int version;
		context.Read(version);
		
		float completionRadius;
		if (context.Read(completionRadius))
			waypoint.SetCompletionRadius(completionRadius);
		
		EAIWaypointCompletionType completionType;
		if (context.Read(completionType))
			waypoint.SetCompletionType(completionType);
		
		return true;
	}
}
