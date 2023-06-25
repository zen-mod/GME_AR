
//------------------------------------------------------------------------------------------------
modded class ScriptedDamageManagerComponent : BaseScriptedDamageManagerComponent
{
	[RplProp(onRplName: "ODIN_OnEnableDamageValueUpdated")]
	protected bool m_bODIN_isDamageEnabled = true;
	
	bool ODIN_IsDamageEnabled()
	{
		return m_bODIN_isDamageEnabled;
	}
	
	void ODIN_SetDamageEnabled(bool enabled)
	{
		m_bODIN_isDamageEnabled = enabled;
		Replication.BumpMe();
		this.ODIN_OnEnableDamageValueUpdated();
	}
	
	void ODIN_OnEnableDamageValueUpdated()
	{
		// add function to invoker if damage is disabled
		if (!m_bODIN_isDamageEnabled)
			this.GetOnDamage().Insert(this.ODIN_disableDamage);
		else
			this.GetOnDamage().Remove(this.ODIN_disableDamage);
	}
	
	void ODIN_disableDamage(EDamageType type, float damage, HitZone pHitZone, IEntity instigator, inout vector outMat[3], float speed, int colliderID, int nodeID)
	{
		ChimeraCharacter char = ChimeraCharacter.Cast(this.GetOwner());
		// null check might not be needed as this lives inside the ScriptedDamageManager component, and as such a char should always be "connected"
		if (!char)
			return;
		// do full heal 
		SCR_CharacterDamageManagerComponent dmgComponent = SCR_CharacterDamageManagerComponent.Cast(char.FindComponent(SCR_CharacterDamageManagerComponent));		
		if (!dmgComponent)
			return;
		
		dmgComponent.FullHeal();
	}	
};
