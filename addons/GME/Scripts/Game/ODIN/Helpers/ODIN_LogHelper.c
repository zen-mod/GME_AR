
class ODIN_LogHelper
{
	// Function to standiardize print messages on errors etc. 
	static void Log(string message, string component = "", LogLevel level = LogLevel.NORMAL)
	{
		string combinedMessage = "ODIN";
		
		if (component != "")
			combinedMessage += "-" + component;
		
		combinedMessage += ": " + message;
		Print(message, level);
	}
}