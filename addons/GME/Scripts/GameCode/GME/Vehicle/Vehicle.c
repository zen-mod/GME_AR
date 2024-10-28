

//------------------------------------------------------------------------------------------------
modded class Vehicle : BaseVehicle
{
	[RplProp(onRplName: "GME_OnVisibilityValueUpdated")]
	protected bool m_bGME_isVisible = true;
	
	void GME_SetVisibility(bool visible)
	{		
		m_bGME_isVisible = visible;
		Replication.BumpMe();
		GME_OnVisibilityValueUpdated();
	}
	
	
	void GME_OnVisibilityValueUpdated()
	{
		if (m_bGME_isVisible)
			SetFlags(EntityFlags.VISIBLE | EntityFlags.TRACEABLE);
		else
			ClearFlags(EntityFlags.VISIBLE | EntityFlags.TRACEABLE);
	}
};