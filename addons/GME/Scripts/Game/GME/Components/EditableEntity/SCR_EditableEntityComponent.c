//------------------------------------------------------------------------------------------------
modded class SCR_EditableEntityComponent : ScriptComponent
{
	[RplProp(onRplName: "GME_OnVisibilityChanged"), Attribute(defvalue: "true", desc: "Whether the entity should be visible")]
	protected bool m_bGME_IsVisible;
	
	//------------------------------------------------------------------------------------------------
	void GME_SetIsVisible(bool isVisible)
	{
		m_bGME_IsVisible = isVisible;
		GME_OnVisibilityChanged();
		Replication.BumpMe();
	}
	
	//------------------------------------------------------------------------------------------------
	bool GME_IsVisible()
	{
		return m_bGME_IsVisible;
	}

	//------------------------------------------------------------------------------------------------
	protected void GME_OnVisibilityChanged()
	{
		if (m_bGME_IsVisible)
			GetOwner().SetFlags(EntityFlags.VISIBLE | EntityFlags.TRACEABLE);
		else
			GetOwner().ClearFlags(EntityFlags.VISIBLE | EntityFlags.TRACEABLE);
	}
}
