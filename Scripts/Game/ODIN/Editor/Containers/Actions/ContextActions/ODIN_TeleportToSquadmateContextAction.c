//------------------------------------------------------------------------------------------------
[BaseContainerProps(), SCR_BaseContainerCustomTitleUIInfo("m_Info")]
class ODIN_TeleportToSquadmateContextAction: SCR_BaseContextAction
{	
	override bool CanBeShown(SCR_EditableEntityComponent hoveredEntity, notnull set<SCR_EditableEntityComponent> selectedEntities, vector cursorWorldPosition, int flags)
	{
		if (!hoveredEntity)
			return false;
		
		GenericEntity owner = hoveredEntity.GetOwner();
		if (!owner) 
			return false;
		
		if (!SCR_ChimeraCharacter.Cast(owner))
			return false;
				
		// TODO only able to do on players
		if (!SCR_EntityHelper.IsAPlayer(owner))
			return false;
		
		return true;
	}
	
	override bool CanBePerformed(SCR_EditableEntityComponent hoveredEntity, notnull set<SCR_EditableEntityComponent> selectedEntities, vector cursorWorldPosition, int flags)
	{
		return CanBeShown(hoveredEntity, selectedEntities, cursorWorldPosition, flags);
	}
	
	override void Perform(SCR_EditableEntityComponent hoveredEntity, notnull set<SCR_EditableEntityComponent> selectedEntities, vector cursorWorldPosition,int flags, int param = -1)
	{
		if (!hoveredEntity)
			return;
		
		GenericEntity owner = hoveredEntity.GetOwner();
		
		MenuBase myMenu = GetGame().GetMenuManager().OpenDialog(ChimeraMenuPreset.ODIN_ListBox_ID, DialogPriority.INFORMATIVE, 0, true); 
		
		// null check 
		if (!myMenu)
		{
			Print("ERROR in TeleportToSquadmateContextAction: Could not find dialog defined");
			return;	
		}
		
		// cast so we have dialog object. Set hovered entity, we need to get squadmates from it and have it to teleport it
		ODIN_TeleportToSquadmateDialog myMenuUI = ODIN_TeleportToSquadmateDialog.Cast(myMenu);
		myMenuUI.SetHoveredEntity(hoveredEntity);
		
		// set Title 
		myMenuUI.SetTitle("Teleport To Squadmate");
			
		// Get squadmates 
		SCR_AIGroup group = ODIN_GroupHelper.GetGroup(owner);
		if (!group || group.GetPlayerCount() <= 0) 
			return; // TODO give error message back to zeus that unit is not in a group 
		
		// can't get character entities directly from group, only for AI. So for players we get their IDs and gotta find and cast them with that
		array<int> members = group.GetPlayerIDs();
		foreach (int playerID : members)
		{
			PlayerManager playerManager = GetGame().GetPlayerManager();
			string name = playerManager.GetPlayerName(playerID);
			ChimeraCharacter player = ChimeraCharacter.Cast(playerManager.GetPlayerControlledEntity(playerID));
			
			// don't add the unit we want to teleport
			if (player.GetID() == owner.GetID())
				continue;
			
			// if Leader, show it
			if (group.IsPlayerLeader(playerID))
				name += " (Leader)";
			
			// if in Vehicle, show it
			if (player.IsInVehicle())
				name += " [VEHICLE]";
			
			myMenuUI.AddItemToListbox(name, player);
		}
		
		// add action to confirm 
		myMenuUI.GetOnConfirm().Insert(ODIN_TeleportToSquadmateContextAction.ODIN_OnConfirm);
		
		// TODO remember to move all strings to stringtable!
		
		// TODO figure out if we pass hoveredentity, or get squadmates here and populate the dialog? Can we actually make a generic dialog and populate it from here? 
		// Would have to be able to pass data to it on creation and bind callback to its "ok" method. 
		
		//SCR_PlayersManagerEditorComponent playersManager = SCR_PlayersManagerEditorComponent.Cast(SCR_PlayersManagerEditorComponent.GetInstance(SCR_PlayersManagerEditorComponent));
		//playersManager.TeleportPlayerToPosition(cursorWorldPosition);
	}
	
	// function for script invoker
	static void ODIN_OnConfirm(SCR_EditableEntityComponent hoveredEntity, SCR_ListBoxComponent listbox)
	{
		// todo, get selected player 
		int selectedItem = listbox.GetSelectedItem();
		ChimeraCharacter selectedPlayer = ChimeraCharacter.Cast(listbox.GetItemData(selectedItem));

		if (selectedPlayer.IsInVehicle())
		{
			// try to put hoveredentity inside vehicle? otherwise keep going and put it next to vic 
		}
		
		// Get world coordinates of player 
		vector target_pos;
		SCR_WorldTools.FindEmptyTerrainPosition(target_pos, selectedPlayer.GetOrigin(), 20);
		
		// get transform for rotation etc. 
		vector target_transform[4];
		hoveredEntity.GetTransform(target_transform);
		
		// use new position, but keep rotation 
		target_transform[3] = target_pos;
		
		hoveredEntity.SetTransform(target_transform, true);
	}
	
	override bool IsServer()
	{
		return true;
	}
};
