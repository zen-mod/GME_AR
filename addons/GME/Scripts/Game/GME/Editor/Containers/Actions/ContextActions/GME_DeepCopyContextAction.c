//------------------------------------------------------------------------------------------------

[BaseContainerProps(), SCR_BaseContainerCustomTitleUIInfo("m_Info")]
class GME_DeepCopyContextAction : SCR_SelectedEntitiesContextAction
{
	override void Perform(SCR_EditableEntityComponent hoveredEntity, notnull set<SCR_EditableEntityComponent> selectedEntities, vector cursorWorldPosition, int flags, int param = -1)
	{		
		Print("---------------------");
		Print(selectedEntities);
		Print(cursorWorldPosition);
		Print("---------------------");
		Print("");
		
		// This function is called on client
		
		// need to look into how basegame uses SCR_EditorStruct, in terms of saving and loading entities. Or check how SCR_SaveLoadComponent does it. 
		
		//set<SCR_EditableEntityComponent> gme_vEditorDeepCopyData = selectedEntities;
		
		// find deepcopy component from player entity 
		
		// set variable in component
		
	}
}