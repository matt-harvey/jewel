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
using jewel::round;
using jewel::CheckedArithmetic;
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
	 
	// Test processing from a csv file	

	Stopwatch sw;

	ifstream data("testdata");
	string current_record;
	map<string, vector<Decimal> > trial_balance;
	vector<Decimal> starter;
	starter.push_back(Decimal("0"));
	starter.push_back(Decimal("0"));
	starter.push_back(Decimal("0"));

	while (getline(data, current_record))
	{
		vector<string> record_vec;
		split(record_vec, current_record, is_dagger);
		string account = record_vec[4];
		Decimal actual_impact(record_vec[5]);
		Decimal budget_impact(record_vec[6]);

		if (trial_balance.find(account) == trial_balance.end())
		{
			trial_balance[account] = starter;
		}
		trial_balance[account][0] += actual_impact;	
		trial_balance[account][1] += budget_impact;
		trial_balance[account][2] += (budget_impact - actual_impact);
	}

	sw.log();

	ofstream tbreport("tbreport");
	for (map<string, vector<Decimal> >::const_iterator it =
	  trial_balance.begin(); it != trial_balance.end(); ++it)
	{
		tbreport << it->first << '|'
		         << it->second[0] << '|'
				 << it->second[1] << '|'
				 << it->second[2] << '\t'
				 << endl;
	}

	sw.log();

	Decimal results[3];
	assert (results[0] == Decimal("0"));
	assert (results[1] == Decimal("0"));
	assert (results[2] == Decimal("0"));
	for (map<string, vector<Decimal> >::const_iterator it =
	  trial_balance.begin(); it != trial_balance.end(); ++it)
	{
		results[0] += it->second[0];
		results[1] += it->second[1];
		results[2] += it->second[2];
	}
	assert (results[0] == Decimal("0"));
	assert (results[1] == Decimal("0"));
	assert (results[2] == Decimal("0"));

	sw.log();

	// End test processing from a csv file


	*/

	/*
	cout << static_cast<bool>(cout.flags() & ios::hex) << endl;
	cout << static_cast<bool>(cout.flags() & ios::dec) << endl;
	cout << std::hex;
	cout << static_cast<bool>(cout.flags() & ios::hex) << endl;
	cout << static_cast<bool>(cout.flags() & ios::dec) << endl;
	*/

	// Test the speed of arithmetic operations

	int lim = 1000000;
	vector<Decimal> vec;
	for (int i = 0; i != lim; ++i)
	{
		vec.push_back(Decimal("3.20"));
		vec.push_back(Decimal("98.35"));
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


	return UnitTest::RunAllTests();
}
