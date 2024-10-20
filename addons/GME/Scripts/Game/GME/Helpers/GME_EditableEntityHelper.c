//------------------------------------------------------------------------------------------------
class GME_EditableEntityHelper
{
	//------------------------------------------------------------------------------------------------
	static SCR_Faction GetFactionFromLabels(SCR_EditableEntityComponent entity)
	{
		SCR_EditableEntityUIInfo entityUIInfo = SCR_EditableEntityUIInfo.Cast(entity.GetInfo());

		array<Faction> factions = { };
		GetGame().GetFactionManager().GetFactionsList(factions);

		foreach (Faction faction : factions)
		{
			SCR_Faction scriptedFaction = SCR_Faction.Cast(faction);
			if (entityUIInfo.HasEntityLabel(scriptedFaction.GetFactionLabel()))
				return scriptedFaction;
		}

		return null;
	}
}