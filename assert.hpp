#ifndef GUARD_assert_hpp
#define GUARD_assert_hpp

#include "log.hpp"
#include "helper_macros.hpp"
#include "exception"

/** @file assert.hpp
 *
 * @brief Contains assertion macro JEWEL_ASSERT which is like the
 * one in <cassert> except that it calls std::terminate rather than
 * std::abort.
 *
 * There is also a JEWEL_HARD_ASSERT macro, which is
 * like JEWEL_ASSERT except that it is always "on",
 * even in release builds.
 *
 * @author Matthew Harvey
 * @date 01 Sep 2013.
 *
 * Copyright (c) 2013, Matthew Harvey. All rights reserved.
 */


#define JEWEL_HARD_ASSERT(p) \
	if (!(p)) \
	{ \
		std::cerr << \
			"Failed assertion (" #p ") " \
			"in file \"" __FILE__ "\" at line " \
			JEWEL_MAKE_STRING_B(__LINE__) ".\n"; \
		std::terminate(); \
	}

#ifndef NDEBUG
#	define JEWEL_ASSERT(p) JEWEL_HARD_ASSERT(p)
#else
#	define JEWEL_ASSERT(p) if (false) (void)(p)  // silence compiler warning re. unused expression, re. also preventing it from being evaluated
#endif
	

#endif  // GUARD_assert_hpp
