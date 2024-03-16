[ComponentEditorProps(category: "GameScripted/Editor (Editables)", description: "", icon: "WBData/ComponentEditorProps/componentEditor.png")]
class GME_EditableWaypointCycleComponentClass: SCR_EditableWaypointComponentClass
{
};

/** @ingroup Editable_Entities
*/

/*!
Special configuration for editable waypoint cycle.
*/
class GME_EditableWaypointCycleComponent : SCR_EditableWaypointComponent
{
	override SCR_EditableEntityComponent EOnEditorPlace(out SCR_EditableEntityComponent parent, SCR_EditableEntityComponent recipient, EEditorPlacingFlags flags, bool isQueue, int playerID = 0)
	{
		//--- Add the group's waypoints to the waypoint cycle 
		if (recipient)
		{
			AIWaypointCycle waypointCycle = AIWaypointCycle.Cast(GetOwner());
			AIGroup group = AIGroup.Cast(recipient.GetOwner());
			if (waypointCycle && group)
			{
				array<AIWaypoint> groupWaypoints = {};
				group.GetWaypoints(groupWaypoints);
				waypointCycle.SetWaypoints(groupWaypoints);
			};
		};
		
		return super.EOnEditorPlace(parent, recipient, flags, isQueue, playerID);
	}
	
	override void OnDelete(IEntity owner)
	{
		//--- Delete all waypoints in the cycle that are already completed
		if (m_Group && IsServer())
		{
			AIGroup group = AIGroup.Cast(m_Group.GetOwner());
			AIWaypoint currentWaypoint = group.GetCurrentWaypoint();
			SCR_EditableWaypointComponent currentWaypointComp = SCR_EditableWaypointComponent.Cast(SCR_EditableEntityComponent.GetEditableEntity(currentWaypoint));
			AIWaypointCycle waypointCycle = AIWaypointCycle.Cast(owner);
			array<AIWaypoint> waypoints = {};
			waypointCycle.GetWaypoints(waypoints);
			
			foreach (AIWaypoint waypoint: waypoints)
			{
				if (waypoint == currentWaypoint)
					break;
				if (!waypoint)
					continue;
				SCR_EditableWaypointComponent waypointComp = SCR_EditableWaypointComponent.Cast(SCR_EditableEntityComponent.GetEditableEntity(waypoint));
				waypointComp.Delete();
			};
		};
		
		super.OnDelete(owner);
	}
};