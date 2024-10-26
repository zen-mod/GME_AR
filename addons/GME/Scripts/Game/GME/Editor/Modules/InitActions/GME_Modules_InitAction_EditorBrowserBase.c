//------------------------------------------------------------------------------------------------
class GME_Modules_InitAction_EditorBrowserBase : GME_Modules_InitAction_Base
{
	[Attribute(desc: "Setter on the module for the spawned entity")]
	protected string m_sSpawnedEntitySetter;
	protected SCR_EditableEntityComponent m_pEntity;
	protected SimulationState m_eEntitySimulationState;
	
	[Attribute(desc: "Content browser configuration")]
	protected ref SCR_EditorContentBrowserDisplayConfig m_ContentBrowserConfig;
	
	protected SCR_PlacingEditorComponent m_pPlacingManager;
	
	//------------------------------------------------------------------------------------------------
	void SetEditorModeEntity(SCR_EditorModeEntity entity)
	{
		m_pPlacingManager = SCR_PlacingEditorComponent.Cast(entity.FindComponent(SCR_PlacingEditorComponent));
	}
	
	//------------------------------------------------------------------------------------------------
	override void OnStartServer()
	{
		// Delay opening of content browser, since it will fail otherwise when the previous action was also a content browser
		GetGame().GetCallqueue().CallLater(m_pModule.RpcInitActionMethod, 100, false, RplRcver.Owner, "OpenContentBrowser", null);
	}
	
	//------------------------------------------------------------------------------------------------
	void OpenContentBrowser()
	{
		SCR_EditorModeEntity editorEntity = SCR_EditorModeEntity.GetInstance();
		if (!editorEntity)
			return m_pModule.OnInitActionCanceled();
		
		SetEditorModeEntity(editorEntity);
		m_pModule.RpcInitActionMethod(RplRcver.Server, "SetEditorModeEntity", editorEntity);		
		m_pModule.RpcInitActionMethod(RplRcver.Server, "AttachServerPlacingHandlers");
	}
	
	//------------------------------------------------------------------------------------------------
	void AttachServerPlacingHandlers()
	{
		m_pPlacingManager.GetOnPlaceEntityServer().Insert(OnPlacingConfirmedServer);
		m_pPlacingManager.GME_GetOnPlacingCanceledServer().Insert(OnPlacingCanceledServer);
	}
	
	//------------------------------------------------------------------------------------------------
	void OnPlacingConfirmedServer(RplId prefabID, SCR_EditableEntityComponent entity, int playerID)
	{
		m_pEntity = entity;
		
		Physics physics = m_pEntity.GetOwner().GetPhysics();
		if (physics)
		{
			m_eEntitySimulationState = physics.GetSimulationState();
			physics.ChangeSimulationState(SimulationState.NONE);
		}
		
		m_pPlacingManager.GetOnPlaceEntityServer().Remove(OnPlacingConfirmedServer);
		m_pPlacingManager.GME_GetOnPlacingCanceledServer().Remove(OnPlacingCanceledServer);
		m_pModule.CallModuleMethod(m_sSpawnedEntitySetter, entity.GetOwner());
		m_pModule.OnInitActionCompleted();
	}
	
	//------------------------------------------------------------------------------------------------
	void OnPlacingCanceledServer()
	{
		m_pPlacingManager.GetOnPlaceEntityServer().Remove(OnPlacingConfirmedServer);
		m_pPlacingManager.GME_GetOnPlacingCanceledServer().Remove(OnPlacingCanceledServer);
		m_pModule.OnInitActionCanceled();
	}
	
	//------------------------------------------------------------------------------------------------
	override void OnInitDoneServer()
	{
		if (!m_pEntity)
			return;
		
		Physics physics = m_pEntity.GetOwner().GetPhysics();
		if (!physics)
			return;

		physics.ChangeSimulationState(m_eEntitySimulationState);
		physics.SetActive(ActiveState.ACTIVE);
	}
	
	//------------------------------------------------------------------------------------------------
	override void OnInitCanceledServer()
	{
		if (m_pEntity)
			m_pEntity.Delete();
	}
}
