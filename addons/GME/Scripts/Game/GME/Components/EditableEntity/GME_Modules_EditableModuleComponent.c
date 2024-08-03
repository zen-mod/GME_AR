//------------------------------------------------------------------------------------------------
[ComponentEditorProps(category: "GameScripted/Editor (Editables)", description: "", icon: "WBData/ComponentEditorProps/componentEditor.png")]
class GME_Modules_EditableModuleComponentClass : SCR_EditableSystemComponentClass
{
}

//------------------------------------------------------------------------------------------------
class GME_Modules_EditableModuleComponent : SCR_EditableSystemComponent
{
	[Attribute(desc: "Interactive actions that will run immediately after placing the module", category: "Editable Entity")]
	protected ref array<ref GME_Modules_InitAction_Base> m_aInitActions;
	
	[RplProp()]
	protected int m_iCurrentInitActionIdx = 0;
	protected int m_iInitActionCount;
	
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
		
		foreach (GME_Modules_InitAction_Base action : m_aInitActions)
		{
			action.SetModule(this);
		}
	}
	
	//------------------------------------------------------------------------------------------------
	void OnInitServer()
	{
		m_iInitActionCount = m_aInitActions.Count();
		
		if (m_iCurrentInitActionIdx < m_iInitActionCount)
			m_aInitActions[m_iCurrentInitActionIdx].OnInitServer();
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
	void RunInitActionOwner(array<IEntity> params = null)
	{
		Rpc(RpcDo_RunInitActionOwner, ActionParamsToRpl(params));
	}
	
	//------------------------------------------------------------------------------------------------
	[RplRpc(RplChannel.Reliable, RplRcver.Owner)]
	protected void RpcDo_RunInitActionOwner(array<RplId> params)
	{
		m_aInitActions[m_iCurrentInitActionIdx].RunOwner(ActionParamsFromRpl(params));
	}
	
	//------------------------------------------------------------------------------------------------
	void RunInitActionServer(array<IEntity> params = null)
	{
		Rpc(RpcDo_RunInitActionServer, ActionParamsToRpl(params));
	}
	
	//------------------------------------------------------------------------------------------------
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	protected void RpcDo_RunInitActionServer(array<RplId> params)
	{
		m_aInitActions[m_iCurrentInitActionIdx].RunServer(ActionParamsFromRpl(params));
	}
	
	//------------------------------------------------------------------------------------------------
	void OnInitActionCompleted()
	{
		Rpc(OnInitActionCompletedServer);
	}
	
	//------------------------------------------------------------------------------------------------
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	protected void OnInitActionCompletedServer()
	{		
		m_iCurrentInitActionIdx++;
		Replication.BumpMe();
		
		if (m_iCurrentInitActionIdx < m_iInitActionCount)
			m_aInitActions[m_iCurrentInitActionIdx].OnInitServer();
		else
			m_pModule.OnInitDoneServer();
	}
	
	//------------------------------------------------------------------------------------------------
	void OnInitActionCanceled()
	{
		Rpc(OnInitActionCanceledServer);
	}
	
	//------------------------------------------------------------------------------------------------
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	protected void OnInitActionCanceledServer()
	{
		foreach (GME_Modules_InitAction_Base action : m_aInitActions)
		{
			action.OnCancelServer();
		}
		
		Delete();
	}
}
