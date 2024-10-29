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
			m_aInitActions[m_iCurrentInitActionIdx].OnStartServer();
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
	Managed CallModuleMethod(string method, Managed param1 = null, Managed param2 = null, Managed param3 = null)
	{
		Managed output;
		
		if (!GetGame().GetScriptModule().Call(m_pModule, method, false, output, param1, param2, param3))
		{
			GME_LogHelper.Log(string.Format("%1.%2 does not exist", GetOwner().Type().ToString(), method), "GME_Modules_EditableModuleComponent", LogLevel.ERROR);
			return null;
		}
		
		return output;
	}
	
	//------------------------------------------------------------------------------------------------
	protected void CallInitActionMethodLocal(string method, Managed param = null)
	{
		if (!GetGame().GetScriptModule().Call(m_aInitActions[m_iCurrentInitActionIdx], method, false, null, param))
		{
			GME_LogHelper.Log(string.Format("%1.%2 does not exist", m_aInitActions[m_iCurrentInitActionIdx].Type().ToString(), method), "GME_Modules_EditableModuleComponent", LogLevel.ERROR);
			return;
		}
	}
	
	//------------------------------------------------------------------------------------------------
	void RpcInitActionMethod(RplRcver receiver, string method, IEntity param = null)
	{
		RplId paramId = RplId.Invalid();
		
		if (param)
		{
			RplComponent rpl = RplComponent.Cast(param.FindComponent(RplComponent));
			if (rpl)
				paramId = rpl.Id();
		}
		
		switch (receiver)
		{
			case RplRcver.Server:
			{
				Rpc(RpcDo_CallInitActionMethodServer, method, paramId);
				return;
			}
			
			case RplRcver.Owner:
			{
				Rpc(RpcDo_CallInitActionMethodOwner, method, paramId);
				return;
			}
			
			case RplRcver.Broadcast:
			{
				Rpc(RpcDo_CallInitActionMethodBroadcast, method, paramId);
				return;
			}
		}
	}
	
	//------------------------------------------------------------------------------------------------
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	protected void RpcDo_CallInitActionMethodServer(string method, RplId paramId)
	{
		IEntity param = null;
		
		RplComponent rpl = RplComponent.Cast(Replication.FindItem(paramId));
		if (rpl)
			param = rpl.GetEntity();
		
		CallInitActionMethodLocal(method, param);
	}
	
	//------------------------------------------------------------------------------------------------
	[RplRpc(RplChannel.Reliable, RplRcver.Owner)]
	protected void RpcDo_CallInitActionMethodOwner(string method, RplId paramId)
	{
		IEntity param = null;
		
		RplComponent rpl = RplComponent.Cast(Replication.FindItem(paramId));
		if (rpl)
			param = rpl.GetEntity();
		
		CallInitActionMethodLocal(method, param);
	}
	
	//------------------------------------------------------------------------------------------------
	[RplRpc(RplChannel.Reliable, RplRcver.Broadcast)]
	protected void RpcDo_CallInitActionMethodBroadcast(string method, RplId paramId)
	{
		IEntity param = null;
		
		RplComponent rpl = RplComponent.Cast(Replication.FindItem(paramId));
		if (rpl)
			param = rpl.GetEntity();
		
		CallInitActionMethodLocal(method, param);
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
		{
			m_aInitActions[m_iCurrentInitActionIdx].OnStartServer();
		}
		else
		{
			foreach (GME_Modules_InitAction_Base action : m_aInitActions)
			{
				action.OnInitDoneServer();
			}
			
			m_pModule.OnInitDoneServer();
		}	
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
			action.OnInitCanceledServer();
		}
		
		Delete();
	}
}
