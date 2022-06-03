modded class SCR_AttributesManagerEditorComponent: SCR_BaseEditorComponent
{		
	[RplRpc(RplChannel.Reliable, RplRcver.Owner)]
	override protected void StartEditingOwner(notnull array<int> attributesIds, notnull array<ref SCR_BaseEditorAttributeVar> attributesVars, notnull array<ref EEditorAttributeMultiSelect> attributesMultiSelect)
	{
		//--- Current menu changed while waiting, ignore
		if (m_StatesManager && !m_StatesManager.SetIsWaiting(false))
		{
			Clean();
			return;
		}
		
		//Print("Receiving attributes:");
		//attributesVars.Debug();
			
		//--- Get attribute values from client (add them to those received from the server)
		GetVariables(false, m_aEditedItems, attributesIds, attributesVars, attributesMultiSelect);
		
		//--- Process all edited attributes
		m_aEditedAttributes = new array<SCR_BaseEditorAttribute>;
		foreach (int i, int attributeId: attributesIds)
		{
			//-- Register to an array that is sent to the invoker
			SCR_BaseEditorAttribute attribute = m_PrefabData.GetAttribute(attributeId);
			SCR_BaseEditorAttributeVar var = attributesVars[i];
			EEditorAttributeMultiSelect attributeMultiSelect = attributesMultiSelect[i];
			
			if (GetIsAttributeDuplicate(attribute.Type()))
			{
				Print(string.Format("Trying to add editor attribute of type '%1' but there is already an attribute of the same type", attribute.Type()), LogLevel.ERROR);
				continue;
			}
			
			attribute.SetIsMultiSelect(attributeMultiSelect & EEditorAttributeMultiSelect.MULTI_SELECT);
			attribute.SetHasConflictingValues(attributeMultiSelect & EEditorAttributeMultiSelect.COLLIDING_ATTRIBUTE);
			
			m_aEditedAttributes.Insert(attribute);
			
			//--- Create snapshot for later comparison
			SSnapshot snapshot = null;
			if (var)
			{
				snapshot = new SSnapshot(SNAPSHOT_SIZE);
				SSnapSerializer snapWriter = SSnapSerializer.MakeWriter(snapshot);
				var.ModdedExtract(snapWriter);
			}
			attribute.StartEditing(var, snapshot);
		}
		
		//--- Open menu
		GetGame().GetMenuManager().OpenDialog(m_MenuPreset);
		
		//--- Invoke an event for other systems
		Event_OnAttributesStart.Invoke(m_aEditedAttributes);
		
		foreach (SCR_BaseEditorAttribute attribute: m_aEditedAttributes)
		{
			attribute.UpdateInterlinkedVariables(attribute.GetVariable(), this, true);
		}
	}
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	//--- Confirm
	/*!
	Confirm changes made during editing.
	Must be called on editor owner.
	*/
	override void ConfirmEditing()
	{		
		//--- Not an owner, ignore
		if (!IsOwner()) return;
		
		//--- Not editing, ignore
		if (!m_aEditedItems) return;
		
		SCR_PlayerController playerController = SCR_PlayerController.Cast(GetGame().GetPlayerController());
		int PlayerID = -1;
		
		if (playerController)
			PlayerID = playerController.GetPlayerId();
		
		bool attributeChangesApplied = false;
		
		//--- Apply changed values
		array<int> attributesIds = new array<int>;
		array<ref SCR_BaseEditorAttributeVar> attributesVars = new array<ref SCR_BaseEditorAttributeVar>;
		foreach (SCR_BaseEditorAttribute attribute: m_aEditedAttributes)
		{
			SCR_BaseEditorAttributeVar var = attribute.GetVariable();
			SSnapshot snapshot = attribute.GetSnapshot();
			
			//--- Non-shared value wasn't set, skip
			if (!var) continue;
			
			//~ Attribute is not enabled so skip
			if (!attribute.IsEnabled()) continue;
			
			//~ If attribute edits multiple entities and the value on the entities differ from eachother and that value is being changed overriden (even if it might be the same as some entities) then consider the value changed
			//~ Else check if attribute was changed using snapshot
			if (!attribute.GetIsMultiSelect() || !attribute.GetHasConflictingValues() || !attribute.GetIsOverridingValues())
			{
				//--- Value didn't change compared to snapshot, skip
				if (snapshot)
				{
					SSnapSerializer snapReader = SSnapSerializer.MakeReader(snapshot);
					if (var.ModdedPropCompare(snapReader))
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
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	//--- Reset
	/*!
	Reset values of currently edited attributes to the state they had when the editing started.
	Must be called on editor owner.
	\param telegraphChange True to update GUI when attribute value is changed
	*/
	override void Reset(bool telegraphChange)
	{
		//--- Not editing, ignore
		if (!m_aEditedItems) return;
				
		//--- Restore original values
		foreach (SCR_BaseEditorAttribute attribute: m_aEditedAttributes)
		{
			SCR_BaseEditorAttributeVar var = attribute.GetVariable();
			SSnapshot snapshot = attribute.GetSnapshot();
			
			if (var && snapshot)
			{
				SSnapSerializer snapReader = SSnapSerializer.MakeReader(snapshot);
				if (!var.ModdedPropCompare(snapReader))
				{
					//--- Create new reader with index back at 0
					snapReader = SSnapSerializer.MakeReader(snapshot);
					var.ModdedInject(snapReader);
					
					//--- Update GUI
					if (telegraphChange)
						attribute.TelegraphChange(true);
					
					foreach (Managed item: m_aEditedItems)
					{
						if (attribute.ReadVariable(item, this))
						{
							attribute.PreviewVariable(false, this);
							attribute.UpdateInterlinkedVariables(var, this);
						}
							
					}
				}
				//Still reset if value is conflicting or if is override var and value did not change
				else if (attribute.GetHasConflictingValues())
				{
					if (telegraphChange)
						attribute.TelegraphChange(true);
				}
			}
			//If conflicting attribute then it will still set UI as default value and update copyVar
			else if (!var)
			{
				if (telegraphChange)
					attribute.TelegraphChange(true);
			}
		}
		
		Event_OnResetAttributes.Invoke();
	}
	
	override protected int GetVariables(bool onlyServer, notnull array<Managed> items, notnull out array<int> outIds, notnull out array<ref SCR_BaseEditorAttributeVar> outVars,  notnull out array<ref EEditorAttributeMultiSelect> outAttributesMultiSelect)
	{		
		SCR_BaseEditorAttribute attribute;
		for (int i = 0, count = m_PrefabData.GetAttributesCount(); i < count; i++)
		{
			attribute = m_PrefabData.GetAttribute(i);
			if (!attribute) continue;
			if (attribute.IsServer() != onlyServer) continue;
			
			SCR_BaseEditorAttributeVar var = null;
			bool isCompatible = false;
			
			SSnapshot snapshot = new SSnapshot(SNAPSHOT_SIZE);
			SSnapSerializer snapWriter;
			SSnapSerializer snapReader;
			
			EEditorAttributeMultiSelect multiSelectState;
			
			foreach (Managed item: items)
			{				
				if (!item) continue;
				
				SCR_BaseEditorAttributeVar checkVar = attribute.ReadVariable(item, this);
				if (!checkVar) continue;
				
				var = checkVar;
				isCompatible = true;
				
				if (!snapReader)
				{
					//--- First item, write snapshot for comparison
					snapWriter = SSnapSerializer.MakeWriter(snapshot);
					snapReader = SSnapSerializer.MakeReader(snapshot);
					var.ModdedExtract(snapWriter);
				}
				else
				{
					//Set is multi select
					multiSelectState |= EEditorAttributeMultiSelect.MULTI_SELECT;
					
					//--- Reset the reader so the comparison starts from the beginning
					snapReader.Seek(0);
					
					if (!var.ModdedPropCompare(snapReader))
					{
						//--- When item don't share the same variable, don't check other items
						multiSelectState |= EEditorAttributeMultiSelect.COLLIDING_ATTRIBUTE;
						break;
					}
				}
			}
		
			//--- Use the attribute when at least one item has it
			if (isCompatible)
			{				
				outIds.Insert(i);
				outVars.Insert(var);
				outAttributesMultiSelect.Insert(multiSelectState);
			}
			multiSelectState = 0;
		}
		return outVars.Count();
	}
};