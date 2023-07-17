//------------------------------------------------------------------------------------------------
[BaseContainerProps(), SCR_BaseContainerCustomTitleUIInfo("m_Info")]
class ODIN_TeleportToSquadmateContextAction: SCR_BaseContextAction
{	
	static int emptyTerrainAreaRadius = 20;
	
	override bool CanBeShown(SCR_EditableEntityComponent hoveredEntity, notnull set<SCR_EditableEntityComponent> selectedEntities, vector cursorWorldPosition, int flags)
	{
		if (!hoveredEntity)
			return false;
		
		GenericEntity owner = hoveredEntity.GetOwner();
		if (!SCR_ChimeraCharacter.Cast(owner))
			return false;
		
		// only able to do on players
		if (!EntityUtils.IsPlayer(owner))
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
			ODIN_LogHelper.Log("Could not find listbox dialog defined", "TeleportToSquadmateContextAction", LogLevel.WARNING);
			return;	
		}
		
		// cast so we have dialog object. Set hovered-entity, we need to get squadmates from it and have it to teleport it
		ODIN_ListboxSingleSelectDialog myMenuUI = ODIN_ListboxSingleSelectDialog.Cast(myMenu);
		myMenuUI.SetHoveredEntity(hoveredEntity);
		
		// set Title 
		myMenuUI.SetTitle("#ODIN-ActionList_TeleportToSquadmate_Name");
			
		// Get squadmates 
		SCR_AIGroup group = ODIN_GroupHelper.GetGroup(owner);
		if (!group || group.GetPlayerCount() <= 0) 
		{
			// give error message back to zeus that unit is not in a group 
			ODIN_LogHelper.Log("null group or <= 0 in group", "TeleportToSquadmateContextAction", LogLevel.WARNING);
			return;
		}
		
		PlayerManager playerManager = GetGame().GetPlayerManager();
		// can't get character entities directly from group, only for AI. So for players we get their IDs and gotta find and cast them with that
		array<int> members = group.GetPlayerIDs();
		foreach (int playerID : members)
		{
			string name = playerManager.GetPlayerName(playerID);
			ChimeraCharacter player = ChimeraCharacter.Cast(playerManager.GetPlayerControlledEntity(playerID));
			
			// if player is dead and the object is null, we skip
			if (!player)
				continue;
			
			// don't add the unit we want to teleport
			if (player == owner)
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
	}
	
	// function for script invoker
	static void ODIN_OnConfirm(SCR_EditableEntityComponent hoveredEntity, SCR_ListBoxComponent listbox)
	{
		// get selected player 
		int selectedItem = listbox.GetSelectedItem();
		if (selectedItem == -1)
		{
			ODIN_LogHelper.Log("Listbox returned -1 as selection, seemed like no option was selected", "TeleportToSquadmateContextAction", LogLevel.WARNING);
			return;
		}
		
		ChimeraCharacter selectedPlayer = ChimeraCharacter.Cast(listbox.GetItemData(selectedItem));
		if (!selectedPlayer)
		{
			ODIN_LogHelper.Log("ChimeraCharacter in userdata for selected listbox item is null", "TeleportToSquadmateContextAction", LogLevel.WARNING);
			return;
		}
	
		// If target is sitting in a vehicle, move player inside as well (Taken from SCR_PlayerSpawnPoint.c)
		SCR_CompartmentAccessComponent compartmentAccessTarget = SCR_CompartmentAccessComponent.Cast(selectedPlayer.FindComponent(SCR_CompartmentAccessComponent));
		IEntity vehicle = compartmentAccessTarget.GetVehicle();
		if (vehicle)
		{
			SCR_CompartmentAccessComponent compartmentAccessPlayer = SCR_CompartmentAccessComponent.Cast(hoveredEntity.GetOwner().FindComponent(SCR_CompartmentAccessComponent));
			
			// only return here if we were succesfull in moving into vehicle. If false its likely vehicle is full; Then we continue function to move unit right next to vehicle
			if (compartmentAccessPlayer.MoveInVehicleAny(vehicle))
				return;
		}
		
		// Get world coordinates of player 
		vector target_pos;
		SCR_WorldTools.FindEmptyTerrainPosition(target_pos, selectedPlayer.GetOrigin(), emptyTerrainAreaRadius);
		
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
