
[WorkbenchPluginAttribute(name: "Code Comment Plugin", description: "Comments or uncomments code in script editor", shortcut: "ctrl+/", wbModules: { "ScriptEditor" }, awesomeFontCode: 0xF121)]
class ODIN_CommentPlugin : WorkbenchPlugin
{
	
	// It doesn't seem like workbench plugins support setting shortcut based on settings, as shortcut can't be changed inside function and attribute can not take a member variable 
	// There seems to be no support from ScriptEditor to be able to get the current selected lines. So no way of applying commenting to multiple lines. 
	// There seems to be no support for managing ctrl+z behaviour, and because we replace/set a line, it seems like a single ctrl+z removes the line completely and it takes a second ctrl+z to go back to the string before
	
	// There doesn't seem to be a way to "load" plugins created other than having it in the workbench folder... so need to add the plugin to the mod... less than ideal and seems counter-intuative for a dev "plugin"
	
	override void Run()
	{		
		ScriptEditor scriptEditor = Workbench.GetModule(ScriptEditor);		
		
		// Check if current line starts with //, then we remove it, otherwise we add it		
		string lineContent;
		scriptEditor.GetLineText(lineContent);
	
		// trimming the line to trim whitespace to test if first text of line is // and thus a comment
		string trimmedLine = lineContent.Trim();
		
		if (trimmedLine.StartsWith("//"))
		{
			// remove first //
			int index = lineContent.IndexOf("//");
			int indexEnd = lineContent.Length();
			
			// As we remove // its -2 at end index
			int indexDiff = indexEnd - index - 2;
			
			// safety check to avoid out-of-bounds substring call
			if ((indexDiff + index + 2) > indexEnd)
			{
				Print("ERROR in CommentPlugin. Tried to access index bigger than text line");
				return;
			}
			
			// part 1 is spaces/indents until we hit first //
			string part1 = lineContent.Substring(0, index);
			// part2 is from index of // to end. 
			string part2 = lineContent.Substring(index+2, indexDiff);

			lineContent = part1 + part2;
		} else 
		{
			// we want to comment, for now ignore indent and comment at first line
			lineContent = "//" + lineContent;
		}		
		scriptEditor.SetLineText(lineContent);
	}
}
