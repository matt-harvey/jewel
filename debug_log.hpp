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
 * If DEBUG is defined, JEWEL_DEBUG_LOG acts like std::clog;
 * but if DEBUG is not defined, JEWEL_DEBUG_LOG, and the rest
 * of the line, is effectively compiled away to nothing.
 *
 * No particular exception safety guarantee is offered by
 * this macro. It is not intended for use in release builds.
 * But note that all it does is write to std::clog; so unless
 * exceptions have been enabled on std::clog, the only part of this
 * function that could throw is in initialization of the string being
 * written to std::clog. This might throw under conditions of
 * memory shortage; but it is extremely unlikely.
 */ 

#ifdef DEBUG
	#define JEWEL_DEBUG_LOG std::clog << "  LOG: "
#else
	#define JEWEL_DEBUG_LOG 0 && std::clog
#endif





#endif  // GUARD_debug_log_hpp
