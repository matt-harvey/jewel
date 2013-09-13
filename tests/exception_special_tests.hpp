// Copyright (c) 2013, Matthew Harvey. All rights reserved.

#ifndef GUARD_exception_special_tests_hpp_5527552434023262
#define GUARD_exception_special_tests_hpp_5527552434023262


#include "../exception.hpp"

/**
 * This file declares special testing functions (not managed by UnitTest++),
 * which should only be invoked from main.cpp.
 *
 * These test the functioning of the macros in exception_macros.hpp.
 */


namespace jewel
{

namespace detail
{

JEWEL_DERIVED_EXCEPTION(TrialException0, Exception);
JEWEL_DERIVED_EXCEPTION(TrialException1, Exception);
JEWEL_DERIVED_EXCEPTION(TrialException0_0, TrialException0);
JEWEL_DERIVED_EXCEPTION(TrialException1_0, TrialException1);
JEWEL_DERIVED_EXCEPTION(TrialException1_0_1, TrialException1_0);

/**
 * This tests throwing the various dummy exception classes defined in
 * exception_special_tests.hpp, and confirms that they can be handled in
 * various expected ways, given they were defined using the macros
 * in exception.hpp.
 */
void test_exception_macros();




}  // namespace detail

}  // namespace jewel


#endif  // GUARD_exception_special_tests_hpp_5527552434023262
