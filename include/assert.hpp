/*
 * Copyright 2012-2013 Matthew Harvey
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef GUARD_assert_hpp_3948190120956018
#define GUARD_assert_hpp_3948190120956018

#ifdef JEWEL_ENABLE_ASSERTION_LOGGING
#	include "log.hpp"
#endif

#include "detail/helper_macros.hpp"
#include <exception>
#include <iostream>

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
 * If JEWEL_ENABLE_ASSERTION_LOGGING is defined, then failure of an
 * assertion via one of these macros will also cause a message
 * to be logged via jewel::Log::log, containing details of
 * the failed assertion, e.g. source file, line number and
 * function. Note that JEWEL_ENABLE_LOGGING does \e not need
 * to be defined in order for this to take effect.
 *
 * @author Matthew Harvey
 * @date 01 Sep 2013.
 *
 */


#ifdef JEWEL_ENABLE_ASSERTION_LOGGING
#	define JEWEL_LOG_ASSERTION_AUX(p) \
		jewel::Log::log \
		(	jewel::Log::error, \
			"Failed assertion", \
			__func__, \
			__FILE__, \
			__LINE__, \
			__DATE__, \
			__TIME__, \
			0, \
			"static_cast<bool>(" #p ")", \
			"false" \
		);
#else
#	define JEWEL_LOG_ASSERTION_AUX(p) if (false) { }
#endif  // JEWEL_ENABLE_ASSERTION_LOGGING


#define JEWEL_HARD_ASSERT(p) \
	if (!(p)) \
	{ \
		JEWEL_LOG_ASSERTION_AUX(p); \
		std::cerr << \
			"Failed assertion (" #p ") " \
			"in file \"" __FILE__ "\" at line " \
			JEWEL_DETAIL_MAKE_STRING_B(__LINE__) ".\n"; \
		std::terminate(); \
	}

#ifndef NDEBUG
#	define JEWEL_ASSERT(p) JEWEL_HARD_ASSERT(p)
#else
#	define JEWEL_ASSERT(p) if (false) { }
#endif  // NDEBUG
	

#endif  // GUARD_assert_hpp_3948190120956018
