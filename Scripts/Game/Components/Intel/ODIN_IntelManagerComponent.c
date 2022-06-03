class ODIN_IntelManagerComponentClass : ScriptComponentClass {};

class ODIN_IntelManagerComponent : ScriptComponent
{	
	[Attribute("", desc: "Title of the intel")]
	protected LocalizedString m_sIntelTitle;
	
	[Attribute("", desc: "Content of the intel")]
	protected LocalizedString m_sIntelContent;

	[Attribute("", desc: "Name of the action")]
	protected LocalizedString m_sActionName;
	
	[Attribute("true", UIWidgets.CheckBox, desc: "Delete the object when the action is completed")]
	protected bool m_bDeleteOnActionCompletion;
	
	[Attribute("", UIWidgets.ResourceNamePicker, desc: "Sound project for the action (acp)")]
	ResourceName m_ActionSoundProject;

	[Attribute("", desc: "Event name in the sound project for the action")]
	string m_ActionSoundEventName;
	
	string GetIntelTitle()
	{
		return m_sIntelTitle;
	}
	
	string GetIntelContent()
	{
		return m_sIntelContent;
	}
	
	string GetActionName()
	{
		return m_sActionName;
	}
	
	void SetIntelTitle(string title)
	{
		m_sIntelTitle = title;
	}
	
	void SetIntelContent(string content)
	{
		m_sIntelContent = content;
	}
	
	void SetActionName(string name)
	{
		m_sActionName = name;
	}
	
	void AddIntel(IEntity user) 
	{
		RplComponent userRpl = RplComponent.Cast(user.FindComponent(RplComponent));
		RplId targetRplId = userRpl.Id();
		RpcDo_AddIntel(targetRplId);
		Rpc(RpcDo_AddIntel, targetRplId);

		IEntity owner = GetOwner();
		if (m_bDeleteOnActionCompletion && owner)
		{
			delete owner;
		};
	}
	
	[RplRpc(RplChannel.Reliable, RplRcver.Broadcast)]
	protected void RpcDo_AddIntel(RplId targetRplId)
	{
		IEntity player = SCR_Global.GetPlayer();
		if (!player) return;
		RplComponent playerRpl = RplComponent.Cast(player.FindComponent(RplComponent));
		if (!player) return;
		if (playerRpl.Id() != targetRplId) return;
		vector mat[4];
		player.GetWorldTransform(mat);
		AudioSystem.PlayEvent(m_ActionSoundProject, m_ActionSoundEventName, mat, new array<string>, new array<float>);
		SCR_HintManagerComponent.GetInstance().ShowCustomHint(m_sIntelContent, m_sIntelTitle, -1);
	}
	
	void ClearIntel()
	{
		m_sIntelTitle = "";
		m_sIntelContent = "";
		m_sActionName = "";
	}
	
	bool HasIntel()
	{
		return !(m_sActionName.IsEmpty() || m_sIntelTitle.IsEmpty() || m_sIntelContent.IsEmpty());
	}
};
