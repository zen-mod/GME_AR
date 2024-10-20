//------------------------------------------------------------------------------------------------
class GME_AmbientFlybyComponentClass : ScriptComponentClass
{
}

//------------------------------------------------------------------------------------------------
class GME_AmbientFlybyComponent : ScriptComponent
{
	[Attribute(defvalue: "30", desc: "Speed in m/s")]
	protected float m_fSpeed;
	
	[Attribute(defvalue: "6000", desc: "Distance from starting position for triggering despawn in meters")]
	protected float m_fDespawnDistance;
	
	protected Physics m_Physics;
	protected vector m_vVelocity;
	protected vector m_vStartPos;
	protected ref array<AIAgent> m_aAgents = {};
	protected SCR_CharacterControllerComponent m_pPilotController;
	protected ref array<SCR_RotorDamageManagerComponent> m_aRotorDamageManagers = {};
	
	protected ref ScriptInvoker m_OnDespawn = new ScriptInvoker();
	
	//------------------------------------------------------------------------------------------------
	override protected void OnPostInit(IEntity owner)
	{
		if (!GetGame().InPlayMode() || !Replication.IsServer())
			return;

		SetEventMask(owner, EntityEvent.SIMULATE);
		m_Physics = owner.GetPhysics();
		m_Physics.SetActive(ActiveState.ACTIVE);
		
		vector transform[4];
		owner.GetWorldTransform(transform);
		m_vStartPos = transform[3];
		m_vVelocity = m_fSpeed * transform[2];
		
		GetGame().GetCallqueue().Call(DelayedInit, owner);
	}
	
	//------------------------------------------------------------------------------------------------
	protected void DelayedInit(IEntity owner)
	{
		SCR_AIGroup crewGroup = GME_VehicleHelper.SpawnCrew(Vehicle.Cast(owner));
		crewGroup.GME_GetOnDoneSpawningMembers().Insert(OnCrewSpawned);
		
		HelicopterControllerComponent ctrl = HelicopterControllerComponent.Cast(owner.FindComponent(HelicopterControllerComponent));
		if (ctrl)
			ctrl.StartEngine();
		
		IEntity child = owner.GetChildren();
		while (child)
		{
			SCR_RotorDamageManagerComponent damageManager = SCR_RotorDamageManagerComponent.Cast(child.FindComponent(SCR_RotorDamageManagerComponent));
			if (damageManager)
			{
				damageManager.GetOnDamageStateChanged().Insert(OnRotorDamageStateChanged);
				m_aRotorDamageManagers.Insert(damageManager);
			}
			
			child = child.GetSibling();
		}
	}
	
	//------------------------------------------------------------------------------------------------
	protected void OnCrewSpawned(SCR_AIGroup group)
	{
		group.GME_GetOnDoneSpawningMembers().Remove(OnCrewSpawned);
		group.GetAgents(m_aAgents);
		m_aAgents.Insert(group);
		
		foreach (AIAgent agent : m_aAgents)
		{
			agent.SetPermanentLOD(0);
		}
		
		SCR_ChimeraCharacter pilot = SCR_ChimeraCharacter.Cast(m_aAgents[0].GetControlledEntity());
		if (!pilot)
			return;
		
		m_pPilotController = SCR_CharacterControllerComponent.Cast(pilot.FindComponent(SCR_CharacterControllerComponent));
		if (!m_pPilotController)
			return;
		
		m_pPilotController.m_OnLifeStateChanged.Insert(OnPilotLifeStateChanged);
	}
	
	//------------------------------------------------------------------------------------------------
	override protected void EOnSimulate(IEntity owner, float timeSlice)
	{
		m_Physics.SetVelocity(m_vVelocity);
		HandleDespawning(owner, timeSlice);
	}
	
	protected const float DISTANCE_CHECK_TIMEOUT = 2;
	protected float m_fDistanceCheckTimer = 0;
	
	//------------------------------------------------------------------------------------------------
	//! Despawns owner when it moved the despawn distance
	protected void HandleDespawning(IEntity owner, float timeSlice)
	{
		m_fDistanceCheckTimer += timeSlice;
		if (m_fDistanceCheckTimer < m_fDistanceCheckTimer)
			return;
		
		m_fDistanceCheckTimer = 0;
		
		if (vector.DistanceXZ(m_vStartPos, owner.GetOrigin()) < m_fDespawnDistance)
			return;
		
		SCR_EntityHelper.DeleteEntityAndChildren(owner);
	}
	
	//------------------------------------------------------------------------------------------------
	//! Terminate simulation when the life state of the pilot changes
	protected void OnPilotLifeStateChanged(ECharacterLifeState previousLifeState, ECharacterLifeState newLifeState)
	{
		TerminateSimulation();
	}
	
	//------------------------------------------------------------------------------------------------
	protected void OnRotorDamageStateChanged(EDamageState state)
	{
		if (state != EDamageState.DESTROYED)
			return;
		
		TerminateSimulation();
	}
	
	//------------------------------------------------------------------------------------------------
	protected void TerminateSimulation()
	{
		foreach (AIAgent agent : m_aAgents)
		{
			if (agent)
				agent.SetPermanentLOD(-1);
		}
		
		ClearEventMask(GetOwner(), EntityEvent.SIMULATE);
		HelicopterControllerComponent ctrl = HelicopterControllerComponent.Cast(GetOwner().FindComponent(HelicopterControllerComponent));
		ctrl.StopEngine();
		
		if (m_pPilotController)
			m_pPilotController.m_OnLifeStateChanged.Remove(OnPilotLifeStateChanged);
		
		foreach (SCR_RotorDamageManagerComponent damageManager : m_aRotorDamageManagers)
		{
			damageManager.GetOnDamageStateChanged().Remove(OnRotorDamageStateChanged);
		}
	}
	
	//------------------------------------------------------------------------------------------------
	void SetSpeed(float speed)
	{
		m_fSpeed = speed;
	}
	
	//------------------------------------------------------------------------------------------------
	void SetDespawnDistance(float distance)
	{
		m_fDespawnDistance = distance;
	}
	
	//------------------------------------------------------------------------------------------------
	void SetDirection(vector direction)
	{
		m_vVelocity = m_fSpeed * direction;
	}
	
	//------------------------------------------------------------------------------------------------
	void SetStartPos(vector pos)
	{
		m_vStartPos = pos;
	}
	
	//------------------------------------------------------------------------------------------------
	vector GetStartPos()
	{
		return m_vStartPos;
	}
	
	//------------------------------------------------------------------------------------------------
	ScriptInvoker GetOnDespawn()
	{
		return m_OnDespawn;
	}
}
