#include <unittest++/UnitTest++.h>
#include "decimal_special_tests.hpp"
#include "num_digits.hpp"
#include "stopwatch.hpp"
#include <boost/lexical_cast.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <iomanip>
#include <iostream>
#include <map>
using jewel::detail::decimal_csv_test;
using jewel::detail::decimal_speed_test;
using std::cout;
using std::endl;
using std::ios;
using std::setprecision;

#include "decimal.hpp"
using jewel::Decimal;



int main()
{

	#ifndef NDEBUG
		cout << "Compiled in debugging mode (NDEBUG not defined)."
		     << endl;
	#else
		cout << "Compiled in release mode (NDEBUG is defined)." << endl;
	#endif


	/*
	cout << "A sample of Decimal instances" << endl << endl;
	cout << "string rep." << std::setw(10) << "m_intval" << std::setw(10)
	     << "m_places"
	     << endl;
	Decimal d0("0");
	Decimal d1("0.0");
	Decimal d2("-0.00");
	Decimal d3("234.212340");
	Decimal d4("0.00033");
	Decimal d5("0.000330");
	Decimal d6("-0.000330");
	Decimal da[] = { d0, d1, d2, d3, d4, d5, d6 };
	for (size_t i = 0; i != 7; ++i)
	{
		cout << std::setw(10) << da[i]
		     << std::setw(10) << da[i].get_intval()
			 << std::setw(10) << da[i].get_places()
			 << std::setw(10) << endl;
	}
	cout << endl;
	*/






	/* Here's how to print currency (though this doesn't include parentheses
	 * for negative, or thousands separators just yet).
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

	// decimal_csv_test();
	// decimal_speed_test();

	return UnitTest::RunAllTests();
}
