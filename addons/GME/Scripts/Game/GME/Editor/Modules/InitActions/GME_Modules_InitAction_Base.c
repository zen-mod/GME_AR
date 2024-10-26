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
	//! Execute methods remotely by calling m_pModule.RpcInitActionMethod
	//! Execute methods of the module dynamically by calling m_pModule.CallModuleMethod
	//! To mark the action as completed, call m_pModule.OnInitActionCompleted()
	//! To mark the action as canceled, call m_pModule.OnInitActionCanceled()
	void OnStartServer();
	
	//------------------------------------------------------------------------------------------------
	//! Called when all init actions are completed
	void OnInitDoneServer();
	
	//------------------------------------------------------------------------------------------------
	//! Called when any init action has been canceled
	//! Do clean-up here when module has been canceled
	void OnInitCanceledServer();
}
