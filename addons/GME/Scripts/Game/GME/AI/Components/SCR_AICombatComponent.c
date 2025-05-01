//------------------------------------------------------------------------------------------------
modded class SCR_AICombatComponent : ScriptComponent
{
	//------------------------------------------------------------------------------------------------
	//! Resets selected target if it is not visible
	override void EvaluateWeaponAndTarget(out bool outWeaponEvent, out bool outSelectedTargetChanged,
		out BaseTarget outPrevTarget, out BaseTarget outCurrentTarget,
		out bool outRetreatTargetChanged, out bool outCompartmentChanged)
	{
		BaseTarget prevTarget = m_SelectedTarget;
		super.EvaluateWeaponAndTarget(outWeaponEvent, outSelectedTargetChanged, outPrevTarget, outCurrentTarget, outRetreatTargetChanged, outCompartmentChanged);
		
		if (!outCurrentTarget)
			return;
		
		IEntity currentEntity = outCurrentTarget.GetTargetEntity();
		if (!currentEntity)
			return;
		
		SCR_EditableEntityComponent editableEntity = SCR_EditableEntityComponent.GetEditableEntity(currentEntity);
		if (!editableEntity || editableEntity.GME_IsVisible())
			return;
			
		m_SelectedTarget = prevTarget;
		outCurrentTarget = prevTarget;
		outSelectedTargetChanged = false;
	}
}
