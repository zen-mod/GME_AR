[BaseContainerProps()]
class GME_IntelUIInfo : SCR_InventoryUIInfo
{
	//------------------------------------------------------------------------------------------------
	//! Function to override to get custom inventory name
	override string GetInventoryItemDescription(InventoryItemComponent item)
	{
		GME_IntelComponent intelComponent = GME_IntelComponent.Cast(item.GetOwner().FindComponent(GME_IntelComponent));
		if (!intelComponent)
			return super.GetInventoryItemDescription(item);
		
		string description = intelComponent.GetInventoryItemDescription();
		if (description.IsEmpty())
			return super.GetInventoryItemDescription(item);
		
		return description;
	}
}
