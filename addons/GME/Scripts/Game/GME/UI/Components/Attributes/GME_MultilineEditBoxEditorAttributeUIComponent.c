//------------------------------------------------------------------------------------------------
class GME_MultilineEditboxEditorAttributeUIComponent: SCR_BaseEditorAttributeUIComponent
{
	protected GME_MultilineEditBoxComponent m_EditBoxComponent;
	
	//------------------------------------------------------------------------------------------------
	override void Init(Widget w, SCR_BaseEditorAttribute attribute)
	{
		Widget editboxWidget = w.FindAnyWidget(m_sUiComponentName);
		if (!editboxWidget) 
			return;
		
		m_EditBoxComponent = GME_MultilineEditBoxComponent.Cast(editboxWidget.FindHandler(GME_MultilineEditBoxComponent));
		if (!m_EditBoxComponent) 
			return;

		m_EditBoxComponent.m_OnChanged.Insert(OnChangeEditbox);

		super.Init(w, attribute);
	}
	
	//------------------------------------------------------------------------------------------------
	//! Sets a default state for the UI and var value if conflicting attribute
	override void SetVariableToDefaultValue(SCR_BaseEditorAttributeVar var)
	{
		m_EditBoxComponent.SetValue("");
		
		if (!var)
			return;
				
		var.GME_SetString("");
	}
	
	//------------------------------------------------------------------------------------------------
	override void SetFromVar(SCR_BaseEditorAttributeVar var)
	{	
		super.SetFromVar(var);
		
		if (!var)
			return;
				
		m_EditBoxComponent.SetValue(var.GME_GetString());
	}
	
	//------------------------------------------------------------------------------------------------
	protected void OnChangeEditbox(GME_MultilineEditBoxComponent selectionBox, string value)
	{
		OnChange(null, false);
	}
	
	//------------------------------------------------------------------------------------------------
	override bool OnChange(Widget w, bool finished)
	{	
		if (!m_EditBoxComponent) 
			return false;
		
		string newValue = m_EditBoxComponent.GetValue();
		
		SCR_BaseEditorAttribute attribute = GetAttribute();
		if (!attribute) return false;

		SCR_BaseEditorAttributeVar var = attribute.GetVariable(true);
		
		if (var.GME_GetString() == newValue)
			return false;
		
		var.GME_SetString(newValue);
		super.OnChange(w, finished);
		return false;
	}	
	
	//------------------------------------------------------------------------------------------------
	override void HandlerDeattached(Widget w)
	{	
		if (m_EditBoxComponent)
			m_EditBoxComponent.m_OnChanged.Remove(OnChangeEditbox);		
	}
}
