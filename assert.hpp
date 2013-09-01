#ifndef GUARD_assert_hpp
#define GUARD_assert_hpp

#include "helper_macros.hpp"
#include <jewel/exception.hpp>

/** @file assert.hpp
 *
 * @brief Contains assertion macro which is like the one is
 * <cassert> except it throws an exception
 * rather than than simply calling std::abort.
 *
 * @author Matthew Harvey
 * @date 01 Sep 2013.
 *
 * Copyright (c) 2013, Matthew Harvey. All rights reserved.
 */


namespace jewel
{

JEWEL_DERIVED_EXCEPTION(AssertionFailure, Exception);

}  // namespace jewel


#ifndef NDEBUG

#	define JEWEL_ASSERT(p) \
		if (!p) \
		{ \
			throw jewel::AssertionFailure \
			(	"Failed assertion (" #p ") in function " JEWEL_FUNCTION ", " \
				"in file \"" __FILE__ "\" at line " \
				JEWEL_MAKE_STRING(__LINE__) \
			); \
		}

#else

#	define JEWEL_ASSERT(p)

#endif
	

#endif  // GUARD_assert_hpp
