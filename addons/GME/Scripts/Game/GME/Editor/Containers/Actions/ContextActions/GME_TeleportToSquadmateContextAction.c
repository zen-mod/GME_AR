//------------------------------------------------------------------------------------------------
[BaseContainerProps(), SCR_BaseContainerCustomTitleUIInfo("m_Info")]
class GME_TeleportToSquadmateContextAction: SCR_BaseContextAction
{		
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
		
		MenuBase myMenu = GetGame().GetMenuManager().OpenDialog(ChimeraMenuPreset.GME_Listbox_SingleSelect_ID, DialogPriority.INFORMATIVE, 0, true); 
		
		// null check 
		if (!myMenu)
		{
			GME_LogHelper.Log("Could not find listbox dialog defined", "TeleportToSquadmateContextAction", LogLevel.WARNING);
			return;	
		}
		
		// cast so we have dialog object. Set hovered-entity, we need to get squadmates from it and have it to teleport it
		GME_ListboxSingleSelectDialog myMenuUI = GME_ListboxSingleSelectDialog.Cast(myMenu);
		myMenuUI.SetHoveredEntity(hoveredEntity);
		
		// set Title 
		myMenuUI.SetTitle("#GME-ActionList_TeleportToSquadmate_Name");
			
		// Get squadmates 
		SCR_AIGroup group = GME_GroupHelper.GetGroup(SCR_ChimeraCharacter.Cast(owner));
		if (!group || group.GetPlayerCount() <= 0) 
		{
			// give error message back to zeus that unit is not in a group 
			GME_LogHelper.Log("null group or <= 0 in group", "TeleportToSquadmateContextAction", LogLevel.WARNING);
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
		myMenuUI.GetOnConfirm().Insert(GME_TeleportToSquadmateContextAction.GME_OnConfirm);
	}
	
	// function for script invoker
	static void GME_OnConfirm(SCR_EditableEntityComponent hoveredEntity, SCR_ListBoxComponent listbox)
	{
		// get selected player 
		int selectedItem = listbox.GetSelectedItem();
		if (selectedItem == -1)
		{
			GME_LogHelper.Log("Listbox returned -1 as selection, seemed like no option was selected", "TeleportToSquadmateContextAction", LogLevel.WARNING);
			return;
		}
		
		ChimeraCharacter selectedPlayer = ChimeraCharacter.Cast(listbox.GetItemData(selectedItem));
		if (!selectedPlayer)
		{
			GME_LogHelper.Log("ChimeraCharacter in userdata for selected listbox item is null", "TeleportToSquadmateContextAction", LogLevel.WARNING);
			return;
		}
		
		// get target player RplId
		RplComponent targetRpl = RplComponent.Cast(selectedPlayer.FindComponent(RplComponent));
		if (!targetRpl)
			return;
					
		// Call RPC through EditorManagerEntity to reach server 
		SCR_EditorManagerEntity managerEntity = SCR_EditorManagerEntity.GetInstance();
		managerEntity.GME_TeleportEntityToPlayer(hoveredEntity.GetOwnerRplId(), targetRpl.Id());
	}
	
	override bool IsServer()
	{
		return false;
	}
};
