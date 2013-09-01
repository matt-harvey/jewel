#ifndef GUARD_debug_log_hpp
#define GUARD_debug_log_hpp

/** @file debug_log.hpp
 *
 * @brief Logging facilities
 *
 * @author Matthew Harvey
 * @date 01 September 2013
 *
 * Copyright (c) 2013, Matthew Harvey. All rights reserved.
 *
 * @todo Documentation and testing.
 */

#include "helper_macros.hpp"
#include <boost/lexical_cast.hpp>
#include <cassert>
#include <exception>
#include <string>

namespace jewel
{


/**
 * Class to facilitate logging.
 * 
 * @todo Testing and documentation.
 */
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

	static void set_stream(std::ostream* os);

	static void set_threshold(Level p_level);

	static void log
	(	Level p_severity,
		std::string const& p_message,
		std::string const& p_file,
		int p_line
	);

	static void log
	(	Level p_severity,
		char const* p_message = 0,
		char const* p_file = 0,
		int p_line = -1
	);

private:
	static char const* severity_string(Level p_level);
	static std::ostream* stream_aux(std::ostream* p_stream = 0);
	static Level& threshold_aux();

};

}  // namespace jewel


// CONVENIENCE MACROS

#ifndef JEWEL_DISABLE_LOGGING

#	define JEWEL_LOG_TRACE() \
		jewel::Log::log(jewel::Log::info, 0, __FILE__, __LINE__)

#	define JEWEL_LOG_MESSAGE(severity, message) \
		jewel::Log::log(severity, message, __FILE__, __LINE__)

#	define JEWEL_LOG_VALUE(expression) \
		jewel::Log::log \
		(	jewel::Log::info, \
			std::string("the value of (" #expression ") is ") + \
				boost::lexical_cast<std::string>(expression), \
			__FILE__, \
			__LINE__ \
		)

#else

#	define JEWEL_LOG_TRACE() 0
#	define JEWEL_LOG_MESSAGE(severity, message) 0
#	define JEWEL_LOG_VALUE(expression) 0

#endif  // JEWEL_DISABLE_LOGGING


// INLINE FUNCTION DEFINITIONS

namespace jewel
{

inline
std::ostream*
Log::stream_aux(std::ostream* p_stream)
{
	static std::ostream* ret = &std::clog;
	if (p_stream) ret = p_stream;
	assert (ret);
	return ret;
}

inline
Log::Level&
Log::threshold_aux()
{
	static Level ret = info;
	return ret;
}


}  // namespace jewel


#endif  // GUARD_debug_log_hpp
