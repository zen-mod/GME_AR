/**
Helper class with staic functions to enable and disable visiblity of units
*/

class ODIN_VisibilityHelper
{
	// Need different function as we have to cast to different parent objects for RPL functionality. Maybe templating could work?
	static void SetVisibilityCharacter(GenericEntity entity, bool visible)
	{
		SCR_ChimeraCharacter char = SCR_ChimeraCharacter.Cast(entity);
		// null check
		if (!char) return;
		
		char.ODIN_SetVisibility(visible);
	}
	
	static void SetVisibilityVehicle(GenericEntity entity, bool visible)
	{
		Vehicle vic = Vehicle.Cast(entity);
		// null check 
		if (!vic) return;
		
		vic.ODIN_SetVisibility(visible);
	}
	
	static bool GetVisibility(IEntity entity)
	{
		int visible = entity.GetFlags() & EntityFlags.VISIBLE;
		return visible;
	}
}
