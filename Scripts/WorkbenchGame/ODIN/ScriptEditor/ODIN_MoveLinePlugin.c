
[WorkbenchPluginAttribute(name: "Move Line Up Plugin", description: "Move line up with ctrl+shift+up in script editor", shortcut: "ctrl+shift+up", wbModules: { "ScriptEditor" }, awesomeFontCode: 0xF121)]
class ODIN_MoveLineUpPlugin : WorkbenchPlugin
{
	override void Run()
	{		
		ScriptEditor scriptEditor = Workbench.GetModule(ScriptEditor);		
		
		// if line index is 0, return as we can't move it further up
		int totalLines = scriptEditor.GetLinesCount();
		int currentLine = scriptEditor.GetCurrentLine();
		
		// due to limitations in engine methods we can't handle up nor down if we are on the very last line... (i.e. missing "insert after" method)
		if (currentLine <= 0 || currentLine >= totalLines-1)
			return;
				
		// get line above line
		string lineContent;
		scriptEditor.GetLineText(lineContent, currentLine-1);
		
		// we need to not touch current line, to keep curser. So we are deleting line above and putting it below us. 
		// insert below us first, to not mess up the indexing for insert
		scriptEditor.InsertLine(lineContent, currentLine+1);
		
		// remove line above 
		scriptEditor.RemoveLine(currentLine-1);
	}
}

[WorkbenchPluginAttribute(name: "Move Line Down Plugin", description: "Move line down with ctrl+shift+down in script editor", shortcut: "ctrl+shift+down", wbModules: { "ScriptEditor" }, awesomeFontCode: 0xF121)]
class ODIN_MoveLineDownPlugin : WorkbenchPlugin
{
	override void Run()
	{		
		ScriptEditor scriptEditor = Workbench.GetModule(ScriptEditor);		
		
		// if line index is 0, return as we can't move it further up
		int totalLines = scriptEditor.GetLinesCount();
		int currentLine = scriptEditor.GetCurrentLine();
		
		if (currentLine <= 0 || currentLine >= totalLines-1)
			return;
				
		// get line below
		string lineContent;
		scriptEditor.GetLineText(lineContent, currentLine+1);
		
		// we need to not touch current line, to keep curser. So we are deleting line below and putting it above us. 
		// remove line below 
		scriptEditor.RemoveLine(currentLine+1);
		
		// insert that line below us
		scriptEditor.InsertLine(lineContent, currentLine);
	}
}
