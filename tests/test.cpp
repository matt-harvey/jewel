// Copyright (c) 2013, Matthew Harvey. All rights reserved.

#include <UnitTest++/UnitTest++.h>
#include <boost/numeric/conversion/cast.hpp>
#include "../decimal_exceptions.hpp"
#include "../checked_arithmetic.hpp"
#include "../decimal.hpp"
#include "decimal_special_tests.hpp"
#include "exception_special_tests.hpp"
#include "../num_digits.hpp"
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
using boost::numeric_cast;
using jewel::Decimal;
using jewel::detail::decimal_csv_test;
using jewel::detail::decimal_speed_test;
using jewel::detail::test_exception_macros;
using jewel::multiplication_is_unsafe;
using jewel::NumDigits;
using std::cout;
using std::endl;
using std::ios;
using std::numeric_limits;
using std::ostringstream;
using std::setprecision;
using std::string;

// TEMP
#include "assert.hpp"
#include "log.hpp"
#include <locale>
using jewel::Log;
using std::locale;

int main()
{
	Log::set_filepath("test.log");
	Log::set_threshold(Log::trace);

	#ifndef NDEBUG
		cout << "Compiled in debugging mode (NDEBUG not defined)."
		     << endl;
	#else
		cout << "Compiled in release mode (NDEBUG is defined)." << endl;
	#endif


	#ifdef JEWEL_DECIMAL_OUTPUT_FAILURE_TEST
		cout << "Compiled with JEWEL_DECIMAL_OUTPUT_FAILURE_TEST defined. "
		     << "Normal tests for jewel::Decimal will be "
			 << "eschewed in order to test a sabotaged version of the\n"
			 << "output function, to ensure it handles errors as expected."
			 << endl;
	#else
		cout << "Compiled without JEWEL_DECIMAL_OUTPUT_FAILURE_TEST defined. "
		     << "Define this to test error handling in stream output operator"
			 << " for jewel::Decimal." << endl;
	#endif


	/*
	cout << std::setprecision(2) << std::showpoint << std::fixed;
	cout << std::setw(10) << std::right << double(11) << endl
	     << std::setw(10) << std::right << double(1) << endl
		 << std::setw(10) << std::right << 1.1 << endl
		 << std::setw(10) << std::right << 1.11 << endl
		 << std::setw(10) << std::right << -1.111 << endl
		 << std::setw(10) << std::right << 0.000000001 << endl
		 << std::setw(10) << std::right << double(100000) << endl;
	 */


	/*
	 * Here's a way of inspecting flags
	cout << static_cast<bool>(cout.flags() & ios::hex) << endl;
	cout << static_cast<bool>(cout.flags() & ios::dec) << endl;
	// and changing a flag...
	cout << std::hex;
	// and inspecting them again...
	cout << static_cast<bool>(cout.flags() & ios::hex) << endl;
	cout << static_cast<bool>(cout.flags() & ios::dec) << endl;
	*/

	/*
	decimal_csv_test();
	decimal_speed_test();
	decimal_serialization_test();
	*/
	
	cout << "Now running special tests of exception related macros."
	     << endl;
	
	test_exception_macros();

	cout << "Now running various unit tests..." << endl;
	return UnitTest::RunAllTests();
}
