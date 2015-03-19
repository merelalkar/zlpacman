#include "platform_windows.h"
using namespace Pegas;


void WindowsOSUtils::showMessage(const String& message, const String& caption)
{
	MessageBox(0, message.c_str(), caption.c_str(), MB_OK|MB_ICONINFORMATION);
}

void WindowsOSUtils::showMessage(RESOURCEID messageID, RESOURCEID captionID)
{
	StringResourceManager& stringResorceManager = StringResourceManager::getInstance();
	
	showMessage(stringResorceManager.getResource(messageID)->getString(), 
		stringResorceManager.getResource(captionID)->getString());
}
		
bool WindowsOSUtils::confirm(const String& message, const String& caption)
{
	return (MessageBox(0, message.c_str(), caption.c_str(), 
		MB_OK|MB_ICONQUESTION) == IDOK);
}

bool WindowsOSUtils::confirm(RESOURCEID messageID, RESOURCEID captionID)
{
	StringResourceManager& stringResorceManager = StringResourceManager::getInstance();

	return confirm(stringResorceManager.getResource(messageID)->getString(), 
		stringResorceManager.getResource(captionID)->getString());
}

MILLISECONDS WindowsOSUtils::getCurrentTime()
{
	return timeGetTime();
}

void _cdecl WindowsOSUtils::debugOutput(const char* message, ...)
{
	char buf[512];

	va_list argList;
	va_start(argList, message);

    _vsnprintf(buf, sizeof(buf)-1, message, argList);
	va_end(argList);

	OutputDebugStringA(buf);
	OutputDebugStringA("\n");
}