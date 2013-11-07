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

#ifndef GUARD_exception_special_tests_hpp_5527552434023262
#define GUARD_exception_special_tests_hpp_5527552434023262


#include "exception.hpp"

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
