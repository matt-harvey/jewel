/** @file debug_log.hpp
 *
 * @brief Debug logging facilities
 *
 * @author Matthew Harvey
 * @date 01 September 2013
 *
 * Copyright (c) 2013, Matthew Harvey. All rights reserved.
 *
 * @todo Documentation and testing.
 */

#ifndef GUARD_debug_log_hpp
#define GUARD_debug_log_hpp

#include <exception>
#include <iostream>

namespace jewel
{

class Log
{
public:
	enum Level
	{
		trace,
		info,
		debug,
		warning,
		error,
		fatal
	};
	
	static Level threshold()
	{
		return threshold_aux();
	}
	
	static void set_threshold(Level p_level)
	{
		threshold_aux() = p_level;
		return;
	}

	static void flush()
	{
		std::clog.flush();
		return;
	}

private:

	static Level& threshold_aux()
	{
		static Level ret = debug;
		return ret;
	}
};

}  // namespace jewel


#ifdef DEBUG

	#define JEWEL_TEST_FATAL(level) \
			if (level >= jewel::Log::fatal) \
			{ \
				std::clog << "OBITUARY:\t"\
				          << "Application terminated due to Log::fatal." \
				          << std::endl; \
				std::terminate(); \
			}

	#define JEWEL_LOG(level, msg) \
		if (level >= jewel::Log::threshold()) \
		{ \
			std::clog << "SEVERITY:\t" << level \
					  << "\tMESSAGE:\t" msg \
					  << "\n"; \
			JEWEL_TEST_FATAL(level); \
		}

	#define JEWEL_LOG_LOCATION(level) \
		if (level >= jewel::Log::threshold()) \
		{ \
			std::clog << "SEVERITY:\t" << level \
			          << "\tFUNCTION:\t" << __func__ \
			          << "\tFILE:\t" << __FILE__ \
					  << "\tLINE:\t" << __LINE__ \
					  << "\n"; \
			JEWEL_TEST_FATAL(level); \
		} 
	
	#define JEWEL_LOG_VALUE(level, val) \
		if (level >= jewel::Log::threshold()) \
		{ \
			std::clog << "SEVERITY:\t" << level \
			          << "\tEXPRESSION:\t" << #val \
					  << "\tVALUE:\t" << val \
					  << "\n"; \
			JEWEL_TEST_FATAL(level); \
		}

#else

	#define JEWEL_TEST_FATAL(level) 0
	#define JEWEL_LOG(level, msg) 0
	#define JEWEL_LOG_LOCATION(level) 0
	#define JEWEL_LOG_VALUE(level, val) 0

#endif



#endif  // GUARD_debug_log_hpp
