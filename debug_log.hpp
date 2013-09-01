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

#include "helper_macros.hpp"
#include <exception>
#include <iostream>

namespace jewel
{

class Log
{
public:

	// Note if we add levels, we also need to update the
	// severity_string function.
	enum Level
	{
		info,
		warning,
		error
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

	/**
	 * For use by JEWEL_LOG... macros only - not for use in client code.
	 */
	static char const* severity_string(Level p_level)
	{
		static char const* strings[] =
		{	"info",
			"warning",
			"error"
		};
		return strings[p_level];
	}

private:

	static Level& threshold_aux()
	{
		static Level ret = info;
		return ret;
	}

};

}  // namespace jewel


#ifdef DEBUG

#	define JEWEL_LOG(level, msg) \
		if (level >= jewel::Log::threshold()) \
		{ \
			std::clog << "SEVERITY:\t" << jewel::Log::severity_string(level) \
					  << "\tMESSAGE:\t" msg \
					  << "\n"; \
		}

#	define JEWEL_LOG_LOCATION(level) \
		if (level >= jewel::Log::threshold()) \
		{ \
			std::clog << "SEVERITY:\t" << jewel::Log::severity_string(level) \
			          << "\tFUNCTION:\t" << __func__ \
			          << "\tFILE:\t" << __FILE__ \
					  << "\tLINE:\t" << __LINE__ \
					  << "\n"; \
		} 
	
#	define JEWEL_LOG_VALUE(level, val) \
		if (level >= jewel::Log::threshold()) \
		{ \
			std::clog << "SEVERITY:\t" << jewel::Log::severity_string(level) \
			          << "\tEXPRESSION:\t" << #val \
					  << "\tVALUE:\t" << val \
					  << "\n"; \
		}

#else

#	define JEWEL_LOG(level, msg) 0
#	define JEWEL_LOG_LOCATION(level) 0
#	define JEWEL_LOG_VALUE(level, val) 0

#endif   // DEBUG



#endif  // GUARD_debug_log_hpp
