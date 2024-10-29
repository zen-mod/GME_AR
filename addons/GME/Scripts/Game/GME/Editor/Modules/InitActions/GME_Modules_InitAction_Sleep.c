//------------------------------------------------------------------------------------------------
class GME_Modules_InitAction_Sleep : GME_Modules_InitAction_Base
{
	[Attribute(defvalue: "1", desc: "Delay in seconds")]
	protected float m_fDelay;
	
	//------------------------------------------------------------------------------------------------
	override void OnStartServer()
	{
		GetGame().GetCallqueue().CallLater(m_pModule.OnInitActionCompleted, m_fDelay * 1000);
	}
}
