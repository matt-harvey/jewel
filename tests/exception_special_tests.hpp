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
