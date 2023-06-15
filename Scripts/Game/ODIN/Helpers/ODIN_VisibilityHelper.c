/**
Helper class with staic functions to enable and disable visiblity of units
*/

class ODIN_VisibilityHelper 
{
	static void SetVisibility(IEntity entity, bool visible)
	{
		if (visible)
			entity.SetFlags(EntityFlags.VISIBLE|EntityFlags.TRACEABLE, 1);
		else 
			entity.ClearFlags(EntityFlags.VISIBLE|EntityFlags.TRACEABLE);
	}
	
	static bool GetVisibility(IEntity entity)
	{
		int visible = entity.GetFlags() & EntityFlags.VISIBLE;
		return visible;
	}
}
