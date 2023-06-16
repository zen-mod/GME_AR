

modded class SCR_ChimeraCharacter : ChimeraCharacter
{
	[RplRpc(RplChannel.Reliable, RplRcver.Broadcast)]
	protected void ODIN_SetLocalFlag(EntityFlags flags, bool enable)
	{
		if (enable)
			this.SetFlags(flags, enable);
		else 
			this.ClearFlags(flags);
	}
	
	void ODIN_SetGlobalFlag(EntityFlags flags, bool enable)
	{
		Rpc(ODIN_SetLocalFlag, flags, enable);
		ODIN_SetLocalFlag(flags, enable);
	}
};