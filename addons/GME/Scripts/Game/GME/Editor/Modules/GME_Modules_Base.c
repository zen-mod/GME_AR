//------------------------------------------------------------------------------------------------
class GME_Modules_BaseClass : GenericEntityClass
{
}

//------------------------------------------------------------------------------------------------
class GME_Modules_Base : GenericEntity
{
	[Attribute(defvalue: "true", desc: "Module will get deleted immediatly when true")]
	protected bool m_bDeleteImmediately;
	
	protected bool m_bInitDone = false;
	
	//------------------------------------------------------------------------------------------------
	void GME_Modules_Base(IEntitySource src, IEntity parent)
	{
		SetEventMask(EntityEvent.INIT);
	}
	
	//------------------------------------------------------------------------------------------------
	override protected void EOnInit(IEntity owner)
	{
		super.EOnInit(owner);
		
		if (!GetGame().InPlayMode() && !Replication.IsServer())
			return;
		
		// Interactive mode: OnInitDoneServer gets called by GME_EditableModuleComponent instead
		if (FindComponent(GME_EditableModuleComponent))
			return;
		
		// Complete init for non-interative mode
		OnInitDoneServer();
	}
	
	//------------------------------------------------------------------------------------------------
	//! Add functionality to the module by overriding this method
	void Run();
	
	//------------------------------------------------------------------------------------------------
	//! Triggered when the module gets moved
	void OnMoved(vector oldTransform[4], vector newTransform[4]);
	
	//------------------------------------------------------------------------------------------------
	//! Triggered when the module gets deleted
	void OnDelete();
	
	//------------------------------------------------------------------------------------------------
	//! Returns true after the initial attribute window was closed
	//! Can be used to distinguish whether an attribute is changed during init or later
	bool IsInitDone()
	{
		return m_bInitDone;
	}
	
	//------------------------------------------------------------------------------------------------
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	void OnInitDoneServer()
	{
		m_bInitDone = true;
		Run();
		
		if (m_bDeleteImmediately)
			SCR_EntityHelper.DeleteEntityAndChildren(this);
	}
}
