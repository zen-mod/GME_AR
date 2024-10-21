
class GME_GroupHelper
{
	static SCR_AIGroup GetGroup(GenericEntity entity)
	{
		PlayerManager playerManager = GetGame().GetPlayerManager();
		
		if (!playerManager)
			return null;
		
		SCR_GroupsManagerComponent groupManager = SCR_GroupsManagerComponent.Cast(GetGame().GetGameMode().FindComponent(SCR_GroupsManagerComponent));
		
		if (!groupManager)
			return null;

		int playerId = playerManager.GetPlayerIdFromControlledEntity(entity);
		
		return groupManager.GetPlayerGroup(playerId);
	};
}