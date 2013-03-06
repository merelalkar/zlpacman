#ifndef PEGAS_UTILS_H
#define PEGAS_UTILS_H
#pragma once
#pragma warning(disable : 4355)

namespace Pegas
{
	class OSUtils: public Singleton<OSUtils>
	{
	public:
		OSUtils(): Singleton(*this) {};
		virtual ~OSUtils() {};

		virtual void showMessage(const String& message, const String& caption) = 0;
		virtual void showMessage(RESOURCEID messageID, RESOURCEID captionID) = 0;
		virtual void _cdecl debugOutput(const char* message, ...) = 0;
		
		virtual bool confirm(const String& message, const String& caption) = 0;
		virtual bool confirm(RESOURCEID messageID, RESOURCEID captionID) = 0;

		virtual MILLISECONDS getCurrentTime() = 0;

	private:
		OSUtils(const OSUtils& src);
		OSUtils& operator=(const OSUtils& src);
	};
}

#endif //PEGAS_UTILS_H