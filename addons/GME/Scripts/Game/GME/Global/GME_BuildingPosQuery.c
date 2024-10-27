//------------------------------------------------------------------------------------------------
class GME_BuildingPosQuery : Managed
{
	protected NavmeshWorldComponent m_pNavmesh;
	protected IEntity m_pBuilding;
	protected vector m_vLocalMins, m_vLocalMaxs;
	protected ref array<vector> m_aPositions;
	protected ref array<vector> m_aDirections;
	protected bool m_bIsRunning = false;
	protected int m_iMaxPos;
	protected int m_iMaxAttempts;
	protected int m_iAttempt;
	protected vector m_vCurrentQueryPos;
	protected static const float QUERY_RADIUS = 3.5;
	protected static const float ATTEMPT_TIMEOUT = 0;
	protected static const float EYE_HEIGHT = 1.53;
	protected static const float LOS_TRACER_LENGTH = 25;
	// We make sure that doors and windows are ignored for the LoS tracer checks
	protected static const ref array<ResourceName> LOS_TRACER_EXCLUDED_PREFABS = {
		"{F1793FE006FDF888}Prefabs/Structures/BuildingParts/Doors/Door_Base.et",
		"{2B188379767C8461}Prefabs/Structures/Core/DestructibleWindow_Base.et",
		"{86834A0D5920F32F}Prefabs/Structures/Core/DestructibleGlass_Base.et",
	};
	protected ref ScriptInvoker m_OnQueryDone;
	
	//------------------------------------------------------------------------------------------------
	void GME_BuildingPosQuery(IEntity building)
	{
		m_pBuilding = building;
		m_pBuilding.GetBounds(m_vLocalMins, m_vLocalMaxs);
		m_pNavmesh = GetGame().GetAIWorld().GetNavmeshWorldComponent("Soldiers");
	}
	
	//------------------------------------------------------------------------------------------------
	//! Computes random building positions
	//! maxPos is the maximum number of positions to generate
	//! maxAttempts is the maximum number of attempts to find positions
	void RunRandomQueries(int maxPos, int maxAttempts = 1000)
	{
		m_iMaxPos = maxPos;
		m_iMaxAttempts = maxAttempts;
		m_bIsRunning = true;
		m_iAttempt = 0;
		m_aPositions = {};
		m_aDirections = {};
		m_vCurrentQueryPos = GetRandomPosInBounds();
		GetGame().GetCallqueue().CallLater(RandomQueryStep, ATTEMPT_TIMEOUT, true);
	}
	
	//------------------------------------------------------------------------------------------------
	protected void RandomQueryStep()
	{
		vector outPos, outDir;
		GME_EBuildingPosQueryState result = QueryPos(m_vCurrentQueryPos, outPos, outDir);
		switch (result)
		{
			case GME_EBuildingPosQueryState.SUCCESS:
			{
				m_aPositions.Insert(outPos);
				m_aDirections.Insert(outDir);
				m_vCurrentQueryPos = GetRandomPosInBounds();
				break;
			}
			
			case GME_EBuildingPosQueryState.FAIL:
			{
				m_vCurrentQueryPos = GetRandomPosInBounds();
				break;
			}
		}
		
		m_iAttempt++;
		
		if (m_iAttempt >= m_iMaxAttempts || m_aPositions.Count() >= m_iMaxPos)
		{
			GetGame().GetCallqueue().Remove(RandomQueryStep);
			m_bIsRunning = false;
			
			if (m_OnQueryDone)
				m_OnQueryDone.Invoke(m_aPositions, m_aDirections);
		}
	}
	
	//------------------------------------------------------------------------------------------------
	protected vector GetRandomPosInBounds()
	{
		vector localPos;
		for (int i = 0; i < 3; i++)
		{
			localPos[i] = Math.RandomFloatInclusive(m_vLocalMins[i], m_vLocalMaxs[i]);
		};
		
		return m_pBuilding.CoordToParent(localPos);
	}
	
