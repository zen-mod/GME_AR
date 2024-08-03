//------------------------------------------------------------------------------------------------
[ComponentEditorProps(category: "GameScripted/Editor (Editables)", description: "", icon: "WBData/ComponentEditorProps/componentEditor.png")]
class GME_Modules_EditableModuleComponentClass : SCR_EditableSystemComponentClass
{
}

//------------------------------------------------------------------------------------------------
class GME_Modules_EditableModuleComponent : SCR_EditableSystemComponent
{
	[Attribute(desc: "Interactive actions that will run immediately after placing the module", category: "Editable Entity")]
	protected ref array<ref GME_Modules_PlacingAction_Base> m_aPlacingActions;
	
	[RplProp()]
	protected int m_iCurrentPlacingActionIdx = 0;
	protected int m_iPlacingActionCount;
	
	protected GME_Modules_Base m_pModule;
	
	//------------------------------------------------------------------------------------------------
	void SetPlacingParamServer(string setter, IEntity value)
	{
		GetGame().GetScriptModule().Call(m_pModule, setter, false, null, value);
	}
	
	//------------------------------------------------------------------------------------------------
	IEntity GetPlacingParamServer(string getter)
	{
		IEntity value;
		GetGame().GetScriptModule().Call(m_pModule, getter, false, value);
		return value;
	}
	
	//------------------------------------------------------------------------------------------------
	override void OnPostInit(IEntity owner)
	{
		super.OnPostInit(owner);
		
		if (!GetGame().InPlayMode())
			return;
		
		m_pModule = GME_Modules_Base.Cast(owner);
		
		foreach (GME_Modules_PlacingAction_Base action : m_aPlacingActions)
		{
			action.SetModule(this);
		}
	}
	
	//------------------------------------------------------------------------------------------------
	void OnInitServer()
	{
		m_iPlacingActionCount = m_aPlacingActions.Count();
		
		if (m_iCurrentPlacingActionIdx < m_iPlacingActionCount)
			m_aPlacingActions[m_iCurrentPlacingActionIdx].OnInitServer();
		else
			m_pModule.OnInitDoneServer();
	}
	
	//------------------------------------------------------------------------------------------------
	override void SetTransform(vector transform[4], bool changedByUser = false)
	{
		vector oldTransform[4];
		GetTransform(oldTransform);
		m_pModule.OnMoved(oldTransform, transform);
		super.SetTransform(transform, changedByUser);
	}
	
	//------------------------------------------------------------------------------------------------
	//! Delete entities that got spawned by the module
	override bool Delete(bool changedByUser = false, bool updateNavmesh = false)
	{
		m_pModule.OnDelete();	
		return super.Delete(changedByUser, updateNavmesh);
	}
	
	//------------------------------------------------------------------------------------------------
	protected array<RplId> ActionParamsToRpl(array<IEntity> params)
	{
		if (!params)
			return null;
		
		array<RplId> rplParams = {};
		foreach (IEntity param : params)
		{
			RplComponent rpl = RplComponent.Cast(param.FindComponent(RplComponent));
			if (!rpl)
				continue;
			
			rplParams.Insert(rpl.Id());
		}
		
		return rplParams;
	}
	
	//------------------------------------------------------------------------------------------------
	protected array<IEntity> ActionParamsFromRpl(array<RplId> rplParams)
	{
		if (!rplParams)
			return null;
		
		array<IEntity> params = {};
		foreach (RplId rplParam : rplParams)
		{
			RplComponent rpl = RplComponent.Cast(Replication.FindItem(rplParam));
			if (!rpl)
				continue;
			
			params.Insert(rpl.GetEntity());
		}
		
		return params;
	}
	
	
	//------------------------------------------------------------------------------------------------
	void RunPlacingActionOwner(array<IEntity> params = null)
	{
		Rpc(RpcDo_RunPlacingActionOwner, ActionParamsToRpl(params));
	}
	
	//------------------------------------------------------------------------------------------------
	[RplRpc(RplChannel.Reliable, RplRcver.Owner)]
	protected void RpcDo_RunPlacingActionOwner(array<RplId> params)
	{
		m_aPlacingActions[m_iCurrentPlacingActionIdx].RunOwner(ActionParamsFromRpl(params));
	}
	
	//------------------------------------------------------------------------------------------------
	void RunPlacingActionServer(array<IEntity> params = null)
	{
		Rpc(RpcDo_RunPlacingActionServer, ActionParamsToRpl(params));
	}
	
	//------------------------------------------------------------------------------------------------
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	protected void RpcDo_RunPlacingActionServer(array<RplId> params)
	{
		m_aPlacingActions[m_iCurrentPlacingActionIdx].RunServer(ActionParamsFromRpl(params));
	}
	
	//------------------------------------------------------------------------------------------------
	void OnPlacingActionCompleted()
	{
		Rpc(OnPlacingActionCompletedServer);
	}
	
	//------------------------------------------------------------------------------------------------
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	protected void OnPlacingActionCompletedServer()
	{		
		m_iCurrentPlacingActionIdx++;
		Replication.BumpMe();
		
		if (m_iCurrentPlacingActionIdx < m_iPlacingActionCount)
			m_aPlacingActions[m_iCurrentPlacingActionIdx].OnInitServer();
		else
			m_pModule.OnInitDoneServer();
	}
	
	//------------------------------------------------------------------------------------------------
	void OnPlacingActionCanceled()
	{
		Rpc(OnPlacingActionCanceledServer);
	}
	
	//------------------------------------------------------------------------------------------------
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	protected void OnPlacingActionCanceledServer()
	{
		foreach (GME_Modules_PlacingAction_Base action : m_aPlacingActions)
		{
			action.OnCancelServer();
		}
		
		Delete();
	}
}
