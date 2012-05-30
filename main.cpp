#include <unittest++/UnitTest++.h>
#include <iostream>


#include "arithmetic_exceptions.hpp"
#include "decimal.hpp"
#include "num_digits.hpp"
#include "stopwatch.hpp"
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <fstream>
#include <iomanip>
#include <limits>
#include <map>
#include <sstream>
#include <vector>
using jewel::Decimal;
using jewel::NumDigits;
using jewel::Stopwatch;
using jewel::UnsafeArithmeticException;
using boost::algorithm::split;
using boost::format;
using boost::lexical_cast;
using boost::numeric_cast;
using std::cout;
using std::endl;
using std::ifstream;
using std::ios;
using std::map;
using std::numeric_limits;
using std::ofstream;
using std::ostringstream;
using std::setprecision;
using std::string;
using std::vector;

typedef Decimal::int_type int_type; 

bool is_dagger(char c)
{
	return c == '|';
}



int main()
{
	#ifdef JEWEL_DECIMAL_DISABLE_ARITHMETIC_CHECKING
		cout << "Decimal arithmetic checking is disabled." << endl;
	#else
		cout << "Decimal arithmetic checking is enabled." << endl;
	#endif

	#ifndef NDEBUG
		cout << "Compiled in debugging mode (NDEBUG not defined)."
		     << endl;
	#else
		cout << "Compiled in release mode (NDEBUG is defined)." << endl;
	#endif

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
	cout << static_cast<bool>(cout.flags() & ios::hex) << endl;
	cout << static_cast<bool>(cout.flags() & ios::dec) << endl;
	cout << std::hex;
	cout << static_cast<bool>(cout.flags() & ios::hex) << endl;
	cout << static_cast<bool>(cout.flags() & ios::dec) << endl;
	*/

	/*
	// Test the speed of arithmetic operations

	int lim = 1000000;
	vector<Decimal> vec;
	for (int i = 0; i != lim; ++i)
	{
		vec.push_back(Decimal("3.32"));
		vec.push_back(Decimal("98.27157"));
	}

	Stopwatch sw_base_case;
	for (int i = 0; i != lim; ++i)
	{
		Decimal d0 = vec[i];
		Decimal d1 = vec[i + 1];
	}
	double const base_case = sw_base_case.seconds_elapsed();

	Stopwatch sw_multiplication;
	for (int i = 0; i != lim; ++i)
	{
		Decimal d0 = vec[i];
		Decimal d1 = vec[i + 1];
		d0 *= d1;
	}
	cout << lim << " multiplications take "
	     << sw_multiplication.seconds_elapsed() - base_case
	     << " seconds." << endl;

	Stopwatch sw_division;
	for (int i = 0; i != lim; ++i)
	{
		Decimal d0 = vec[i];
		Decimal d1 = vec[i + 1];
		d0 /= d1;
	}
	cout << lim << " divisions take "
	     << sw_division.seconds_elapsed() - base_case
		 << " seconds." << endl;

	
	Stopwatch sw_addition;
	for (int i = 0; i != lim; ++i)
	{
		Decimal d0 = vec[i];
		Decimal d1 = vec[i + 1];
		d0 += d1;
	}
	cout << lim << " additions take "
	     << sw_addition.seconds_elapsed() - base_case
	     << " seconds." << endl;

		
	Stopwatch sw_subtraction;
	for (int i = 0; i != lim; ++i)
	{
		Decimal d0 = vec[i];
		Decimal d1 = vec[i + 1];
		d0 -= d1;
	}
	cout << lim << " subtractions take "
	     << sw_subtraction.seconds_elapsed() - base_case
	     << " seconds." << endl;
	
	// End "test the speed of arithmetic operations"
	*/

	return UnitTest::RunAllTests();
}
