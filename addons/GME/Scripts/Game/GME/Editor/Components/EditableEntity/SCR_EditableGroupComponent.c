/*!
Special configuration for editable group.
*/
modded class SCR_EditableGroupComponent : SCR_EditableEntityComponent
{	
	override protected void OnWaypointRemoved(AIWaypoint wp)
	{
		//--- Delete waypoints which were not assigned to another group
		SCR_EditableWaypointComponent waypointCompToRemove = SCR_EditableWaypointComponent.Cast(SCR_EditableEntityComponent.GetEditableEntity(wp));
			
		if (waypointCompToRemove && waypointCompToRemove.GetParentEntity() == this)
		{
			bool has_cycle = AIWaypointCycle.Cast(wp);
			
			if (!has_cycle)
				has_cycle = GME_HasWaypointCycle();
				
			// Only delete if there is no waypoint cycle present
			if (!has_cycle)
			{
				waypointCompToRemove.Delete();
			}
		}
		ReindexWaypoints();
	}
	
	protected bool GME_HasWaypointCycle()
	{
		array<AIWaypoint> waypoints = {};
		m_Group.GetWaypoints(waypoints);
		
		foreach (AIWaypoint waypoint : waypoints)
		{
				
			if (AIWaypointCycle.Cast(waypoint))
			{
				return true;
			};
		};
		
		return false;
	}
	
	override protected void OnWaypointAdded(AIWaypoint wp)
	{
		SCR_EditableWaypointComponent waypoint = SCR_EditableWaypointComponent.Cast(SCR_EditableEntityComponent.GetEditableEntity(wp));
		if (waypoint)
		{
			waypoint.setCompleted(false);
			waypoint.SetParentEntity(this);
		}
		ReindexWaypoints();
	}
};