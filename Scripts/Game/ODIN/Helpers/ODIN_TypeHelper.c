/**
Helper class with static functions for helping type match
*/
class ODIN_TypeHelper 
{
	static string VehicleTypeName = "Vehicle";
	static string CharacterTypeName = "SCR_ChimeraCharacter";
	
	static bool IsUnit(IEntity entity)
	{
		string type = entity.ClassName();
		if (type.Contains(ODIN_TypeHelper.CharacterTypeName))
			return true;
		else 
			return false;
	}
	
	static bool IsVehicle(IEntity entity)
	{
		string type = entity.ClassName();
		if (type.Contains(ODIN_TypeHelper.VehicleTypeName))
			return true;
		else 
			return false;
	}
}