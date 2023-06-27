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
		
			//IEntity controlledEntity = SCR_PlayerController.GetLocalControlledEntity();
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
		
		ODIN_TeleportToSquadmateDialog myMenuUI = ODIN_TeleportToSquadmateDialog.Cast(myMenu);
		myMenuUI.myCallerEntity = owner;
		// with the method above we can make functions in listbox for setting listbox data, title etc. For generic listbox item?
		
		// TODO figure out if we pass hoveredentity, or get squadmates here and populate the dialog? Can we actually make a generic dialog and populate it from here? 
		// Would have to be able to pass data to it on creation and bind callback to its "ok" method. 
		
	}
	override bool IsServer()
	{
		return false;
	}
};
