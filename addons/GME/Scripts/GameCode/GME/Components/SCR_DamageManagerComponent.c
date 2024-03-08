//------------------------------------------------------------------------------------------------
modded class SCR_DamageManagerComponent : BaseSCR_DamageManagerComponent
{
	protected bool m_bGME_isDamageEnabled = true;
	
	//------------------------------------------------------------------------------------------------
	bool GME_IsDamageEnabled()
	{
		return m_bGME_isDamageEnabled;
	}
	
	//------------------------------------------------------------------------------------------------
	void GME_SetDamageEnabled(bool enabled)
	{
		m_bGME_isDamageEnabled = enabled;
	}
	
	//------------------------------------------------------------------------------------------------
	override bool SCR_HijackDamageHandling(notnull BaseDamageContext damageContext)
	{
		if (m_bGME_isDamageEnabled)
			return super.SCR_HijackDamageHandling(damageContext);
		
		return true;
	}
}
