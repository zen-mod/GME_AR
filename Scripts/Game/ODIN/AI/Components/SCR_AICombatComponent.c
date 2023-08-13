

modded class SCR_AICombatComponent : ScriptComponent
{
	override bool SelectTarget(BaseTarget newTarget)
	{
		// gotta do null check as otherwise we crash, as when there is no targets at spawn, NewTarget is null. So we just call super and call it a day
		if (!newTarget)
			return super.SelectTarget(newTarget);
		
		// get visibility, if not visible AI can't change to that target, otherwise up to super
		bool visible = ODIN_VisibilityHelper.GetVisibility(newTarget.GetTargetEntity());
		if (!visible)
			return false;
		else
			return super.SelectTarget(newTarget);
	}
};