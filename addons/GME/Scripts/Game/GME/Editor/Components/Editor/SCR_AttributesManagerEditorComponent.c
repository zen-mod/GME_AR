//! @ingroup Editor_Components

//! Manager ofeditor attributes.
//!
//! All available attributes (SCR_BaseEditorAttribute and inherited classes) are defined here, each attribute (added in the EditorMode prefab) must be an unique class inherited from SCR_BaseEditorAttribute!
//!
//! To edit attributes, follow these steps:
//! 1. Call StartEditing() to create snapshots of all attributes and open them for editing
//! 2. Call ConfirmEditing() to confirm changes or CancelEditing() to do nothing
modded class SCR_AttributesManagerEditorComponent : SCR_BaseEditorComponent
{
	//------------------------------------------------------------------------------------------------
	protected bool GME_IsEditingModule()
	{
		return GME_Modules_EditableModuleComponent.Cast(m_aEditedItems[0]);
	}
	
	//------------------------------------------------------------------------------------------------
	//! Copy of vanilla method, but implemented force-saving
	override void ConfirmEditing()
	{		
		//--- Not an owner, ignore
		if (!IsOwner())
			return;
		
		//--- Not editing, ignore
		if (!m_aEditedItems)
			return;
		
		SCR_PlayerController playerController = SCR_PlayerController.Cast(GetGame().GetPlayerController());
		int PlayerID = -1;
		
		if (playerController)
			PlayerID = playerController.GetPlayerId();
		
		bool attributeChangesApplied = false;
		
		//--- Apply changed values
		array<int> attributesIds = {};
		array<ref SCR_BaseEditorAttributeVar> attributesVars = {};
		foreach (SCR_BaseEditorAttribute attribute: m_aEditedAttributes)
		{
			SCR_BaseEditorAttributeVar var = attribute.GetVariable();
			SSnapshot snapshot = attribute.GetSnapshot();
			
			//--- Non-shared value wasn't set, skip
			if (!var)
				continue;
			
			//~ Attribute is not enabled so skip
			if (!attribute.IsEnabled())
				continue;
			
			//~ If attribute edits multiple entities and the value on the entities differ from eachother and that value is being changed overridden (even if it might be the same as some entities) then consider the value changed
			//~ Else check if attribute was changed using snapshot
			// --- GME Edit: Force writing attributes for modules
			if ((!attribute.GetIsMultiSelect() || !attribute.GetHasConflictingValues() || !attribute.GetIsOverridingValues()) && !GME_IsEditingModule())
			{
				//--- Value didn't change compared to snapshot, skip
				if (snapshot)
				{
					SSnapSerializer snapReader = SSnapSerializer.MakeReader(snapshot);
					if (SCR_BaseEditorAttributeVar.PropCompare(var, snapReader, null))
						continue;
				}
			}
			
			if (attribute.IsServer())
			{
				//--- Add to the list to be sent to server
				attributesIds.Insert(m_PrefabData.FindAttribute(attribute));
				attributesVars.Insert(var);
			}
			else
			{
				//--- Apply locally
				foreach (Managed item: m_aEditedItems)
				{
					if (attribute.ReadVariable(item, this))
						attribute.WriteVariable(item, var, this, PlayerID);
				}
			}
			
			attributeChangesApplied = true;
		}
		
		//--- Send to server
		if (!attributesIds.IsEmpty())
		{			
			//ConfirmEditingServer(attributesIds, attributesVars);
			Rpc(ConfirmEditingServer, attributesIds, attributesVars, PlayerID);
		}
		
		Event_OnAttributesConfirm.Invoke(m_aEditedAttributes);
		Clean();
		
		if (attributeChangesApplied)
		{
			Event_OnAttributeChangesApplied.Invoke();
			
			if (!m_bHasChangedAttributesOnce)
				m_bHasChangedAttributesOnce = true;
		}
	}
}
