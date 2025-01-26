//------------------------------------------------------------------------------------------------

[BaseContainerProps(), SCR_BaseContainerCustomTitleUIInfo("m_Info")]
class GME_DeepPasteContextAction : SCR_SelectedEntitiesContextAction
{
	override void Perform(SCR_EditableEntityComponent hoveredEntity, notnull set<SCR_EditableEntityComponent> selectedEntities, vector cursorWorldPosition, int flags, int param = -1)
	{		
		Print("---------------------");
		Print(selectedEntities);
		Print(cursorWorldPosition);
		Print("---------------------");
		Print("");
		
		// If server is running this function, then we need to store the data in a hashmap, with entry per player? As otherwise players will overwrite eachothers paste data.... 
		//  Honestly should probably be run locally on player with RPC channel back to Authority for spawning the entities
		
		// data is in component, go through it and spawn at cursorWorldPosition? 
		
		// get component, get data, either iterate through them here, or send straight to server on RPC to let server spawn them?
		
		//foreach (auto entity : gme_vEditorDeepCopyData)
		//{
			// Spawn at cursor position?
		//	Print(entity);
		//}
		
	}
}