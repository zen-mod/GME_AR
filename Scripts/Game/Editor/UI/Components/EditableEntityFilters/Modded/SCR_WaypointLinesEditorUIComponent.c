/*!
Special configuration for drawing waypoint lines that supports cycle waypoint
*/
modded class SCR_WaypointLinesEditorUIComponent: SCR_BaseEditorUIComponent
{	
	override protected void OnMenuUpdate(float timeSlice)
	{
		SCR_EditableEntityComponent child, prevWaypoint;
		SCR_EditableWaypointComponent waypoint;
		vector points[2];
		vector pos1, pos2;
		foreach (SCR_EditableEntityComponent group, int groupTokens: m_Groups)
		{
			if (!group)
				continue;
					
			for (int i = 0, count = group.GetChildrenCount(true); i < count; i++)
			{
				child = group.GetChild(i);
				if (child.GetEntityType() != EEditableEntityType.WAYPOINT)
					continue;
				
				waypoint = SCR_EditableWaypointComponent.Cast(child);
				prevWaypoint = waypoint.GetPrevWaypoint();
								
				// don't draw lines if the waypoint have been completed. Can happen when using cycle waypoint		
				if (!waypoint.GetPos(pos1) || !prevWaypoint.GetPos(pos2) || waypoint.IsCompleted())
					continue;
				
				points = {pos1, pos2};
				Shape.CreateLines(m_iLineColorPacked, m_ShapeFlags, points, 2);
			}
			
		}
		
#ifdef WAYPOINT_LINES_DEBUG
		DbgUI.Begin("");
		DbgUI.Text(m_Groups.Count().ToString());
		DbgUI.End();
#endif
	}
};