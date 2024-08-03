//------------------------------------------------------------------------------------------------
class GME_Modules_PlacingAction_Base : ScriptAndConfig
{
	protected GME_Modules_EditableModuleComponent m_pModule;
	
	//------------------------------------------------------------------------------------------------
	void SetModule(GME_Modules_EditableModuleComponent module)
	{
		m_pModule = module;
	}
	
	//------------------------------------------------------------------------------------------------
	//! Called when action is started
	//! Continue here to run an action on owner or server by calling m_pModule.RunPlacingActionOwner()
	//! or m_pModule.RunPlacingActionServer()
	//! To mark the action as complated, call m_pModule.OnPlacingActionCompleted()
	//! To mark the action as canceled, call m_pModule.OnPlacingActionCanceled()
	void OnInitServer();
	
	//------------------------------------------------------------------------------------------------
	//! Executed when m_pModule.RunPlacingActionOwner() is called
	void RunOwner(array<IEntity> params = null);
	
	//------------------------------------------------------------------------------------------------
	//! Executed when m_pModule.RunPlacingActionServer() is called
	void RunServer(array<IEntity> params = null);
	
	//------------------------------------------------------------------------------------------------
	//! Do clean-up here when module has been canceled
	void OnCancelServer();
}
