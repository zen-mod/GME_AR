
[WorkbenchPluginAttribute(name: "Deindent Line Plugin", description: "Deindent current line in script editor", shortcut: "shift+tab", wbModules: { "ScriptEditor" }, awesomeFontCode: 0xF121)]
class ODIN_DeIndentPlugin : WorkbenchPlugin
{
	override void Run()
	{		
		ScriptEditor scriptEditor = Workbench.GetModule(ScriptEditor);		
				
		// get line above line
		string lineContent;
		scriptEditor.GetLineText(lineContent);
		
		// check if first characters are tabs
		if (lineContent.StartsWith("\t"))
		{
			lineContent = lineContent.Substring(1,lineContent.Length()-1); 
			scriptEditor.SetLineText(lineContent);
		}
	}
}