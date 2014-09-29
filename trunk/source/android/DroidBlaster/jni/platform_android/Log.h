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
		static void error(const char* pMessage, ...);
		static void warn(const char* pMessage, ...);
		static void info(const char* pMessage, ...);
		static void infoLoop(const char* pMessage, ...);
		static void debug(const char* pMessage, ...);
		static void debugLoop(const char* pMessage, ...);
	};
}

#ifndef NDEBUG
	#define Pegas_log_debug(...) Pegas::Log::debug(__VA_ARGS__)
	#define Pegas_log_info(...) Pegas::Log::info(__VA_ARGS__)
	#define Pegas_log_debug_loop(...) Pegas::Log::debugLoop(__VA_ARGS__)
	#define Pegas_log_info_loop(...) Pegas::Log::infoLoop(__VA_ARGS__)
#else
	#define Pegas_log_debug(...)
	#define Pegas_log_info(...)
	#define Pegas_log_debug_loop(...)
	#define Pegas_log_info_loop(...)
#endif

#endif /* LOG_H */
