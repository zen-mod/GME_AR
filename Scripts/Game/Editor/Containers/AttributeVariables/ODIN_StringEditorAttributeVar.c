class ODIN_StringEditorAttributeVar: SCR_BaseEditorAttributeVar
{
	static const int MAX_STRING_LENGTH = 1000;
	protected string m_sValue;
	
	string GetString()
	{
		return m_sValue;
	}
	void SetString(string value)
	{
		m_sValue = value;
	}
	
	//--- Network functions
	override bool ModdedPropCompare(SSnapSerializerBase snapshot) 
	{
		return snapshot.Compare(m_sValue, MAX_STRING_LENGTH);
	}
	override bool ModdedExtract(SSnapSerializerBase snapshot) 
	{
		snapshot.SerializeBytes(m_sValue, MAX_STRING_LENGTH);
		return true;
	}
	override bool ModdedInject(SSnapSerializerBase snapshot) 
	{
		return ModdedExtract(snapshot);
	}

	static ODIN_StringEditorAttributeVar CreateString(string value)
	{
		ODIN_StringEditorAttributeVar var = new ODIN_StringEditorAttributeVar();
		var.SetString(value);
		return var;
	}
};