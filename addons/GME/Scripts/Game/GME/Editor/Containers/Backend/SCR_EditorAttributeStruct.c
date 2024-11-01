//------------------------------------------------------------------------------------------------
//! Saved data for editor attribute.
//! Add support for string-based attributes
modded class SCR_EditorAttributeStruct: JsonApiStruct
{
	//--- Serialized (names shortened to save memory)
	protected string GME_v0;
	
	//------------------------------------------------------------------------------------------------
	void SCR_EditorAttributeStruct()
	{
		RegV("GME_v0");
	}
	
	//------------------------------------------------------------------------------------------------
	override static void SerializeAttributes(out notnull array<ref SCR_EditorAttributeStruct> outEntries, SCR_EditorAttributeList attributeList = null, Managed item = null)
	{
		super.SerializeAttributes(outEntries, attributeList, item);
		
		foreach (SCR_EditorAttributeStruct entry : outEntries)
		{
			SCR_BaseEditorAttribute attribute = attributeList.GetAttribute(entry.id);
			SCR_BaseEditorAttributeVar var = attribute.ReadVariable(item, null);
			entry.GME_v0 = var.GME_GetString();
		}
	}
	
	//------------------------------------------------------------------------------------------------
	override static void DeserializeAttributes(notnull array<ref SCR_EditorAttributeStruct> entries, SCR_EditorAttributeList attributeList = null, Managed item = null)
	{
		super.DeserializeAttributes(entries, attributeList, item);
		
		foreach (SCR_EditorAttributeStruct entry: entries)
		{
			if (entry.GME_v0.IsEmpty())
				continue;
			
			SCR_BaseEditorAttribute attribute = attributeList.GetAttribute(entry.id);
			SCR_BaseEditorAttributeVar var = SCR_BaseEditorAttributeVar.GME_CreateString(entry.GME_v0);
			attribute.WriteVariable(item, var, null, -1);
		}
	}
}
