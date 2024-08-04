//------------------------------------------------------------------------------------------------
class GME_CompareEditableEntityDisplayName : SCR_SortCompare<SCR_EditableEntityComponent>
{
	//-------------------------------------------------------------------------------------------
	override static int Compare(SCR_EditableEntityComponent left, SCR_EditableEntityComponent right)
	{
		if (!left || !right)
			return -1;
		
		string name1 = left.GetInfo().GetName();
		string name2 = right.GetInfo().GetName();
		
		if (name1.Compare(name2) == -1)
			return 1;
		else
			return 0;
	}
}
