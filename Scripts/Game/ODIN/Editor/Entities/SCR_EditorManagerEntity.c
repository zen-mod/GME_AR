/** @ingroup Editor_Entities
*/

/*!
The entity which enables all editor functionality for the player.

- It's **created by SCR_EditorManagerCore** when a player connects.
 + Every player receives one.
- In multiplayer, the entity is **local to the player**.
 + This is required to allow communication with server (see Replication for more details).
 + Requires RplComonent to function.
- This entity handles **only barebone editor functionality**, like access rules, or opening and closing.
- Everything else, like camera or menu handling, must be on specialized editor modes (SCR_EditorModeEntity) and components (SCR_BaseEditorComponent)!
- Editor can have a limitied state, influenced by its modes and checked by IsLimited() function:
 + Editor is **limited** if all its modes are.
 + If at least one mode is unlimited, the editor is **unlimited** as well.
 + The state influences behavior of certain components, e.g., photo mode camera movement may be restricted if the editor is limited.
 + As a rule of thumb, unlimited state means that player can ***cheat*** with the editor.
- Default editor manager prefab is defined in SCR_EditorManagerCore (config is Configs/Core folder).
*/
modded class SCR_EditorManagerEntity : SCR_EditorBaseEntity
{
	static int emptyTerrainAreaRadius = 20;
	
	/*!
	Functions to tell the server to move an entity
	*/
	void ODIN_TeleportEntityToPlayer(RplId sourceEntityId, RplId targetEntityId)
	{
		Rpc(ODIN_Server_TeleportEntityToPlayer, sourceEntityId, targetEntityId);
	}
	
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	protected void ODIN_Server_TeleportEntityToPlayer(RplId entityId, RplId targetEntityId)
	{		
		// get entity from id 
		RplComponent entityRpl = RplComponent.Cast(Replication.FindItem(entityId));
		RplComponent targetRpl = RplComponent.Cast(Replication.FindItem(targetEntityId));
		
		if (!entityRpl || !targetRpl)
			return;
		
		IEntity sourceEntity = entityRpl.GetEntity();
		IEntity targetEntity = targetRpl.GetEntity();
		
		if (!sourceEntity || !targetEntity)
			return;
		
		// If target is sitting in a vehicle, move player inside as well (Taken from SCR_PlayerSpawnPoint.c)
		SCR_CompartmentAccessComponent compartmentAccessTarget = SCR_CompartmentAccessComponent.Cast(targetEntity.FindComponent(SCR_CompartmentAccessComponent));
		IEntity vehicle = compartmentAccessTarget.GetVehicle();
		if (vehicle)
		{
			SCR_CompartmentAccessComponent compartmentAccessPlayer = SCR_CompartmentAccessComponent.Cast(sourceEntity.FindComponent(SCR_CompartmentAccessComponent));
			
			// if we succeed, we return to avoid teleporting the unit after having moved him into the vehicle. If we fail, we just continue and TP player next to vehicle
			if (compartmentAccessPlayer.MoveInVehicleAny(vehicle))
				return;
		}
		
		// Get world coordinates of player 
		vector target_pos;
		SCR_WorldTools.FindEmptyTerrainPosition(target_pos, targetEntity.GetOrigin(), emptyTerrainAreaRadius);
		
		// get transform for rotation etc. 
		vector target_transform[4];
		sourceEntity.GetTransform(target_transform);
		
		// use new position, but keep rotation 
		target_transform[3] = target_pos;
		
		// Get editable component 
		SCR_EditableEntityComponent editableEntity = SCR_EditableEntityComponent.Cast(sourceEntity.FindComponent(SCR_EditableEntityComponent));
		
		// call transform
		if (editableEntity)
			editableEntity.SetTransform(target_transform, true);
	}
};