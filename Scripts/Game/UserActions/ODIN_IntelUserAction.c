class ODIN_IntelUserAction : ScriptedUserAction
{
	protected ODIN_IntelManagerComponent m_pIntelManager = null;
		
	override void Init(IEntity pOwnerEntity, GenericComponent pManagerComponent) 
	{
		m_pIntelManager = ODIN_IntelManagerComponent.Cast(pOwnerEntity.FindComponent(ODIN_IntelManagerComponent));
	}
	
	override bool GetActionNameScript(out string outName)
	{
		if (!m_pIntelManager) return false;
		outName = m_pIntelManager.GetActionName();
		return true;
	}
	
	override bool CanBeShownScript(IEntity user) {
		if (!m_pIntelManager)
			return false;
		return m_pIntelManager.HasIntel();
	}
	
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity) 
	{
		m_pIntelManager.AddIntel(pUserEntity);
	}
};
