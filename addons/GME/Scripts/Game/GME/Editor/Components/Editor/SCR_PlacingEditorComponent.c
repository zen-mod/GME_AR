//------------------------------------------------------------------------------------------------
modded class SCR_PlacingEditorComponent : SCR_BaseEditorComponent
{
	private ref ScriptInvoker GME_Event_OnPlacingCanceledServer = new ScriptInvoker;
	
	//------------------------------------------------------------------------------------------------
	override protected void EOnEditorInitServer()
	{
		super.EOnEditorInitServer();
		GetOnPlaceEntityServer().Insert(GME_OnPlaceEntityServer);
	}
	
	//------------------------------------------------------------------------------------------------
	//! Method for requesting the spawning of an interactive module
	//! To be called by the local GM client
	void GME_RequestPlaceModule(ResourceName resName, vector transform[4])
	{
		Rpc(RpcAsk_GME_RequestPlaceModuleServer, resName, transform, GetManager().GetPlayerID());
	}
	
	//------------------------------------------------------------------------------------------------
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	protected void RpcAsk_GME_RequestPlaceModuleServer(ResourceName resName, vector transform[4], int playerID)
	{
		Resource res = Resource.Load(resName);
		if (!res.IsValid())
			return;
		
		EntitySpawnParams params = new EntitySpawnParams();
		params.TransformMode = ETransformMode.WORLD;
		params.Transform = transform;
		
		GME_Modules_Base module = GME_Modules_Base.Cast(GetGame().SpawnEntityPrefab(res, null, params));
		if (module)
			GME_OnModulePlacedServer(module, playerID);
	}
	
	//------------------------------------------------------------------------------------------------
	//! Called when GM placed an entity
	protected void GME_OnPlaceEntityServer(RplId prefabID, SCR_EditableEntityComponent entity, int playerID)
	{
		GME_Modules_Base module = GME_Modules_Base.Cast(entity.GetOwner());
		// Handle placement of modules
		if (module)
		{
			GME_OnModulePlacedServer(module, playerID);
		}
		// Put wheeled vehicles in first gear to prevent them sliding down slopes
		else if (SCR_EditableVehicleComponent.Cast(entity))
		{
			Vehicle vehicle = Vehicle.Cast(entity.GetOwner());
			if (!vehicle)
				return;
			
			CarControllerComponent controller = CarControllerComponent.Cast(vehicle.GetVehicleController());
			if (!controller)
				return;
			
			VehicleWheeledSimulation simulation = controller.GetSimulation();
			if (!simulation)
				return;
			
			// Gear mapping: 0 => R, 1 => N, 2 => D/1, ...
			simulation.SetGear(2);
		}
	}
	
	//------------------------------------------------------------------------------------------------
	//! Transfers ownership of the module to the GM that placed it and runs module init
	protected void GME_OnModulePlacedServer(notnull GME_Modules_Base module, int playerID)
	{
		RplComponent moduleRpl = RplComponent.Cast(module.FindComponent(RplComponent));
		if (!moduleRpl)
			return;
		
		PlayerController playerCtrl = GetGame().GetPlayerManager().GetPlayerController(playerID);
		if (!playerCtrl)
			return;
		
		moduleRpl.Give(playerCtrl.GetRplIdentity());
		
		GME_Modules_EditableModuleComponent editableModuleComponent = GME_Modules_EditableModuleComponent.Cast(SCR_EditableEntityComponent.GetEditableEntity(module));
		if (editableModuleComponent)
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
