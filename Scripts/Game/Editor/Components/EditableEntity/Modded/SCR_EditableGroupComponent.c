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
			// Only delete if there is no waypoint cycle present
			bool has_cycle = false;
			array<AIWaypoint> waypoints = {};
			m_Group.GetWaypoints(waypoints);
			foreach (AIWaypoint waypoint: waypoints)
			{
				if (AIWaypointCycle.Cast(waypoint))
				{
					has_cycle = true;
					break;
				};
			};
			if (!has_cycle)
			{
				waypointCompToRemove.Delete();
			}
		}
		ReindexWaypoints();
	}
};