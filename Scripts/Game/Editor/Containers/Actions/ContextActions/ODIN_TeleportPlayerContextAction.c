[BaseContainerProps(), SCR_BaseContainerCustomTitleUIInfo("m_Info")]
class ODIN_TeleportPlayerContextAction : SCR_SelectedEntitiesContextAction
{
	override bool CanBeShown(SCR_EditableEntityComponent selectedEntity, vector cursorWorldPosition, int flags)
	{
		return CanBePerformed(selectedEntity, cursorWorldPosition, flags);
	}
	override bool CanBePerformed(SCR_EditableEntityComponent selectedEntity, vector cursorWorldPosition, int flags)
	{
		if (selectedEntity == null || selectedEntity.GetEntityType() != EEditableEntityType.CHARACTER)
			return false;
		return true;
	}
	
	override void Perform(SCR_EditableEntityComponent selectedEntity, vector cursorWorldPosition)
	{		
		// TODO do the code that add drawhandler and use the mouse to select destination position for TP 
		
		
		// teleporting part .....
		
		//--- Apply transformation
		vector transform[4];
		selectedEntity.GetTransform(transform);
		transform[3] = cursorWorldPosition;

		selectedEntity.SetTransform(transform);
		
	}
};