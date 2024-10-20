//------------------------------------------------------------------------------------------------
class GME_Modules_AmbientFlybyClass : GME_Modules_BaseClass
{
}

//------------------------------------------------------------------------------------------------
class GME_Modules_AmbientFlyby : GME_Modules_Base
{
	[Attribute(uiwidget: UIWidgets.ResourcePickerThumbnail, desc: "Prefab name for the ambient helicopter", params: "et")]
	protected ResourceName m_sAmbientHeloPrefabName;
	
	[Attribute(defvalue: "50", desc: "Speed in m/s")]
	protected float m_fSpeed;
	
	[Attribute(defvalue: "80", desc: "Flying height relative to terrain in meters")]
	protected float m_fHeight;
	
	[Attribute(defvalue: "3000", desc: "Starting distance from player in meters")]
	protected float m_fStartDistance;
	
	protected const float TERRAIN_Y_SCAN_STEP_SIZE = 100;
	
	protected Vehicle m_pAircraft;
	protected GME_AmbientFlybyComponent m_pFlybyComponent;
	
	//------------------------------------------------------------------------------------------------
	void SetSpeed(float speed)
	{
		m_fSpeed = speed;
		
		if (m_pFlybyComponent)
			m_pFlybyComponent.SetSpeed(speed);
	}
	
	//------------------------------------------------------------------------------------------------
	float GetSpeed()
	{
		return m_fSpeed;
	}
	
	//------------------------------------------------------------------------------------------------
	void SetHeight(float height)
	{
		m_fHeight = height;
		
		if (m_pFlybyComponent)
		{
			vector transform[4];
			GetWorldTransform(transform);
			OnMoved(transform, transform);
		}
	}
	
	//------------------------------------------------------------------------------------------------
	float GetHeight()
	{
		return m_fHeight;
	}
	
	//------------------------------------------------------------------------------------------------
	void SetStartDistance(float distance)
	{
		m_fStartDistance = distance;
		
		if (m_pFlybyComponent)
		{
			vector transform[4];
			GetWorldTransform(transform);
			OnMoved(transform, transform);
		}
	}
	
	//------------------------------------------------------------------------------------------------
	float GetStartDistance()
	{
		return m_fStartDistance;
	}
	
	//------------------------------------------------------------------------------------------------
	override void Run()
	{
		EntitySpawnParams params = new EntitySpawnParams();
		params.TransformMode = ETransformMode.WORLD;
		GetWorldTransform(params.Transform);
		ComputeInitialAircraftTransform(params.Transform, params.Transform);
		m_pAircraft = Vehicle.Cast(GetGame().SpawnEntityPrefab(Resource.Load(m_sAmbientHeloPrefabName), null, params));
		m_pFlybyComponent = GME_AmbientFlybyComponent.Cast(m_pAircraft.FindComponent(GME_AmbientFlybyComponent));
		m_pFlybyComponent.SetSpeed(m_fSpeed);
		m_pFlybyComponent.SetDespawnDistance(2 * m_fStartDistance);
		m_pFlybyComponent.GetOnDespawn().Insert(OnAircraftDespawned);
		SCR_EditableEntityComponent editComponent = SCR_EditableEntityComponent.Cast(m_pAircraft.FindComponent(SCR_EditableEntityComponent));
		editComponent.GetOnDeleted().Insert(OnAircraftDespawned);
	}
	
	//------------------------------------------------------------------------------------------------
	protected void ComputeInitialAircraftTransform(vector moduleTransform[4], out vector transform[4])
	{
		transform = moduleTransform;
		vector offset = m_fStartDistance * transform[2];
		transform[3] = transform[3] - offset;
		Math3D.AnglesToMatrix(Vector(offset.ToYaw(), 0, 0), transform);
		
		float maxTerrainY = 0;
		for (int i = 0; i < Math.Ceil(2 * m_fStartDistance / TERRAIN_Y_SCAN_STEP_SIZE); i++)
		{
			maxTerrainY = Math.Max(maxTerrainY, SCR_TerrainHelper.GetTerrainY(transform[3] + i * TERRAIN_Y_SCAN_STEP_SIZE * transform[2]));
		}
		
		transform[3][1] = transform[3][1] + maxTerrainY + m_fHeight;
	}
	
	//------------------------------------------------------------------------------------------------
	override void OnMoved(vector oldTransform[4], vector newTransform[4])
	{
		if (!m_pFlybyComponent)
			return;
		
		float progress = vector.DistanceXZ(m_pAircraft.GetOrigin(), m_pFlybyComponent.GetStartPos());
		
		vector transform[4];
		ComputeInitialAircraftTransform(newTransform, transform);
		m_pFlybyComponent.SetDirection(transform[2]);
		m_pFlybyComponent.SetStartPos(transform[3]);
		
		transform[3] = transform[3] + progress * transform[2];
		m_pAircraft.SetWorldTransform(transform);
	}
	
	//------------------------------------------------------------------------------------------------
	override void OnDelete()
	{
		SCR_EntityHelper.DeleteEntityAndChildren(m_pAircraft);
	}
	
	//------------------------------------------------------------------------------------------------
	void OnAircraftDespawned(IEntity aircraft)
	{
		SCR_EntityHelper.DeleteEntityAndChildren(this);
	}
}
