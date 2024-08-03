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
	
	[Attribute(defvalue: "-1", desc: "AI LOD of the crew")]
	protected int m_iPermanentAILOD;
		
	protected SCR_BaseCompartmentManagerComponent m_pCompartmentManager;
	
	//------------------------------------------------------------------------------------------------
	override void OnInitServer()
	{
		m_pModule.RunInitActionServer();
	}
	
	//------------------------------------------------------------------------------------------------
	override void RunServer(array<IEntity> params = null)
	{
		IEntity vehicleToOccupy = m_pModule.GetPlacingParamServer(m_sVehicleToOccupyGetter);	
		m_pCompartmentManager = SCR_BaseCompartmentManagerComponent.Cast(vehicleToOccupy.FindComponent(SCR_BaseCompartmentManagerComponent));
		m_pCompartmentManager.GetOnDoneSpawningDefaultOccupants().Insert(OnOccupantsSpawned);
		m_pCompartmentManager.SpawnDefaultOccupants(m_aCompartmentTypes);
	}
	
	//------------------------------------------------------------------------------------------------
	protected void OnOccupantsSpawned(SCR_BaseCompartmentManagerComponent compartmentManager = null, array<IEntity> spawnedCharacters = null, bool wasCanceled = false)
	{
		foreach (IEntity member : spawnedCharacters)
		{
			AIComponent aiComponent = AIComponent.Cast(member.FindComponent(AIComponent));
			AIAgent agent = aiComponent.GetAIAgent();
			agent.SetPermanentLOD(m_iPermanentAILOD);
			
			if (!m_pOccupantsGroup)
			{
				m_pOccupantsGroup = SCR_AIGroup.Cast(agent.GetParentGroup());
				m_pOccupantsGroup.SetPermanentLOD(0);
				m_pModule.SetPlacingParamServer(m_sOccupantsGroupSetter, m_pOccupantsGroup);
			}
		}
		
		m_pCompartmentManager.GetOnDoneSpawningDefaultOccupants().Remove(OnOccupantsSpawned);
		m_pModule.OnInitActionCompleted();
	}
	
	//------------------------------------------------------------------------------------------------
	override void OnCancelServer()
	{
		if (m_pOccupantsGroup)
			SCR_EntityHelper.DeleteEntityAndChildren(m_pOccupantsGroup);
	}
}
