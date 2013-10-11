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


#include "assert.hpp"
#include "capped_string.hpp"
#include "checked_arithmetic.hpp"
#include "decimal.hpp"
#include "decimal_exceptions.hpp"
#include "decimal_special_tests.hpp"
#include "exception.hpp"
#include "exception_special_tests.hpp"
#include "log.hpp"
#include "num_digits.hpp"
#include <boost/numeric/conversion/cast.hpp>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <UnitTest++/UnitTest++.h>
using boost::numeric_cast;
using jewel::Decimal;
using jewel::detail::decimal_csv_test;
using jewel::detail::decimal_speed_test;
using jewel::detail::test_exception_macros;
using jewel::multiplication_is_unsafe;
using jewel::Log;
using jewel::NumDigits;
using std::cout;
using std::endl;
using std::ios;
using std::numeric_limits;
using std::ostringstream;
using std::setprecision;
using std::string;

int main()
{
	Log::set_filepath("test.log");
	Log::set_threshold(Log::trace);

	#ifndef NDEBUG
		cout << "Compiled in debugging mode (NDEBUG not defined)."
		     << endl;
	#else
		cout << "Compiled in release mode (NDEBUG is defined)."
		     << endl;
	#endif

	#ifdef JEWEL_PERFORM_DECIMAL_OUTPUT_FAILURE_TEST
		cout << "Compiled with JEWEL_PERFORM_DECIMAL_OUTPUT_FAILURE_TEST "
		     << "defined. Normal tests for jewel::Decimal will be "
			 << "eschewed in order to test a sabotaged version of the "
			 << "output function, to ensure it handles errors as expected.\n"
			 << endl;
	#else
		cout << "Compiled without JEWEL_PERFORM_DECIMAL_OUTPUT_FAILURE_TEST "
		     << "defined. Define this to test error handling in stream output"
			 << " operator for jewel::Decimal."
			 << endl;
	#endif

	# ifdef JEWEL_PERFORM_DECIMAL_CSV_TEST
		decimal_csv_test();
	# else
		cout << "Compiled without JEWEL_PERFORM_DECIMAL_CSV_TEST defined. "
		     << "Define this to perform a test involving reading and summing "
			 << "large number of jewel::Decimal from a CSV file."
			 << endl;
	# endif

	# ifdef JEWEL_PERFORM_DECIMAL_SPEED_TEST
		decimal_speed_test();
	# else
		cout << "Compiled without JEWEL_PERFORM_DECIMAL_SPEED_TEST defined. "
		     << "Define this to perform tests on the speed of various "
			 << "operations involving jewel::Decimal."
			 << endl;
	# endif
	
	cout << "\nNow running special tests of exception related macros."
	     << endl;
	
	test_exception_macros();

	cout << "\nNow running various unit tests..." << endl;
	return UnitTest::RunAllTests();
}