	//------------------------------------------------------------------------------------------------
	//! Return true if a position was found
	protected GME_EBuildingPosQueryState QueryPos(vector queryPos, out vector outPos, out vector outDir)
	{
		// Make sure the navmesh tile is loaded
		if (m_pNavmesh.IsTileRequested(queryPos))
			return GME_EBuildingPosQueryState.RUNNING;
		
		if (!m_pNavmesh.IsTileLoaded(queryPos))
		{
			m_pNavmesh.LoadTileIn(queryPos);
			return GME_EBuildingPosQueryState.RUNNING;
		};
		
		// Get closest point on navmesh
		if (!m_pNavmesh.GetReachablePoint(queryPos, QUERY_RADIUS, outPos))
			return GME_EBuildingPosQueryState.FAIL;
		
		// Tracer check that the floor belongs to the building
		TraceParam params = new TraceParam();
		params.Flags = TraceFlags.ENTS;
		params.Start = outPos + EYE_HEIGHT * vector.Up;
		params.End = outPos - 5 * vector.Up;
		
		if (GetGame().GetWorld().TraceMove(params, null) >= 0.999)
			return GME_EBuildingPosQueryState.FAIL;
		
		if (params.TraceEnt.GetRootParent() != m_pBuilding.GetRootParent())
			return GME_EBuildingPosQueryState.FAIL;
		
		// If the surface is not flat, we only accept positions inside the building,
		// so that we don't end up on top of a gable roof
		if (params.TraceNorm[1] < 0.999)
		{
			// Tracer check that we have a roof over us
			params.End = outPos + 15 * vector.Up;
			
			if (GetGame().GetWorld().TraceMove(params, null) >= 0.999)
				return GME_EBuildingPosQueryState.FAIL;
			
			if (params.TraceEnt.GetRootParent() != m_pBuilding.GetRootParent())
				return GME_EBuildingPosQueryState.FAIL;
		}
		
		outDir = vector.FromYaw(ComputeBestLoSYaw(outPos));
		return GME_EBuildingPosQueryState.SUCCESS;
	}
	
	//------------------------------------------------------------------------------------------------
	//! Does 360 tracer checks and returns the yaw angle with the best LoS
	protected float ComputeBestLoSYaw(vector pos)
	{
		vector eyePos = pos + EYE_HEIGHT * vector.Up;
		
		array<float> yaws = {};
		yaws.Reserve(36);
		
		for (int y = 0; y < 360; y += 10)
		{
			yaws.Insert(y);
		}
		
		// If there are multiple best candidates, we make sure that they are randomly selected
		SCR_ArrayHelperT<float>.Shuffle(yaws);
		
		float bestDistance = 0;
		float bestYaw = 0;
		TraceParam params = new TraceParam();
		params.Flags = TraceFlags.ENTS;
		params.Start = eyePos;
		
		foreach (float yaw : yaws)
		{
			float distance = LOS_TRACER_LENGTH;
			params.End = eyePos + LOS_TRACER_LENGTH * vector.FromYaw(yaw);
			float percentage = GetGame().GetWorld().TraceMove(params, LoSTracerEntityCallback);
			
			if (percentage < 1)
			{
				distance *= percentage;
			}
			
			if (distance > bestDistance)
			{
				bestDistance = distance;
				bestYaw = yaw;
			}
				
		}
		
		return bestYaw;
	}
	
	//------------------------------------------------------------------------------------------------
	protected bool LoSTracerEntityCallback(IEntity entity)
	{
		EntityPrefabData data = entity.GetPrefabData();
		if (!data)
			return false;
		
		BaseContainer container = data.GetPrefab();
		
		foreach (ResourceName res : LOS_TRACER_EXCLUDED_PREFABS)
		{
			if (SCR_BaseContainerTools.IsKindOf(container, res))
				return false;
		}
		
		return true;
	}
	
	//------------------------------------------------------------------------------------------------
	bool IsRunning()
	{
		return m_bIsRunning;
	}
	
	//------------------------------------------------------------------------------------------------
	array<vector> GetPositions()
	{
		return m_aPositions;
	}
	
	//------------------------------------------------------------------------------------------------
	array<vector> GetDirections()
	{
		return m_aDirections;
	}
	
	//------------------------------------------------------------------------------------------------
	ScriptInvoker GetOnQueryDone()
	{
		if (!m_OnQueryDone)
			m_OnQueryDone = new ScriptInvoker();
		
		return m_OnQueryDone;
	}
}

//------------------------------------------------------------------------------------------------
enum GME_EBuildingPosQueryState
{
	SUCCESS,
	FAIL,
	RUNNING
}
