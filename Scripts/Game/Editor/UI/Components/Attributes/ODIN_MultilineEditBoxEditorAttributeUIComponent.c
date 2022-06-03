class ODIN_MultilineEditboxEditorAttributeUIComponent: SCR_BaseEditorAttributeUIComponent
{
	protected ODIN_MultilineEditBoxComponent m_EditBoxComponent;
	
	override void Init(Widget w, SCR_BaseEditorAttribute attribute)
	{
		Widget editboxWidget = w.FindAnyWidget(m_sUiComponentName);
		if (!editboxWidget) 
			return;
		
		m_EditBoxComponent = ODIN_MultilineEditBoxComponent.Cast(editboxWidget.FindHandler(ODIN_MultilineEditBoxComponent));
		if (!m_EditBoxComponent) 
			return;

		m_EditBoxComponent.m_OnChanged.Insert(OnChangeEditbox);

		super.Init(w, attribute);
	}
	
	//Sets a default state for the UI and var value if conflicting attribute
	override void SetVariableToDefaultValue(SCR_BaseEditorAttributeVar var)
	{
		m_EditBoxComponent.SetValue("");
		
		if (!var) return;
		
		ODIN_StringEditorAttributeVar stringVar = ODIN_StringEditorAttributeVar.Cast(var);
		
		stringVar.SetString("");
	}
	
	override void SetFromVar(SCR_BaseEditorAttributeVar var)
	{	
		super.SetFromVar(var);
		
		if (!var) return;
		
		ODIN_StringEditorAttributeVar stringVar = ODIN_StringEditorAttributeVar.Cast(var);
		
		m_EditBoxComponent.SetValue(stringVar.GetString());
	}
	
	protected void OnChangeEditbox(ODIN_MultilineEditBoxComponent selectionBox, string value)
	{
		OnChange(null, 0, 0, false);
	}
	
	override bool OnChange(Widget w, int x, int y, bool finished)
	{	
		if (!m_EditBoxComponent) 
			return false;
		
		string newValue = m_EditBoxComponent.GetValue();
		
		SCR_BaseEditorAttribute attribute = GetAttribute();
		if (!attribute) return false;

		SCR_BaseEditorAttributeVar var = attribute.GetVariable(true);
		ODIN_StringEditorAttributeVar stringVar = ODIN_StringEditorAttributeVar.Cast(var);
		
		if (stringVar.GetString() == newValue)
			return false;
		
		stringVar.SetString(newValue);
		super.OnChange(w, x, y, finished);
		return false;
	}	
	
	override void HandlerDeattached(Widget w)
	{	
		if (m_EditBoxComponent)
			m_EditBoxComponent.m_OnChanged.Remove(OnChangeEditbox);		
	}
};