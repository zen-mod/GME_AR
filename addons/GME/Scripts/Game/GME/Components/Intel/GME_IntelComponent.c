//------------------------------------------------------------------------------------------------
class GME_IntelComponentClass : ScriptComponentClass
{
}

//------------------------------------------------------------------------------------------------
class GME_IntelComponent : ScriptComponent
{	
	[RplProp(onRplName: "OnContentChanged"), Attribute(desc: "Title of the intel")]
	protected string m_sTitle;
	
	[RplProp(onRplName: "OnContentChanged"), Attribute(desc: "Content of the intel")]
	protected string m_sContent;
	
	protected InventoryItemComponent m_pItemComponent;
	protected string m_sInventoryItemDescription;
	protected SCR_EditableEntityComponent m_pEditableEntityComponent;
	protected ref GME_EditableIntelUIInfo m_EditableIntelUIInfo;
	
	//------------------------------------------------------------------------------------------------
	override void OnPostInit(IEntity owner)
	{
		super.OnPostInit(owner);
		
		if (!GetGame().InPlayMode())
			return;
		
		m_pItemComponent = InventoryItemComponent.Cast(owner.FindComponent(InventoryItemComponent));
		if (!m_pItemComponent)
			return;
		
		m_pEditableEntityComponent = SCR_EditableEntityComponent.GetEditableEntity(owner);
		if (!m_pEditableEntityComponent)
			return;
				
		if (Replication.IsServer())
		{
			m_pItemComponent.m_OnParentSlotChangedInvoker.Insert(OnPickedUpByPlayer);
			
			ChimeraWorld world = GetGame().GetWorld();
			SCR_GarbageSystem garbageSystem = SCR_GarbageSystem.GetByEntityWorld(owner);
			if (garbageSystem)
				garbageSystem.UpdateBlacklist(owner, true);
		}
	}
	
	//------------------------------------------------------------------------------------------------
	protected void OnContentChanged()
	{
		m_sInventoryItemDescription = m_sTitle;
		
		if (!m_sTitle.IsEmpty() && !m_sContent.IsEmpty())
		{
			m_sInventoryItemDescription += ":\n";
			
			for (int i = 0; i < m_sTitle.Length() + 1; i++)
			{
				m_sInventoryItemDescription += "=";
			}
			
			m_sInventoryItemDescription += "\n";
		}
		
		m_sInventoryItemDescription += m_sContent;
		
		if (m_sInventoryItemDescription.IsEmpty())
		{
			m_pEditableEntityComponent.SetInfoInstance(null);
			return;
		}
		
		if (!m_EditableIntelUIInfo)
		{
			// We have to set a custom instance, since otherwise the global info is edited
			m_EditableIntelUIInfo = new GME_EditableIntelUIInfo();
			m_EditableIntelUIInfo.CopyFrom(m_pEditableEntityComponent.GetInfo());
			m_pEditableEntityComponent.SetInfoInstance(m_EditableIntelUIInfo);
		}
		
		m_EditableIntelUIInfo.SetDescription(m_sInventoryItemDescription);
	}
	
	//------------------------------------------------------------------------------------------------
	protected void OnPickedUpByPlayer(InventoryStorageSlot oldSlot, InventoryStorageSlot newSlot)
	{
		if (!newSlot)
			return;
		
		InventoryItemComponent itemComponent = InventoryItemComponent.Cast(newSlot.GetParentContainer());
		if (!itemComponent)
			return;
		
		IEntity owner = itemComponent.GetOwner();
		if (!owner)
			return;
		
		IEntity rootOwner = owner.GetRootParent();
		if (!EntityUtils.IsPlayer(rootOwner))
			return;
		
		int playerID = GetGame().GetPlayerManager().GetPlayerIdFromControlledEntity(rootOwner);
		
		RplComponent rpl = RplComponent.Cast(GetOwner().FindComponent(RplComponent));
		if (!rpl)
			return;
		
		SCR_NotificationsComponent.SendLocalUnlimitedEditor(ENotification.GME_PLAYER_PICKED_UP_ITEM, playerID, rpl.Id());		
		m_pItemComponent.m_OnParentSlotChangedInvoker.Remove(OnPickedUpByPlayer);
	}
	
	//------------------------------------------------------------------------------------------------
	void SetTitle(string title)
	{
		m_sTitle = title;
		OnContentChanged();
		Replication.BumpMe();
	}
	
	//------------------------------------------------------------------------------------------------
	string GetTitle()
	{
		return m_sTitle;
	}

	//------------------------------------------------------------------------------------------------
	void SetContent(string content)
	{
		m_sContent = content;
		OnContentChanged();
		Replication.BumpMe();
	}
	
	//------------------------------------------------------------------------------------------------
	string GetContent()
	{
		return m_sContent;
	}
	
	//------------------------------------------------------------------------------------------------
	string GetInventoryItemDescription()
	{
		return m_sInventoryItemDescription;
	}
	
	//------------------------------------------------------------------------------------------------
	void Clear()
	{
		m_sTitle = "";
		m_sContent = "";
	}
	
	//------------------------------------------------------------------------------------------------
	bool HasIntel()
	{
		return !(m_sTitle.IsEmpty() || m_sContent.IsEmpty());
	}
}
