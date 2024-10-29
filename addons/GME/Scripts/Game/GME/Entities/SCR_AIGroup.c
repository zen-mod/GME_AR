//------------------------------------------------------------------------------------------------
modded class SCR_AIGroup : ChimeraAIGroup
{
	protected bool m_bGME_IsMemberSpawningDone = false;
	protected ref ScriptInvoker m_GME_OnDoneSpawningMembers;
	
	//------------------------------------------------------------------------------------------------
	override void EndDelayedSpawn()
	{
		super.EndDelayedSpawn();
		
		if (m_GME_OnDoneSpawningMembers)
			m_GME_OnDoneSpawningMembers.Invoke(this);
		
		m_bGME_IsMemberSpawningDone = true;
	}
	
	//------------------------------------------------------------------------------------------------
	ScriptInvoker GME_GetOnDoneSpawningMembers()
	{
		if (!m_GME_OnDoneSpawningMembers)
			m_GME_OnDoneSpawningMembers = new ScriptInvoker();
		
		return m_GME_OnDoneSpawningMembers;
	}
	
	//------------------------------------------------------------------------------------------------
	bool GME_IsMemberSpawningDone()
	{
		return m_bGME_IsMemberSpawningDone;
	}
}
