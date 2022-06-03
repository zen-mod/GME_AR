//------------------------------------------------------------------------------------------------
class ODIN_MultilineEditBoxComponent : SCR_ChangeableComponentBase 
{
	[Attribute("0.1")]
	protected float m_fColorsAnimationTime;
	
	[Attribute("0")]
	protected bool m_bShowWriteIcon;
	
	[Attribute("", UIWidgets.ResourcePickerThumbnail, "Image resource for icons")]
	protected ResourceName m_sIconImageSet;
	
	[Attribute("")]
	protected string m_sIconImageName;
	
	[Attribute("0.1", "0 1")]
	protected float m_fOpacityDefault;
	
	[Attribute("0.4", "0 1")]
	protected float m_fOpacityFocused;
	
	MultilineEditBoxWidget m_wEditBox;
	Widget m_wColorOverlay;
	Widget m_wEditBackground;
	Widget m_wWarningIcon;
	Widget m_wHorizontalLayout;
	
	Widget m_wWriteIconScale;
	ImageWidget m_wImgWriteIcon;
	
	protected bool m_bValidInput = true;
	protected bool m_bIsTyping;
	protected ref Color COLOR_INVALID_INPUT = UIColors.WARNING;
	protected ref Color COLOR_VALID_INPUT 	= Color.White;
	
	// Arguments passed: SCR_EditBoxComponent, string (text)
	ref ScriptInvoker m_OnConfirm = new ref ScriptInvoker();
	ref ScriptInvoker m_OnFocusChangedEditBox = new ref ScriptInvoker();
	//------------------------------------------------------------------------------------------------
	override void HandlerAttached(Widget w)
	{
		super.HandlerAttached(w);
		m_wColorOverlay = w.FindAnyWidget("OverlayPanel");
		m_wEditBox = MultilineEditBoxWidget.Cast(w.FindAnyWidget("EditBox"));
		m_wWarningIcon = w.FindAnyWidget("ErrorIcon");
		m_wEditBackground = w.FindAnyWidget("EditBackground");
		m_wHorizontalLayout = w.FindAnyWidget("HorizontalLayout");
		
		m_wWriteIconScale = w.FindAnyWidget("WriteIconScale");
		m_wImgWriteIcon = ImageWidget.Cast(w.FindAnyWidget("WriteIcon"));
		
		if (m_wWarningIcon)
		{
			if (m_bUseLabel)
				m_wWarningIcon.SetOpacity(0);
			else
				m_wWarningIcon.SetOpacity(0); // Without label, warning icon not shown at all
		}
		
		// Setup for label-less edit boxes
		m_wBackground.SetVisible(m_bUseLabel);
		
		// Convert to transition rate
		if (m_fColorsAnimationTime <= 0)
			m_fColorsAnimationTime = 1000;
		else
			m_fColorsAnimationTime = 1 / m_fColorsAnimationTime;
		
		if (!m_wEditBox)
			return;

		EditBoxFilterComponent filter = EditBoxFilterComponent.Cast(m_wEditBox.FindHandler(EditBoxFilterComponent));
		if (filter)
		{
			filter.m_OnInvalidInput.Insert(OnInvalidInput);
			filter.m_OnTextTooLong.Insert(OnInvalidInput);
			filter.m_OnValidInput.Insert(OnValueChanged);
		}
		
		SCR_EventHandlerComponent handler = SCR_EventHandlerComponent.Cast(m_wEditBox.FindHandler(SCR_EventHandlerComponent));
		if (handler)
		{
			handler.GetOnFocus().Insert(OnHandlerFocus);
			handler.GetOnFocusLost().Insert(OnHandlerFocusLost);
			handler.GetOnChangeFinal().Insert(OnConfirm);
		}
		
		if (m_wEditBackground)
			m_wEditBackground.SetOpacity(m_fOpacityDefault);
		
		// Image image 
		if (m_wImgWriteIcon && m_wWriteIconScale)
		{
			bool hasResource = !m_sIconImageSet.IsEmpty() && !m_sIconImageName.IsEmpty() && m_bShowWriteIcon;
			m_wWriteIconScale.SetVisible(hasResource);
			
			if (hasResource)
				m_wImgWriteIcon.LoadImageFromSet(0, m_sIconImageSet, m_sIconImageName);
		}
	}
	
