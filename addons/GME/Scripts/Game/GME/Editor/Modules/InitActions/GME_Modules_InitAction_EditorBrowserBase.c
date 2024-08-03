//------------------------------------------------------------------------------------------------
class GME_Modules_InitAction_EditorBrowserBase : GME_Modules_InitAction_Base
{
	[Attribute(desc: "Setter on the module for the spawned entity")]
	protected string m_sSpawnedEntitySetter;
	protected SCR_EditableEntityComponent m_pEntity;
	
	[Attribute(desc: "Content browser configuration")]
	protected ref SCR_EditorContentBrowserDisplayConfig m_ContentBrowserConfig;
	
	protected SCR_PlacingEditorComponent m_pPlacingManager;
	
	//------------------------------------------------------------------------------------------------
	override void OnInitServer()
	{
		m_pModule.RunInitActionOwner();
	}
	
	//------------------------------------------------------------------------------------------------
	override void RunOwner(array<IEntity> params = null)
	{
		m_pPlacingManager = SCR_PlacingEditorComponent.Cast(SCR_PlacingEditorComponent.GetInstance(SCR_PlacingEditorComponent, true, true));
		m_pModule.RunInitActionServer({m_pPlacingManager.GetOwner()});
	}
	
	//------------------------------------------------------------------------------------------------
	override void RunServer(array<IEntity> params = null)
	{
		IEntity editorModeEntity = params[0];
		m_pPlacingManager = SCR_PlacingEditorComponent.Cast(editorModeEntity.FindComponent(SCR_PlacingEditorComponent));
		m_pPlacingManager.GetOnPlaceEntityServer().Insert(OnPlacingConfirmedServer);
		m_pPlacingManager.GME_GetOnPlacingCanceledServer().Insert(OnPlacingCanceledServer);
	}
	
	//------------------------------------------------------------------------------------------------
	void OnPlacingConfirmedServer(RplId prefabID, SCR_EditableEntityComponent entity, int playerID)
	{
		m_pEntity = entity;
		m_pPlacingManager.GetOnPlaceEntityServer().Remove(OnPlacingConfirmedServer);
		m_pPlacingManager.GME_GetOnPlacingCanceledServer().Remove(OnPlacingCanceledServer);
		m_pModule.SetPlacingParamServer(m_sSpawnedEntitySetter, entity.GetOwner());
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
	override void OnCancelServer()
	{
		if (m_pEntity)
			m_pEntity.Delete();
	}
}
