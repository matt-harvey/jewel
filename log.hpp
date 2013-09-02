#ifndef GUARD_log_hpp_778298908902
#define GUARD_log_hpp_778298908902

/** @file log.hpp
 *
 * @brief Logging facilities
 *
 * @author Matthew Harvey
 * @date 01 September 2013
 *
 * Copyright (c) 2013, Matthew Harvey. All rights reserved.
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
 * To fire logging events using the \e Log class, client code should call the
 * following macros.
 *
 * JEWEL_LOG_MESSAGE(severity, message) will fire a logging event will
 * severity of \e severity and with a message \e message, which should be
 * a string (either C-style, or std::string will do).
 *
 * JEWEL_LOG_VALUE(severity, expression) will fire a logging event which
 * describes the value of \e expression, which should be C++ expression
 * that may be written to a std::ostream, e.g. "1 + 2" or "x" (assuming x
 * is defined). Do not include quotes around the expression when passing
 * it to the macro.
 *
 * JEWEL_DISABLE_LOGGING if defined will disable logging entirely. All logging
 * statements <em>which use these macros</em> will then be compiled away
 * to nothing.
 *
 * @todo Testing.
 */
class Log
{
public:

	/**
	 * @enum Level
	 *
	 * Represents a series of increasing severity levels for
	 * logging events.
	 */
	enum Level
	{
		// Note if we add levels, we also need to update the
		// severity_string function.
		trace,
		info,
		warning,
		error
	};

	/**
	 * Pass a pointer to a heap-allocated (newed) std::ostream*. Note:
	 * do \e not delete the pointer. The memory should remain allocated
	 * for the life of the application. (You normally should not call
	 * this more than once.) If this is not called, then by default
	 * this will write to \e std::clog.
	 */
	static void set_stream(std::ostream* p_os);

	/**
	 * Sets the logging threshold so that logging events will be writting
	 * to the stream if and only if their severity is greater than or
	 * equal to \e p_level. By default, the threshold is \e info.
	 */
	static void set_threshold(Level p_level);

	/**
	 * Passes a logging event to the logging mechanism. Note this should
	 * not normally be called by client code, which should instead use
	 * the convenience macros provided (below).
	 */
	static void log
	(	Level p_severity,
		std::string const& p_message,
		std::string const& p_file,
		int p_line
	);

	/**
	 * Passes a logging event to the logging mechanism. Note this should
	 * not normally be called by client code, which should instead use
	 * the convenience macros provided (below).
	 */
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


// MACROS

#ifndef JEWEL_DISABLE_LOGGING
#	define JEWEL_LOG_TRACE() \
		jewel::Log::log(jewel::Log::trace, 0, __FILE__, __LINE__)
#	define JEWEL_LOG_MESSAGE(severity, message) \
		jewel::Log::log(severity, message, __FILE__, __LINE__)
#	define JEWEL_LOG_VALUE(severity, expression) \
		jewel::Log::log \
		(	severity, \
			std::string("the value of (" #expression ") is ") + \
				boost::lexical_cast<std::string>(expression), \
			__FILE__, \
			__LINE__ \
		)
#else
#	define JEWEL_LOG_TRACE() 0
#	define JEWEL_LOG_MESSAGE(severity, message) 0
#	define JEWEL_LOG_VALUE(severity, expression) 0
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


#endif  // GUARD_log_hpp_778298908902
