/*!
Special configuration for editable waypoint.
*/

modded class SCR_EditableWaypointComponent : SCR_EditableEntityComponent
{	
	[RplProp()]
	protected bool m_bCompleted = false;
	
	/*!
	Check if the waypoint has been completed. Can be nessecary for cycle behaviour.
	\return True if completed
	*/
	bool IsCompleted()
	{
		return m_bCompleted;
	}
	
	/*!
	Set waypoint as completed. Can be nessecary for cycle behaviour.
	\param explicit set completed status
	*/
	void setCompleted(bool completed = true)
	{
		if (completed)
			m_bCompleted = true;
		else 
			m_bCompleted = false;
	}
}