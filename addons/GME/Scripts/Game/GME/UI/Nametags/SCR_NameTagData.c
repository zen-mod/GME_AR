//------------------------------------------------------------------------------------------------
//! Nametag data
modded class SCR_NameTagData : Managed
{
	protected SCR_EditableEntityComponent m_pGME_EditableEntity;
	
	//------------------------------------------------------------------------------------------------
	//! Init nametag data class, the widget is held and reused, data has to be reinitialized 
	override bool InitTag(SCR_NameTagDisplay display, IEntity entity, SCR_NameTagConfig config, bool IsCurrentPlayer = false)
	{
		m_pGME_EditableEntity = SCR_EditableEntityComponent.GetEditableEntity(entity);
		return super.InitTag(display, entity, config, IsCurrentPlayer);
	}
	
	//------------------------------------------------------------------------------------------------
	bool GME_IsVisible()
	{
		if (!m_pGME_EditableEntity)
			return true;
		
		return m_pGME_EditableEntity.GME_IsVisible();
	}
}
