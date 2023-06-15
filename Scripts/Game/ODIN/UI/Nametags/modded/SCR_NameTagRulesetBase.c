[BaseContainerProps()]
modded class SCR_NameTagRulesetBase : Managed
{
	override bool TestVisibility(SCR_NameTagData data, float timeSlice)
	{		
		// test if entity are invisible
		int visible = ODIN_VisibilityHelper.GetVisibility(data.m_Entity);
		if (!visible)
			return false;
		
		// otherwise we call super and return it, so it decides when nameplates are visible
		return super.TestVisibility(data, timeSlice);	
	}
};
