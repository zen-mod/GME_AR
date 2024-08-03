//------------------------------------------------------------------------------------------------
[BaseContainerProps()]
class GME_EditablePointUIInfo : SCR_EditableEntityUIInfo
{
	protected LocalizedString m_sCallsign;
	
	//------------------------------------------------------------------------------------------------
	override LocalizedString GetName()
	{
		if (m_sCallsign.IsEmpty())
			return super.GetName();
		else
			return string.Format("%1: %2", super.GetName(), m_sCallsign);
	}
	
	//------------------------------------------------------------------------------------------------
	// override without 'protected' keyword
	override void CopyFrom(SCR_UIName source)
	{
		super.CopyFrom(source);
	}
	
	//------------------------------------------------------------------------------------------------
	void SetCallsign(LocalizedString callsign)
	{
		m_sCallsign = callsign;
	}
}
