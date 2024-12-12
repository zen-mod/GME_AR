class GME_ListboxSingleSelectDialog: ChimeraMenuBase
{
	protected SCR_InputButtonComponent m_Cancel;
	protected SCR_InputButtonComponent m_Confirm;
	
	ref ScriptInvoker m_OnConfirm = new ScriptInvoker();
	ref ScriptInvoker m_OnCancel = new ScriptInvoker();
	
	protected float m_fAnimationRate = UIConstants.FADE_RATE_FAST;
	
	protected OverlayWidget m_ListBoxOverlay;
    protected SCR_ListBoxComponent m_ListBoxComponent;
	
	protected TextWidget m_wTitle;
	
	// passed entity for storing owner/caller
	protected SCR_EditableEntityComponent m_HoveredEntity;

	//------------------------------------------------------------------------------------------------
	protected override void OnMenuOpen()
	{
		Widget rootWidget = GetRootWidget();
		if (!rootWidget)
		{
			GME_LogHelper.Log("Could not get root widget", "ListboxSingleSelect", LogLevel.ERROR);
			return;
		}
		
		// Play animation
		rootWidget.SetOpacity(0);
		AnimateWidget.Opacity(rootWidget, 1, m_fAnimationRate);

		// Texts
		m_wTitle = TextWidget.Cast(rootWidget.FindAnyWidget("TextTitel"));
		
		// Cancel button
		Widget m_wCancelButton = rootWidget.FindAnyWidget("ButtonCancel");
		m_Cancel = SCR_InputButtonComponent.GetInputButtonComponent("ButtonCancel", rootWidget);
		if (m_Cancel)
			m_Cancel.m_OnActivated.Insert(OnCancel);

		// Confirm button
		m_Confirm = SCR_InputButtonComponent.GetInputButtonComponent("ButtonOK", rootWidget);
		if (m_Confirm)
			m_Confirm.m_OnActivated.Insert(OnConfirm);
		
		// Get listbox component
		m_ListBoxOverlay = OverlayWidget.Cast(rootWidget.FindAnyWidget("ListBox0"));
        m_ListBoxComponent = SCR_ListBoxComponent.Cast(m_ListBoxOverlay.FindHandler(SCR_ListBoxComponent));
		
		/*
			ESC/Start listener
		*/

		InputManager inputManager = GetGame().GetInputManager();
		if (inputManager)
		{
			// this is for the menu/dialog to close when pressing ESC
			// an alternative is to have a button with the SCR_NavigationButtonComponent component
			// and its Action Name field set to MenuBack - this would activate the button on ESC press
			inputManager.AddActionListener("MenuOpen", EActionTrigger.DOWN, CloseAnimated);
			inputManager.AddActionListener("MenuBack", EActionTrigger.DOWN, CloseAnimated);
#ifdef WORKBENCH // in Workbench, F10 is used because ESC closes the preview
			inputManager.AddActionListener("MenuOpenWB", EActionTrigger.DOWN, CloseAnimated);
			inputManager.AddActionListener("MenuBackWB", EActionTrigger.DOWN, CloseAnimated);
#endif
		}
		else if (!m_wCancelButton)
		{
			GME_LogHelper.Log("Auto-closing the dialog that has no exit path", "ListboxSingleSelect", LogLevel.WARNING);
			Close();
			return;
		}
	}

	//------------------------------------------------------------------------------------------------
	protected override void OnMenuClose()
	{
		// here we clean action listeners added above as the good practice wants it
		InputManager inputManager = GetGame().GetInputManager();
		if (inputManager)
		{
			inputManager.RemoveActionListener("MenuOpen", EActionTrigger.DOWN, CloseAnimated);
			inputManager.RemoveActionListener("MenuBack", EActionTrigger.DOWN, CloseAnimated);
#ifdef WORKBENCH
			inputManager.RemoveActionListener("MenuOpenWB", EActionTrigger.DOWN, CloseAnimated);
			inputManager.RemoveActionListener("MenuBackWB", EActionTrigger.DOWN, CloseAnimated);
#endif
		}
	}
	
	//------------------------------------------------------------------------------------------------
	void AddItemToListbox(string name, Managed data = null)
	{
        if (m_ListBoxComponent)
			m_ListBoxComponent.AddItem(name, data);	
	}
	
	void AddItemAndIconToListbox(string name, ResourceName imageOrImageset, string iconName, Managed data = null)
	{
        if (m_ListBoxComponent)
		{
			SCR_ListBoxElementComponent comp;
			m_ListBoxComponent.AddItemAndIcon(name, imageOrImageset, iconName, comp, data);
		}
	}
	
	void SetTitle(string title)
	{
		if (m_wTitle)
			m_wTitle.SetText(title);
	}
	
	int getSelectedListBox()
	{
		if (!m_ListBoxComponent)
			return -1;	
		
		return m_ListBoxComponent.GetSelectedItem();
	}
	
	Managed getDataForListboxItem(int itemID)
	{
		if (!m_ListBoxComponent)
			return null;
		
		return m_ListBoxComponent.GetItemData(itemID);
	}
	
	void SetHoveredEntity(SCR_EditableEntityComponent owner)
	{
		m_HoveredEntity = owner;
	}
	
	ScriptInvoker GetOnConfirm()
	{
		return m_OnConfirm;
	} 
	
	ScriptInvoker GetOnCancel()
	{
		return m_OnCancel;
	} 
	
	//------------------------------------------------------------------------------------------------
	protected void OnConfirm()
	{
		m_OnConfirm.Invoke(m_HoveredEntity, m_ListBoxComponent);
		CloseAnimated();
	}

	//------------------------------------------------------------------------------------------------
	protected void OnCancel()
	{
		m_OnCancel.Invoke(m_HoveredEntity, m_ListBoxComponent);
		CloseAnimated();
	}
	
	//------------------------------------------------------------------------------------------------
	//! animates dialog closure
	void CloseAnimated()
	{
		AnimateWidget.Opacity(GetRootWidget(), 0, m_fAnimationRate);
		int delay;
		if (m_fAnimationRate > 0)
			delay = 1 / m_fAnimationRate * 1000;
		GetGame().GetCallqueue().CallLater(Close, delay);
	}
}