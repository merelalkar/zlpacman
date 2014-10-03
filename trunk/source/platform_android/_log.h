/*
 * Log.hpp
 *
 *  Created on: 25.07.2014
 *      Author: DNS
 */

#ifndef PEGAS_LOG_H
#define PEGAS_LOG_H

namespace Pegas {
	class Log {
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

#define Pegas_log_warning(...)	Pegas::Log::warn(Pegas::Log::k_tagCommon, __VA_ARGS__)
#define Pegas_log_error(...) Pegas::Log::error(Pegas::Log::k_tagCommon, __VA_ARGS__)

#ifndef NDEBUG
	#define Pegas_log_debug(...) Pegas::Log::debug(Pegas::Log::k_tagCommon, __VA_ARGS__)
	#define Pegas_log_debug_loop(...) Pegas::Log::debug(Pegas::Log::k_tagLoop, __VA_ARGS__)
	#define Pegas_log_debug_input(...) Pegas::Log::debug(Pegas::Log::k_tagInput, __VA_ARGS__)
	#define Pegas_log_info(...) Pegas::Log::info(Pegas::Log::k_tagCommon, __VA_ARGS__)
	#define Pegas_log_info_loop(...) Pegas::Log::info(Pegas::Log::k_tagLoop, __VA_ARGS__)
	#define Pegas_log_info_input(...) Pegas::Log::info(Pegas::Log::k_tagInput, __VA_ARGS__)
#else
	#define Pegas_log_debug(...)
	#define Pegas_log_debug_loop(...)
	#define Pegas_log_debug_input(...)
	#define Pegas_log_info(...)
	#define Pegas_log_info_loop(...)
	#define Pegas_log_info_input(...)
#endif

#endif /* LOG_H */
