//------------------------------------------------------------------------------------------------
modded class SCR_BaseEditorAttributeVar
{
	protected string m_sGME_Value;
	
	//------------------------------------------------------------------------------------------------
	void GME_SetString(string value)
	{
		m_sGME_Value = value;
	}
	
	//------------------------------------------------------------------------------------------------
	string GME_GetString()
	{
		return m_sGME_Value;
	}

	//------------------------------------------------------------------------------------------------
	static SCR_BaseEditorAttributeVar GME_CreateString(string value)
	{
		SCR_BaseEditorAttributeVar var = new SCR_BaseEditorAttributeVar();
		var.GME_SetString(value);
		return var;
	}
	
	//------------------------------------------------------------------------------------------------
	override static bool Extract(SCR_BaseEditorAttributeVar prop, ScriptCtx hint, SSnapSerializerBase snapshot) 
	{
		super.Extract(prop, hint, snapshot);
		snapshot.SerializeString(prop.m_sGME_Value);
		return true;
	}
	
	//------------------------------------------------------------------------------------------------
	override static bool Inject(SSnapSerializerBase snapshot, ScriptCtx hint, SCR_BaseEditorAttributeVar prop) 
	{
		super.Inject(snapshot, hint, prop);
		snapshot.SerializeString(prop.m_sGME_Value);
		return true;
	}
	
	//------------------------------------------------------------------------------------------------
	override static void Encode(SSnapSerializerBase snapshot, ScriptCtx hint, ScriptBitSerializer packet) 
	{
		super.Encode(snapshot, hint, packet);

		string val;
		snapshot.SerializeString(val);
		packet.SerializeString(val);
	}
	
	//------------------------------------------------------------------------------------------------
	override static bool Decode(ScriptBitSerializer packet, ScriptCtx hint, SSnapSerializerBase snapshot) 
	{
		super.Decode(packet, hint, snapshot);
		
		string val;
		packet.SerializeString(val);
		snapshot.SerializeString(val);
		
		return true;
	}
	
	//------------------------------------------------------------------------------------------------
	override static bool SnapCompare(SSnapSerializerBase lhs, SSnapSerializerBase rhs, ScriptCtx hint) 
	{
		if (!super.SnapCompare(lhs, rhs, hint))
			return false;
		
		return lhs.CompareStringSnapshots(rhs);
	}
	
	//------------------------------------------------------------------------------------------------
	override static bool PropCompare(SCR_BaseEditorAttributeVar prop, SSnapSerializerBase snapshot, ScriptCtx hint) 
	{
		if (!super.PropCompare(prop, snapshot, hint))
			return false;
		
		return snapshot.CompareString(prop.m_sGME_Value);
	}
}
