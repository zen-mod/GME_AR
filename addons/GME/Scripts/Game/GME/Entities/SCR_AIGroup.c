//------------------------------------------------------------------------------------------------
modded class SCR_AIGroup : ChimeraAIGroup
{
	protected ref ScriptInvoker m_GME_OnDoneSpawningMembers;
	
	//------------------------------------------------------------------------------------------------
	override void EndDelayedSpawn()
	{
		super.EndDelayedSpawn();
		
		if (m_GME_OnDoneSpawningMembers)
			m_GME_OnDoneSpawningMembers.Invoke(this);
	}
	
	//------------------------------------------------------------------------------------------------
	ScriptInvoker GME_GetOnDoneSpawningMembers()
	{
		if (!m_GME_OnDoneSpawningMembers)
			m_GME_OnDoneSpawningMembers = new ScriptInvoker();
		
		return m_GME_OnDoneSpawningMembers;
	}
}
