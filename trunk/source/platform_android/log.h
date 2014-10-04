/*
 * Log.hpp
 *
 *  Created on: 25.07.2014
 *      Author: DNS
 */

#pragma once

namespace packt
{
	class Log
	{
	public:
		static const char k_tagCommon[];
		static const char k_tagLoop[];
		static const char k_tagInput[];

	public:
		static void error(const char* pTag, const char* pMessage, ...);
		static void warn(const char* pTag, const char* pMessage, ...);
		static void info(const char* pTag, const char* pMessage, ...);
		static void debug(const char* pTag, const char* pMessage, ...);
	};
}

#define Pegas_log_warning(...)	packt::Log::warn(Pegas::Log::k_tagCommon, __VA_ARGS__)
#define Pegas_log_error(...) packt::Log::error(Pegas::Log::k_tagCommon, __VA_ARGS__)

#ifndef NDEBUG
	#define Pegas_log_debug(...) packt::Log::debug(packt::Log::k_tagCommon, __VA_ARGS__)
	#define Pegas_log_debug_loop(...) packt::Log::debug(packt::Log::k_tagLoop, __VA_ARGS__)
	#define Pegas_log_debug_input(...) packt::Log::debug(packt::Log::k_tagInput, __VA_ARGS__)
	#define Pegas_log_info(...) packt::Log::info(packt::Log::k_tagCommon, __VA_ARGS__)
	#define Pegas_log_info_loop(...) packt::Log::info(packt::Log::k_tagLoop, __VA_ARGS__)
	#define Pegas_log_info_input(...) packt::Log::info(packt::Log::k_tagInput, __VA_ARGS__)
#else
	#define Pegas_log_debug(...)
	#define Pegas_log_debug_loop(...)
	#define Pegas_log_debug_input(...)
	#define Pegas_log_info(...)
	#define Pegas_log_info_loop(...)
	#define Pegas_log_info_input(...)
#endif


