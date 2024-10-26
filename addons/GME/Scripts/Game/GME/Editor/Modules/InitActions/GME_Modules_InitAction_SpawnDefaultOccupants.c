//------------------------------------------------------------------------------------------------
class GME_Modules_InitAction_SpawnDefaultOccupants : GME_Modules_InitAction_Base
{
	[Attribute(desc: "Getter on the module for the vehicle to occupy")]
	protected string m_sVehicleToOccupyGetter;
	
	[Attribute(desc: "Setter on the module for the spawned occupants group")]
	protected string m_sOccupantsGroupSetter;
	protected SCR_AIGroup m_pOccupantsGroup;
	
	[Attribute(desc: "Compartment types to occupy")]
	protected ref array<ECompartmentType> m_aCompartmentTypes;
	
	[Attribute(defvalue: "false", desc: "Enable AI LOD of the crew. Set false if you want the vehicle to move independent of whether players are nearby")]
	protected bool m_bEnableAILODs;
		
	protected SCR_BaseCompartmentManagerComponent m_pCompartmentManager;
	
	//------------------------------------------------------------------------------------------------
	override void OnStartServer()
	{
		IEntity vehicleToOccupy = IEntity.Cast(m_pModule.CallModuleMethod(m_sVehicleToOccupyGetter));
		m_pOccupantsGroup = GME_VehicleHelper.SpawnCrew(Vehicle.Cast(vehicleToOccupy));
		m_pModule.CallModuleMethod(m_sOccupantsGroupSetter, m_pOccupantsGroup);
		GME_GroupHelper.EnableAILODs(m_pOccupantsGroup, m_bEnableAILODs);
		m_pModule.OnInitActionCompleted();
	}
	
	//------------------------------------------------------------------------------------------------
	override void OnInitCanceledServer()
	{
		if (m_pOccupantsGroup)
			SCR_EntityHelper.DeleteEntityAndChildren(m_pOccupantsGroup);
	}
}
