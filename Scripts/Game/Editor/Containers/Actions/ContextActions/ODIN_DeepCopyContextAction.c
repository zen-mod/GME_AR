//------------------------------------------------------------------------------------------------
[BaseContainerProps(), SCR_BaseContainerCustomTitleUIInfo("m_Info")]
class ODIN_DeepCopyContextAction : SCR_SelectedEntitiesContextAction
{
	override void Perform(SCR_EditableEntityComponent hoveredEntity, notnull set<SCR_EditableEntityComponent> selectedEntities, vector cursorWorldPosition, int flags, int param = -1)
	{		
		Print("---------------------");
		Print(selectedEntities);
		Print(cursorWorldPosition);
		Print("---------------------");
		Print("");
	}
}