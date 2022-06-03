modded class SCR_BaseEditorAttributeVar
{
	bool ModdedPropCompare(SSnapSerializerBase snapshot)
	{
		return PropCompare(this, snapshot, null);
	};
	
	bool ModdedExtract(SSnapSerializerBase snapshot)
	{
		return Extract(this, null, snapshot);
	};

	bool ModdedInject(SSnapSerializerBase snapshot)
	{
		return Inject(snapshot, null, this);
	};
};