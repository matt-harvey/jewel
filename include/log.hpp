/*
 * This file is part of the Jewel project and is distributed under the
 * terms of the License contained in the file LICENSE.txt distributed
 * with this package.
 * 
 * Author: Matthew Harvey <matthew@matthewharvey.net>
 *
 * Copyright (c) 2012-2013, Matthew Harvey.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef GUARD_log_hpp_778298908902
#define GUARD_log_hpp_778298908902

/** @file log.hpp
 *
 * @brief Logging facilities
 *
 * @author Matthew Harvey
 * @date 01 September 2013
 *
 */

#include <boost/lexical_cast.hpp>
#include <exception>
#include <new>
#include <string>

namespace jewel
{


/**
 * Class to facilitate logging.
 *
 * To fire logging events using the \e Log class, client code should use the
 * following macros.
 *
 * <b>JEWEL_ENABLE_LOGGING must be defined, otherwise the logging macros
 * will have no effect at all and will compile away to nothing.</b>
 *
 * JEWEL_LOG_TRACE() will fire a logging event that simply displays the name
 * of the function, file and line number where it appeared, with a severity
 * level of \e trace.
 *
 * JEWEL_LOG_MESSAGE(severity, message) will fire a logging event with
 * severity of \e severity and with a message \e message, which should be
 * a string (either C-style, or std::string will do). The log will also show
 * the function, file and line number in the source code where it appears.
 *
 * JEWEL_LOG_VALUE(severity, expression) will fire a logging event which
 * describes the value of \e expression, which should be C++ expression
 * that may be written to a std::ostream, e.g. "1 + 2" or "x" (assuming x
 * is defined). Do not include quotes around the expression when passing
 * it to the macro. The log will also show the function, file and line number
 * in the source code where is appears.
 *
 * JEWEL_HARD_LOGGING_THRESHOLD is by default defined as 0, which means that
 * there is no
 * <em>compile-time</em> restriction on what is logged. If you want to
 * restrict which statements are logged at <em>compile-time</em>, so that
 * statements below a certain severity level are not logged regardless of
 * the runtime threshold passed to <em>Log::set_threshold(...), you can
 * do so by setting <em>JEWEL_HARD_LOGGING_THRESHOLD</em> to a higher
 * severity level, e.g. <em>jewel::Log::warning</em>. On most compilers,
 * with optimization on,
 * should mean that logging statements below this "hard" threshold are
 * compiled away to nothing. You should of course make sure that this is
 * defined prior to including <jewel/log.hpp>.
 *
 * NOTE These logging facilities are not thread-safe.
 *
 * @todo Testing.
 *
 * These logging facilities are guaranteed never to throw an exception, with
 * the following provisos...
 *
 * (a) set_filepath may throw std::bad_alloc.
 *
 * (b) The JEWEL_LOG_VALUE macro makes use of
 * boost::lexical_cast, the implementation of which inserts the passed
 * expression onto a std::ostream during the casting process. If this process
 * of calling boost::lexical_cast results in either boost::bad_lexical_cast or
 * std::bad_alloc being thrown, then that exception will be swallowed rather
 * than propagated. But if any other exception is thrown during the insertion
 * of the passed expression onto the std::ostream, then that exception will
 * not be caught.
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
		trace = 0,  // Must be zero.
		info,
		warning,
		error
	};

	/**
	 * Tell the logging engine the file you want log messages written to.
	 * This must be called or logging will not occur at all.
	 *
	 * @throws std::bad_alloc in the unlikely event of memory allocation
	 * failure while creating the underlying logging stream.
	 */
	static void set_filepath(std::string const& p_filepath);

	/**
	 * Sets the logging threshold so that logging events will be written
	 * to the file if and only if their severity is greater than or
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
		std::string const& p_function,
		std::string const& p_file,
		int p_line,
		std::string const& p_compilation_date,
		std::string const& p_compilation_time,
		std::string const& p_exception_type
	);

	/**
	 * Passes a logging event to the logging mechanism. Note this should
	 * not normally be called by client code, which should instead use
	 * the convenience macros provided (below).
	 */
	static void log
	(	Level p_severity,
		char const* p_message,
		char const* p_function,
		char const* p_file,
		int p_line,
		char const* p_compilation_date = 0,
		char const* p_compilation_time = 0,
		char const* p_exception_type = 0,
		char const* p_expression = 0,
		char const* p_value = 0
	);

private:

	Log();  // private and unimplemented - we don't want instances.

	static char const* severity_string(Level p_level);

	// Pass a stream pointer to set the static stream pointer that lives
	// inside the function. Pass nothing to simply retrieve that pointer.
	static std::ostream* stream_aux(std::ostream* p_stream = 0);

	static Level& threshold_aux();

};

}  // namespace jewel


// MACROS


#ifdef JEWEL_ENABLE_LOGGING

#	ifndef JEWEL_HARD_LOGGING_THRESHOLD
#		define JEWEL_HARD_LOGGING_THRESHOLD 0
#	endif
#	define JEWEL_LOG_TRACE() \
		if (jewel::Log::trace < JEWEL_HARD_LOGGING_THRESHOLD) ; \
		else \
			jewel::Log::log \
			(	jewel::Log::trace, \
				0, \
				__func__, \
				__FILE__, \
				__LINE__, \
				__DATE__, \
				__TIME__ \
			)
#	define JEWEL_LOG_MESSAGE(severity, message) \
		if (severity < JEWEL_HARD_LOGGING_THRESHOLD) ; \
		else \
			jewel::Log::log \
			(	severity, \
				message, \
				__func__, \
				__FILE__, \
				__LINE__, \
				__DATE__, \
				__TIME__ \
			)
#	define JEWEL_LOG_VALUE(severity, expression) \
		if (severity < JEWEL_HARD_LOGGING_THRESHOLD) ; \
		else \
			try \
			{ \
				jewel::Log::log \
				(	severity, \
					0, \
					__func__, \
					__FILE__, \
					__LINE__, \
					__DATE__, \
					__TIME__, \
					0, \
					#expression, \
					boost::lexical_cast<std::string>(expression).c_str() \
				); \
			} \
			catch (boost::bad_lexical_cast) \
			{ \
				JEWEL_LOG_MESSAGE \
				( \
					severity, \
					"Could not log value of expression (" #expression "): " \
					"caught boost::bad_lexical_cast." \
				); \
			} \
			catch (std::bad_alloc) \
			{ \
				JEWEL_LOG_MESSAGE \
				( \
					severity, \
					"Could not log value of expression (" #expression "): " \
					"caught std::bad_alloc." \
				); \
			}
#else
#	define JEWEL_LOG_TRACE() 0
#	define JEWEL_LOG_MESSAGE(severity, message) \
		if (false) { (void)(severity); (void)(message); }  // Silence compiler warnings re. unused variables, and prevent them from being evaluated.
#	define JEWEL_LOG_VALUE(severity, expression) \
		if (false) { (void)(severity); (void)(expression); } // Silence compiler warnings re. unused variables, and prevent them from being evaluated.
		
#endif  // JEWEL_ENABLE_LOGGING


// INLINE FUNCTION DEFINITIONS

namespace jewel
{


inline
Log::Level&
Log::threshold_aux()
{
	static Level ret = info;
	return ret;
}


}  // namespace jewel


#endif  // GUARD_log_hpp_778298908902