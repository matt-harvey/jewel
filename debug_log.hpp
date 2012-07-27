#ifndef GUARD_debug_log_hpp
#define GUARD_debug_log_hpp

/** @file debug_log.hpp
 *
 * @brief Provides macro for printing to std::clog in debug builds.
 *
 * From someone called "snstrnd" on Stack Overflow.
 */

#include <iostream>


/**Macro to print to std::clog iff DEBUG is defined.
 *
 * If DEBUG is defined, DEBUG_LOG acts like std::clog;
 * but if DEBUG is not defined, DEBUG_LOG, and the rest
 * of the line, is effectively compiled away to nothing.
 */
#ifdef DEBUG
	#define DEBUG_LOG std::clog
#else
	#define DEBUG_LOG 0 && std::clog
#endif





#endif  // GUARD_debug_log_hpp