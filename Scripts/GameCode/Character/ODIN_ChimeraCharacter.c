

modded class SCR_ChimeraCharacter : ChimeraCharacter
{
	[RplProp(onRplName: "ODIN_OnVisibilityValueUpdated")]
	protected bool m_bODIN_isVisible = true;
	
	void ODIN_SetVisibility(bool visible)
	{		
		m_bODIN_isVisible = visible;
		Replication.BumpMe();
		this.ODIN_OnVisibilityValueUpdated();
	}
	
	
	void ODIN_OnVisibilityValueUpdated()
	{
		Print(m_bODIN_isVisible);
		if (m_bODIN_isVisible)
			this.SetFlags(EntityFlags.VISIBLE|EntityFlags.TRACEABLE, m_bODIN_isVisible);
		else
			this.ClearFlags(EntityFlags.VISIBLE|EntityFlags.TRACEABLE);
	}
};