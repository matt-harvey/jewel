#include <unittest++/UnitTest++.h>
#include <boost/numeric/conversion/cast.hpp>
#include "checked_arithmetic.hpp"
#include "decimal.hpp"
#include "decimal_special_tests.hpp"
#include "num_digits.hpp"
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
using boost::numeric_cast;
using jewel::Decimal;
using jewel::detail::decimal_csv_test;
using jewel::detail::decimal_speed_test;
using jewel::multiplication_is_unsafe;
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

	#ifndef NDEBUG
		cout << "Compiled in debugging mode (NDEBUG not defined)."
		     << endl;
	#else
		cout << "Compiled in release mode (NDEBUG is defined)." << endl;
	#endif

	cout << endl;

	cout << NumDigits::num_digits(numeric_limits<long long>::max()) << endl;
	cout << numeric_limits<long long>::max() << endl;
	cout << Decimal::maximum() << endl;
	cout << string(18, '8') << endl;
	cout << endl;

	cout << Decimal::maximum() / Decimal(string(18, '9')) << endl;
	cout << endl;

	cout << "With Decimals: " << endl;
	cout << Decimal(string(18, '8')) / Decimal::maximum() << endl;
	cout << endl;

	cout << setprecision(19);

	long long maxll = numeric_limits<long long>::max();
	long double maxll_as_long_double = numeric_cast<long double>(maxll);
	long double dividend = numeric_cast<long double>(888888888888888888);
	cout << "With long doubles: " << endl;
	cout << dividend / maxll_as_long_double << endl;
	cout << endl;

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

	decimal_csv_test();
	// decimal_speed_test();

	return UnitTest::RunAllTests();
}
