#ifndef PEGAS_PLATFORM_WINDOWS_OS_UTILS_H
#define PEGAS_PLATFORM_WINDOWS_OS_UTILS_H
#pragma once

namespace Pegas
{
	class WindowsOSUtils: public OSUtils
	{
	public:
		WindowsOSUtils() {};
		virtual ~WindowsOSUtils() {};

		virtual void showMessage(const String& message, const String& caption);
		virtual void showMessage(RESOURCEID messageID, RESOURCEID captionID);
		virtual void _cdecl debugOutput(const char* message, ...);
		
		virtual bool confirm(const String& message, const String& caption);
		virtual bool confirm(RESOURCEID messageID, RESOURCEID captionID);

		virtual MILLISECONDS getCurrentTime();
	
	private:
		WindowsOSUtils(const WindowsOSUtils& src);
		WindowsOSUtils& operator=(const WindowsOSUtils& src);
	};
}

#endif