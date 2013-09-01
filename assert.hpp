#ifndef GUARD_assert_hpp
#define GUARD_assert_hpp

#include "helper_macros.hpp"
#include <jewel/debug_log.hpp>
#include <jewel/exception.hpp>

/** @file assert.hpp
 *
 * @brief Contains assertion macro which is like the one is
 * <cassert> except it throws an exception
 * rather than than simply calling std::abort. In addition,
 * it writes to the log via jewel::Log, just before throwing
 * the exception.
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
		if (!(p)) \
		{ \
			JEWEL_LOG \
			(	jewel::Log::error, \
				"Throwing FailedAssertion instance" \
				" from the following location..." \
			) \
			JEWEL_LOG_LOCATION(jewel::Log::error) \
			char const* msg = \
				"Failed assertion (" #p ") " \
				"in file \"" __FILE__ "\" at line " \
				JEWEL_MAKE_STRING_B(__LINE__) "."; \
			throw jewel::AssertionFailure(msg); \
		}

#else

#	define JEWEL_ASSERT(p)

#endif
	

#endif  // GUARD_assert_hpp
