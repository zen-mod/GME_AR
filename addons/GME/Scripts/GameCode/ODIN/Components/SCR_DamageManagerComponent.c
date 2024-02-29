//------------------------------------------------------------------------------------------------
modded class SCR_DamageManagerComponent : BaseSCR_DamageManagerComponent
{
	protected bool m_bODIN_isDamageEnabled = true;
	
	//------------------------------------------------------------------------------------------------
	bool ODIN_IsDamageEnabled()
	{
		return m_bODIN_isDamageEnabled;
	}
	
	//------------------------------------------------------------------------------------------------
	void ODIN_SetDamageEnabled(bool enabled)
	{
		m_bODIN_isDamageEnabled = enabled;
	}
	
	//------------------------------------------------------------------------------------------------
	override bool SCR_HijackDamageHandling(notnull BaseDamageContext damageContext)
	{
		if (m_bODIN_isDamageEnabled)
			return super.SCR_HijackDamageHandling(damageContext);
		
		return true;
	}
}
