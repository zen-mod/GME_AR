[BaseContainerProps(), SCR_BaseContainerCustomTitleUIInfo("m_Info")]
class ODIN_TeleportToSquadmateContextAction: SCR_BaseContextAction
{	
	override bool CanBeShown(SCR_EditableEntityComponent hoveredEntity, notnull set<SCR_EditableEntityComponent> selectedEntities, vector cursorWorldPosition, int flags)
	{
		if (!hoveredEntity)
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
			return false;
		
		// TODO figure out if we pass hoveredentity, or get squadmates here and populate the dialog? Can we actually make a generic dialog and populate it from here? 
		// Would have to be able to pass data to it on creation and bind callback to its "ok" method. 
		
	}
	override bool IsServer()
	{
		return false;
	}
};