	//------------------------------------------------------------------------------------------------
	override bool OnFocus(Widget w, int x, int y)
	{
		// Set focus to handler - super.OnFocus will be called only after the handler is focused!
		GetGame().GetWorkspace().SetFocusedWidget(m_wEditBox);
		m_bIsTyping = true;
		return true;
	}

	//------------------------------------------------------------------------------------------------
	void OnValueChanged()
	{
		m_OnChanged.Invoke(this, m_wEditBox.GetText());

		if (m_bValidInput)
			return;
		
		WidgetAnimator.PlayAnimation(m_wWarningIcon, WidgetAnimationType.Opacity, 0, m_fColorsAnimationTime);
		WidgetAnimator.PlayAnimation(m_wColorOverlay, WidgetAnimationType.Color, COLOR_VALID_INPUT, m_fColorsAnimationTime);
		m_bValidInput = true;
	}
	
	//------------------------------------------------------------------------------------------------
	void OnHandlerFocus()
	{
		// Call focus event on parent class
		super.OnFocus(m_wRoot, 0, 0);
		WidgetAnimator.PlayAnimation(m_wBackground, WidgetAnimationType.Opacity, m_fOpacityFocused, m_fAnimationRate);
		m_bIsTyping = true;
		
		// Make the widget unfocusable
		m_wRoot.SetFlags(WidgetFlags.NOFOCUS);
		m_OnFocusChangedEditBox.Invoke(this, m_wEditBox, true);
	}
	
	//------------------------------------------------------------------------------------------------
	void OnHandlerFocusLost()
	{
		// Call focusLost event on parent class
		super.OnFocusLost(m_wRoot, 0, 0);
		WidgetAnimator.PlayAnimation(m_wBackground, WidgetAnimationType.Opacity, m_fOpacityDefault, m_fAnimationRate);
		m_bIsTyping = false;
		
		// Make focusable again
		m_wRoot.ClearFlags(WidgetFlags.NOFOCUS);
		
		m_OnFocusChangedEditBox.Invoke(this, m_wEditBox, false);
	}
	
	//------------------------------------------------------------------------------------------------
	void OnInvalidInput()
	{
		WidgetAnimator.PlayAnimation(m_wWarningIcon, WidgetAnimationType.Opacity, 1, m_fColorsAnimationTime);
		WidgetAnimator.PlayAnimation(m_wColorOverlay, WidgetAnimationType.Color, COLOR_INVALID_INPUT, m_fColorsAnimationTime);
		m_bValidInput = false;
	}
	
	//------------------------------------------------------------------------------------------------
	protected void OnConfirm(Widget w)
	{
		if (m_bIsTyping)
			m_OnConfirm.Invoke(this, GetValue());
	}
	
	//------------------------------------------------------------------------------------------------
	//! Static method to easily find component by providing name and parent.
	//! Searching all children will go through whole hierarchy, instead of immediate chidren
	static SCR_EditBoxComponent GetEditBoxComponent(string name, Widget parent, bool searchAllChildren = true)
	{
		if (!parent || name == string.Empty)
			return null;
		
		Widget w;
		if (searchAllChildren)
			w = parent.FindAnyWidget(name);
		else
			w = parent.FindWidget(name);
		
		if (!w)
			return null;
		
		SCR_EditBoxComponent comp = SCR_EditBoxComponent.Cast(w.FindHandler(SCR_EditBoxComponent));
		return comp;
	}
	
	// User API
	void SetValue(string value) { m_wEditBox.SetText(value); }
	string GetValue() { return m_wEditBox.GetText(); }	
};