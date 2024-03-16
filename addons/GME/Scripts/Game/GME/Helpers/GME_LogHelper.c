
class GME_LogHelper
{
	// Function to standiardize print messages on errors etc. 
	static void Log(string message, string component = "", LogLevel level = LogLevel.NORMAL)
	{
		string combinedMessage = "GME";
		
		if (component != "")
			combinedMessage += "-" + component;
		
		combinedMessage += ": " + message;
		Print(message, level);
	}
}