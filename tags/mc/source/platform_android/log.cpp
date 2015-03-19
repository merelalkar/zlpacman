/*
 * Log.cpp
 *
 *  Created on: 25.07.2014
 *      Author: DNS
 */
#include "Log.h"

#include <stdarg.h>
#include <android/log.h>

namespace packt
{
	const char Log::k_tagCommon[] = "PEGAS_COMMON";
	const char Log::k_tagLoop[] = "PEGAS_LOOP";
	const char Log::k_tagInput[] = "PEGAS_INPUT";

	void Log::info(const char* pTag, const char* pMessage, ...)
	{
		va_list args;
		va_start(args, pMessage);

		__android_log_vprint(ANDROID_LOG_INFO, pTag, pMessage, args);
		__android_log_print(ANDROID_LOG_INFO, pTag,  "\n");

		va_end(args);
	}

	void Log::debug(const char* pTag, const char* pMessage, ...)
	{
		va_list args;
		va_start(args, pMessage);

		__android_log_vprint(ANDROID_LOG_DEBUG, pTag, pMessage, args);
		__android_log_print(ANDROID_LOG_DEBUG, pTag,  "\n");

		va_end(args);
	}

	void Log::error(const char* pTag, const char* pMessage, ...)
	{
		va_list args;
		va_start(args, pMessage);

		__android_log_vprint(ANDROID_LOG_ERROR, pTag, pMessage, args);
		__android_log_print(ANDROID_LOG_ERROR, 	pTag,  "\n");

		va_end(args);
	}

	void Log::warn(const char* pTag, const char* pMessage, ...)
	{
		va_list args;
		va_start(args, pMessage);

		__android_log_vprint(ANDROID_LOG_WARN, pTag, pMessage, args);
		__android_log_print(ANDROID_LOG_WARN, pTag,  "\n");

		va_end(args);
	}
}


