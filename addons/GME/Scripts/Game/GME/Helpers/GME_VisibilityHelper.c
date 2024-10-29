//------------------------------------------------------------------------------------------------
//! Helper class with staic functions to enable and disable visiblity of units
class GME_VisibilityHelper
{
	//------------------------------------------------------------------------------------------------
	static void SetVisibility(GenericEntity entity, bool visible)
	{
		// casting to possible supported types, if not null we found our match and change visibility and return
		SCR_ChimeraCharacter char = SCR_ChimeraCharacter.Cast(entity);
		if (char)
		{
			char.GME_SetVisibility(visible);
			return;
		}
		
		Vehicle vic = Vehicle.Cast(entity);
		if (vic) 
		{
			vic.GME_SetVisibility(visible);
			return;
		}
	}
	
	//------------------------------------------------------------------------------------------------
	static bool GetVisibility(IEntity entity)
	{
		return entity.GetFlags() & EntityFlags.VISIBLE;
	}
}
