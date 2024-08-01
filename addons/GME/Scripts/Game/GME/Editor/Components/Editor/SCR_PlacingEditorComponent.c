//------------------------------------------------------------------------------------------------
modded class SCR_PlacingEditorComponent : SCR_BaseEditorComponent
{
	private ref ScriptInvoker GME_Event_OnPlacingCanceledServer = new ScriptInvoker;
	
	//------------------------------------------------------------------------------------------------
	ScriptInvoker GME_GetOnPlacingCanceledServer()
	{
		return GME_Event_OnPlacingCanceledServer;
	}
	
	//------------------------------------------------------------------------------------------------
	void GME_OnPlacingCanceled()
	{
		Rpc(RpcAsk_Server_GME_OnPlacingCanceled);
	}
	
	//------------------------------------------------------------------------------------------------
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	protected void RpcAsk_Server_GME_OnPlacingCanceled()
	{
		GME_Event_OnPlacingCanceledServer.Invoke();
	}
}
