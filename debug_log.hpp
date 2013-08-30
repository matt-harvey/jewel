// Copyright (c) 2013, Matthew Harvey. All rights reserved.

#ifndef GUARD_debug_log_hpp
#define GUARD_debug_log_hpp

/** @file debug_log.hpp
 *
 * @brief Provides macro for printing to std::clog in debug builds.
 *
 * From someone called "snstrnd" on Stack Overflow.
 */

#include <iostream>


/**Macros to print to std::clog if DEBUG is defined, or else to compile
 * away to nothing if DEBUG is not defined.
 *
 * If DEBUG is defined, JEWEL_DEBUG_LOG acts like std::clog
 * (but prints "LOG: " before the rest of the output),
 * but if DEBUG is not defined, JEWEL_DEBUG_LOG, and the rest
 * of the line, is effectively compiled away to nothing.
 *
 * If DEBUG is defined JEWEL_DEBUG_LOG_LOCATION prints the
 * file and line number to std::clog; otherwise, it is
 * effectively compiled away to nothing.
 *
 * If DEBUG is defined JEWEL_DEBUG_LOG_VALUE(x) prints a string
 * of the form "LOG: x: [value of x]". E.g. if x == 3, then
 * JEWEL_DEBUG_LOG_VALUE(x) will print "LOG: x: 3".
 *
 * Note a semicolon must still be included by client code
 * at the end of a statement using either of these macros.
 *
 * No particular exception safety guarantee is offered by
 * these macros. They are not intended for use in release builds.
 * But note that all it does is write to std::clog; so unless
 * exceptions have been enabled on std::clog, the only part of this
 * function that could throw is in initialization of the string being
 * written to std::clog. This might throw under conditions of
 * memory shortage; but it is extremely unlikely.
 */ 

#ifdef DEBUG
	#define JEWEL_DEBUG_LOG std::clog << "LOG: "
	#define JEWEL_DEBUG_LOG_LOCATION std::clog << __FILE__ << ": " << __LINE__ << std::endl;
	#define JEWEL_DEBUG_LOG_VALUE(x) std::clog << "LOG: " #x ": " << x << std::endl;
#else
	#define JEWEL_DEBUG_LOG 0 && std::clog
	#define JEWEL_DEBUG_LOG_LOCATION 0
	#define JEWEL_DEBUG_LOG_VALUE(X) 0
#endif





#endif  // GUARD_debug_log_hpp
