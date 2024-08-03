//------------------------------------------------------------------------------------------------
class GME_Modules_InitAction_Base : ScriptAndConfig
{
	protected GME_Modules_EditableModuleComponent m_pModule;
	
	//------------------------------------------------------------------------------------------------
	void SetModule(GME_Modules_EditableModuleComponent module)
	{
		m_pModule = module;
	}
	
	//------------------------------------------------------------------------------------------------
	//! Called when action is started
	//! Continue here to run an action on owner or server by calling m_pModule.RunInitActionOwner()
	//! or m_pModule.RunInitActionServer()
	//! To mark the action as complated, call m_pModule.OnInitActionCompleted()
	//! To mark the action as canceled, call m_pModule.OnInitActionCanceled()
	void OnInitServer();
	
	//------------------------------------------------------------------------------------------------
	//! Executed when m_pModule.RunInitActionOwner() is called
	void RunOwner(array<IEntity> params = null);
	
	//------------------------------------------------------------------------------------------------
	//! Executed when m_pModule.RunInitActionServer() is called
	void RunServer(array<IEntity> params = null);
	
	//------------------------------------------------------------------------------------------------
	//! Called when all init actions are completed
	void OnConfirmServer();
	
	//------------------------------------------------------------------------------------------------
	//! Called when any init action has been cancelled
	//! Do clean-up here when module has been canceled
	void OnCancelServer();
}
