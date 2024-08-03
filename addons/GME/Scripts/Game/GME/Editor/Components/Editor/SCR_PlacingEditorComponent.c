//------------------------------------------------------------------------------------------------
modded class SCR_PlacingEditorComponent : SCR_BaseEditorComponent
{
	private ref ScriptInvoker GME_Event_OnPlacingCanceledServer = new ScriptInvoker;
	
	//------------------------------------------------------------------------------------------------
	override protected void EOnEditorInitServer()
	{
		super.EOnEditorInitServer();
		GetOnPlaceEntityServer().Insert(GME_GetOnPlaceEntityServer);
	}
	
	//------------------------------------------------------------------------------------------------
	//! Transfer ownership of GME modules to player that placed it and run init
	protected void GME_GetOnPlaceEntityServer(RplId prefabID, SCR_EditableEntityComponent entity, int playerID)
	{
		GME_Modules_Base module = GME_Modules_Base.Cast(entity.GetOwner());
		if (!module)
			return;
		
		RplComponent moduleRpl = RplComponent.Cast(module.FindComponent(RplComponent));
		PlayerController playerCtrl = GetGame().GetPlayerManager().GetPlayerController(playerID);
		moduleRpl.Give(playerCtrl.GetRplIdentity());
		GME_EditableModuleComponent editableModuleComponent = GME_EditableModuleComponent.Cast(entity);
		editableModuleComponent.OnInitServer();
	}
		
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
