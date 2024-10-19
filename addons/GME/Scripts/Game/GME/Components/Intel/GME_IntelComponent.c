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
	protected UIInfo m_ItemUIInfo;
	protected SCR_EditableEntityUIInfo m_EditableEntityUIInfo;
	
	//------------------------------------------------------------------------------------------------
	override void OnPostInit(IEntity owner)
	{
		super.OnPostInit(owner);
		
		if (!GetGame().InPlayMode())
			return;
		
		m_pItemComponent = InventoryItemComponent.Cast(owner.FindComponent(InventoryItemComponent));
		if (!m_pItemComponent)
			return;
		
		m_ItemUIInfo = m_pItemComponent.GetUIInfo();
		
		SCR_EditableEntityComponent editableEntityComponent = SCR_EditableEntityComponent.Cast(owner.FindComponent(SCR_EditableEntityComponent));
		if (!editableEntityComponent)
			return;
		
		m_EditableEntityUIInfo = SCR_EditableEntityUIInfo.Cast(editableEntityComponent.GetInfo());
		
		if (Replication.IsServer())
			m_pItemComponent.m_OnParentSlotChangedInvoker.Insert(OnPickedUpByPlayer);
	}
	
	//------------------------------------------------------------------------------------------------
	protected void OnContentChanged()
	{
		string description = m_sTitle;
		
		if (!m_sTitle.IsEmpty() && !m_sContent.IsEmpty())
		{
			description += ":\n";
			
			for (int i = 0; i < m_sTitle.Length() + 1; i++)
			{
				description += "=";
			}
			
			description += "\n";
		}
		
		description += m_sContent;
		m_ItemUIInfo.SetDescription(description);
		m_EditableEntityUIInfo.SetDescription(description);
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
