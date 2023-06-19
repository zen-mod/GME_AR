modded class SCR_AICombatComponent
{
	override bool ShouldAttackEndForTarget(BaseTarget enemyTarget, out bool shouldInvestigateFurther = false, out string context = string.Empty)
	{
		bool shouldEnd = super.ShouldAttackEndForTarget(enemyTarget, shouldInvestigateFurther, context);
		
		if(!shouldEnd)
		{
			IEntity targetEntity = enemyTarget.GetTargetEntity();
			ChimeraCharacter targetCharacter = ChimeraCharacter.Cast(targetEntity);
			if (!targetCharacter)
				return true;
			
			CharacterControllerComponent controller = targetCharacter.GetCharacterController();
			
			if (controller && controller.IsUnconscious())
			{
				context = "Target entity is unconscious";
				return true;
			};
		};
		
		return shouldEnd;
	};

};