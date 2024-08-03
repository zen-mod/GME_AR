//------------------------------------------------------------------------------------------------
class GME_ModulePlacingAction_Sleep : GME_ModulePlacingAction_Base
{
	[Attribute(defvalue: "1", desc: "Delay in seconds")]
	protected float m_fDelay;
	
	//------------------------------------------------------------------------------------------------
	override void OnInitServer()
	{
		GetGame().GetCallqueue().CallLater(m_pModule.OnPlacingActionCompleted, m_fDelay * 1000);
	}
}
