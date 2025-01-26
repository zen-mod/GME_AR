//------------------------------------------------------------------------------------------------
class GME_DeepCopyComponentClass : ScriptComponentClass
{
}

//------------------------------------------------------------------------------------------------
class GME_DeepCopyComponent : ScriptComponent
{	
	
	// var to store the set of entities. Might be changed depending on how the items should be copied
	protected set<SCR_EditableEntityComponent> m_vCopiedentities;

	
	//------------------------------------------------------------------------------------------------
	override void OnPostInit(IEntity owner)
	{
		super.OnPostInit(owner);
		
		if (!GetGame().InPlayMode())
			return;
			
		//m_pEditableEntityComponent = SCR_EditableEntityComponent.GetEditableEntity(owner);
		//if (!m_pEditableEntityComponent)
		//	return;
				
	}
	
	void setCopyData(set<SCR_EditableEntityComponent> data)
	{
		m_vCopiedentities = data;
	}
	
	set<SCR_EditableEntityComponent> getCopyData()
	{
		// TODO, need to handle null here, if var is null, we send empty set?
		return m_vCopiedentities;
	}
}
