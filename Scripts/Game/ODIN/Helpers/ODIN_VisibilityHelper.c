/**
Helper class with staic functions to enable and disable visiblity of units
*/

class ODIN_VisibilityHelper : Managed
{
	// Need different function as we have to cast to different parent objects for RPL functionality. Maybe templating could work?
	static void SetVisibilityCharacter(GenericEntity entity, bool visible)
	{
		SCR_ChimeraCharacter char = SCR_ChimeraCharacter.Cast(entity);
		// null check
		if (!char) return;
		
		if (visible)
			char.ODIN_SetGlobalFlag(EntityFlags.VISIBLE, true);
		else 
			char.ODIN_SetGlobalFlag(EntityFlags.VISIBLE, false);
	}
	
	static void SetVisibilityVehicle(GenericEntity entity, bool visible)
	{
		Vehicle vic = Vehicle.Cast(entity);
		// null check 
		if (!vic) return;
		
		if (visible)
			vic.ODIN_SetGlobalFlag(EntityFlags.VISIBLE, true);
		else 
			vic.ODIN_SetGlobalFlag(EntityFlags.VISIBLE, false);
	}
	
	static bool GetVisibility(IEntity entity)
	{
		int visible = entity.GetFlags() & EntityFlags.VISIBLE;
		return visible;
	}
}
