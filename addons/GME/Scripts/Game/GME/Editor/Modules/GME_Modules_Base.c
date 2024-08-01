//------------------------------------------------------------------------------------------------
class GME_Modules_BaseClass : GenericEntityClass
{
}

//------------------------------------------------------------------------------------------------
class GME_Modules_Base : GenericEntity
{
	[Attribute(defvalue: "true", desc: "Attributes window will pop up when module gets placed")]
	protected bool m_bShowAttributesOnInit;
	
	[Attribute(defvalue: "false", desc: "Module will get deleted immediatly when true")]
	protected bool m_bDeleteImmediately;
	
	protected bool m_bInitDone = false;
	
	//------------------------------------------------------------------------------------------------
	//! Add functionality to the module by overriding this method
	void Run();
	
	//------------------------------------------------------------------------------------------------
	//! Triggered when the module gets moved
	void OnMoved(vector oldTransform[4], vector newTransform[4]);
	
	//------------------------------------------------------------------------------------------------
	//! Triggered when the module gets deleted
	void OnDelete();
	
	//------------------------------------------------------------------------------------------------
	//! Returns true after the initial attribute window was closed
	//! Can be used to distinguish whether an attribute is changed during init or later
	bool IsInitDone()
	{
		return m_bInitDone;
	}
	
	//------------------------------------------------------------------------------------------------
	void GME_Module_Base(IEntitySource src, IEntity parent)
	{
		SetEventMask(EntityEvent.INIT);
	}
	
	//------------------------------------------------------------------------------------------------
	override protected void EOnInit(IEntity owner)
	{
		super.EOnInit(owner);
		
		if (!GetGame().InPlayMode())
			return;
		
		RplComponent rpl = RplComponent.Cast(FindComponent(RplComponent));
		if (!rpl)
			return;
		
		if (m_bShowAttributesOnInit)
		{
			SCR_AttributesManagerEditorComponent attributesManager = SCR_AttributesManagerEditorComponent.Cast(SCR_AttributesManagerEditorComponent.GetInstance(SCR_AttributesManagerEditorComponent));
			if (!attributesManager)
				return;
			
			if (rpl.Role() == RplRole.Authority)
			{
				attributesManager.GetOnAttributesConfirm().Insert(OnInitialDialogClosedServer);
				attributesManager.GetOnAttributesCancel().Insert(OnInitialDialogClosedServer)
			}
			
			if (rpl.IsOwner())
			{
				attributesManager.StartEditing(FindComponent(GME_EditableModuleComponent));
			}
		}
		else
		{
			if (rpl.Role() == RplRole.Authority)
				OnInitDoneServer();
		}
	}
	
	//------------------------------------------------------------------------------------------------
	//! Triggered when the initial attribute window gets closed 
	protected void OnInitialDialogClosedServer()
	{
		SCR_AttributesManagerEditorComponent attributesManager = SCR_AttributesManagerEditorComponent.Cast(SCR_AttributesManagerEditorComponent.GetInstance(SCR_AttributesManagerEditorComponent));
		if (attributesManager)
		{
			attributesManager.GetOnAttributesConfirm().Remove(OnInitialDialogClosedServer);
			attributesManager.GetOnAttributesCancel().Remove(OnInitialDialogClosedServer);
		}
		
		OnInitDoneServer();
	}
	
	//------------------------------------------------------------------------------------------------
	protected void OnInitDoneServer()
	{
		m_bInitDone = true;
		Run();
		
		if (m_bDeleteImmediately)
		{
			OnDelete();
			SCR_EntityHelper.DeleteEntityAndChildren(this);
		}
	}
}

