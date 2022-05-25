[BaseContainerProps(), SCR_BaseContainerCustomTitleUIInfo("m_info")]
class CROW_DeleteAllDeadContextAction : SCR_BaseContextAction
{
	override bool CanBeShown(SCR_EditableEntityComponent hoveredEntity, notnull set<SCR_EditableEntityComponent> selectedEntities, vector cursorWorldPosition, int flags)
	{
		return CanBePerformed(hoveredEntity, selectedEntities, cursorWorldPosition, flags);
	}
	override bool CanBePerformed(SCR_EditableEntityComponent hoveredEntity, notnull set<SCR_EditableEntityComponent> selectedEntities, vector cursorWorldPosition, int flags)
	{
		if (hoveredEntity)
		{
			vector pos;
			return hoveredEntity.GetPos(pos);
		}
		else
		{
			return cursorWorldPosition != vector.Zero;
		}
	}
	
	override void Perform(SCR_EditableEntityComponent hoveredEntity, notnull set<SCR_EditableEntityComponent> selectedEntities, vector cursorWorldPosition,int flags, int param = -1)
	{
		// get all dead units 
		SCR_EntitiesManagerEditorComponent entitiesManager = SCR_EntitiesManagerEditorComponent.Cast(SCR_EntitiesManagerEditorComponent.GetInstance(SCR_EntitiesManagerEditorComponent, true));
		if (entitiesManager)
		{
			// get all entities from the manager with filter destroyed
			SCR_DestroyedEditableEntityFilter deadManager = SCR_DestroyedEditableEntityFilter.Cast(entitiesManager.GetFilter(EEditableEntityState.DESTROYED));

			set<SCR_EditableEntityComponent> deadEntities = new set<SCR_EditableEntityComponent>();
			deadManager.GetEntities(deadEntities);
			
			// DEBUG
			//PrintFormat("Found %1 dead entities", deadEntities.Count());
						
			// delete them
			foreach (SCR_EditableEntityComponent entity : deadEntities)
			{
				entity.Delete();
			}
		}
	}
};