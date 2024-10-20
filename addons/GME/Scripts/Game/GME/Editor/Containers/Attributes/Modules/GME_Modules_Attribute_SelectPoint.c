//------------------------------------------------------------------------------------------------
[BaseContainerProps(), SCR_BaseEditorAttributeCustomTitle()]
class GME_Modules_Attribute_SelectPointBase : SCR_BasePresetsEditorAttribute
{
	protected ref array<SCR_EditableEntityComponent> m_aAllPoints = {};
	
	//------------------------------------------------------------------------------------------------
	override void WriteVariable(Managed item, SCR_BaseEditorAttributeVar var, SCR_AttributesManagerEditorComponent manager, int playerID)
	{
		if (m_aAllPoints.IsEmpty())
			CreatePresets();
	}
	
	//------------------------------------------------------------------------------------------------
	override protected void CreatePresets()
	{
		m_aAllPoints.Clear();
		SCR_EditableEntityCore core = SCR_EditableEntityCore.Cast(SCR_EditableEntityCore.GetInstance(SCR_EditableEntityCore));
		set<SCR_EditableEntityComponent> entities = new set<SCR_EditableEntityComponent>();
		core.GetAllEntities(entities, true);
		
		foreach (SCR_EditableEntityComponent entity : entities)
		{
			if (IsPoint(entity))
				m_aAllPoints.Insert(entity);
		}
		
		SCR_Sorting<SCR_EditableEntityComponent, GME_CompareEditableEntityDisplayName>.HeapSort(m_aAllPoints);
		m_aValues.Clear();
		
		foreach (int i, SCR_EditableEntityComponent point : m_aAllPoints)
		{
			SCR_EditorAttributeFloatStringValueHolder value = new SCR_EditorAttributeFloatStringValueHolder();
			value.SetWithUIInfo(point.GetInfo(), i);
			m_aValues.Insert(value);
		}
	}
	
	//------------------------------------------------------------------------------------------------
	protected bool IsPoint(SCR_EditableEntityComponent entity)
	{
		return false;
	}
}

