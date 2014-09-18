/*
 * Log.cpp
 *
 *  Created on: 25.07.2014
 *      Author: DNS
 */
#include "Log.h"

#include <stdarg.h>
#include <android/log.h>

namespace Pegas
{
	void Log::info(const char* pMessage, ...)
	{
		va_list args;
		va_start(args, pMessage);

		__android_log_vprint(ANDROID_LOG_INFO, "PEGAS", pMessage, args);
		__android_log_print(ANDROID_LOG_INFO, "PEGAS",  "\n");

		va_end(args);
	}

	void Log::error(const char* pMessage, ...)
	{
		va_list args;
		va_start(args, pMessage);

		__android_log_vprint(ANDROID_LOG_ERROR, "PEGAS", pMessage, args);
		__android_log_print(ANDROID_LOG_ERROR, "PEGAS",  "\n");

		va_end(args);
	}

	void Log::debug(const char* pMessage, ...)
	{
		va_list args;
		va_start(args, pMessage);

		__android_log_vprint(ANDROID_LOG_DEBUG, "PEGAS", pMessage, args);
		__android_log_print(ANDROID_LOG_DEBUG, "PEGAS",  "\n");

		va_end(args);
	}

	void Log::warn(const char* pMessage, ...)
	{
		va_list args;
		va_start(args, pMessage);

		__android_log_vprint(ANDROID_LOG_WARN, "PEGAS", pMessage, args);
		__android_log_print(ANDROID_LOG_WARN, "PEGAS",  "\n");

		va_end(args);
	}
}


