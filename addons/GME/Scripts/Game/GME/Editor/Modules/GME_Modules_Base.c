//------------------------------------------------------------------------------------------------
class GME_Modules_BaseClass : GenericEntityClass
{
}

//------------------------------------------------------------------------------------------------
//! Modules can be used in three ways:
//! 1) Placed in world editor for immediate execution when the scenario starts (non-interactive)
//! 2) Placed by GM (interactive)
//! 3) Spawned with scripts (non-interactive)
//!
//! Basic procedure when spawned via scripts:
//! > GME_Modules_MyModule module = GME_Modules_MyModule.Cast(GetGame().SpawnEntityPrefab(modulePrefabName, null, spawnParams));
//! > module.SetSomeParam(someModuleParam); // You only need this if you haven't specified the params already in the prefab.
//! > module.OnInitDoneServer();
class GME_Modules_Base : GenericEntity
{
	[Attribute(defvalue: "true", desc: "Module will get deleted immediatly after executing Run() when true")]
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
		
		if (!GetGame().InPlayMode() || !Replication.IsServer())
			return;
		
		// Complete init when module was placed in the world editor 
		if (Replication.Loadtime())
			OnInitDoneServer();
	}
	
	//------------------------------------------------------------------------------------------------
	//! Called when init is done
	//! Executes Run()
	//! There are three possible ways this function gets called:
	//! 1) By GME_Modules_EditableModuleComponent when spawned by GM
	//! 2) Immediately after EOnInit when it is part of the world editor
	//! 3) Manually when spawned via SpawnEntityPrefab
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	void OnInitDoneServer()
	{
		if (m_bInitDone)
			return;
		
		m_bInitDone = true;
		Run();
		
		if (m_bDeleteImmediately)
			SCR_EntityHelper.DeleteEntityAndChildren(this);
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
}
