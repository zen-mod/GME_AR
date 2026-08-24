//------------------------------------------------------------------------------------------------
modded class SCR_AIGroup : ChimeraAIGroup
{
	protected bool m_bGME_IsMemberSpawningDone = false;
	
	//------------------------------------------------------------------------------------------------
	override void EOnInit(IEntity owner)
	{
		super.EOnInit(owner);

		// Inserted before any consumer, so that GME_IsMemberSpawningDone() is already up to date
		// once their handlers are invoked
		GetOnAllDelayedEntitySpawned().Insert(GME_OnAllDelayedEntitySpawned);
	}

	//------------------------------------------------------------------------------------------------
	protected void GME_OnAllDelayedEntitySpawned(SCR_AIGroup group)
	{
		m_bGME_IsMemberSpawningDone = true;
	}
	
	//------------------------------------------------------------------------------------------------
	//! Event invoked once all delayed group members have spawned.
	//! Provided by the base game since Reforger 1.8.0, GME only tracks its completion state.
	ScriptInvokerBase<ScriptInvokerAIGroup> GME_GetOnDoneSpawningMembers()
	{
		return GetOnAllDelayedEntitySpawned();
	}
	
	//------------------------------------------------------------------------------------------------
	bool GME_IsMemberSpawningDone()
	{
		return m_bGME_IsMemberSpawningDone;
	}
}
