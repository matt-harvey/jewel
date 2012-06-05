#include "decimal.hpp"
#include "decimal_special_tests.hpp"
#include "stopwatch.hpp"
#include <boost/algorithm/string.hpp>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using boost::algorithm::split;
using std::cout;
using std::endl;
using std::ifstream;
using std::string;
using std::vector;



namespace jewel
{

namespace detail
{

void decimal_speed_test()
{
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

	return;
}


bool is_dagger(char c)
{
	return c == '|';
}


void decimal_csv_test()
{
	cout << "Testing processing two columns of Decimals from a text file..."
	     << endl;
	ifstream data("testdata");
	string current_record;
	Decimal field_0_total("0");
	Decimal field_1_total("0");
	while (getline(data, current_record))
	{
		vector<string> record_vec;
		split(record_vec, current_record, is_dagger);
		Decimal d0(record_vec[0]);
		Decimal d1(record_vec[1]);
		field_0_total += d0;
		field_1_total += d1; 
	}
	if (field_0_total != Decimal("0"))
	{
		cout << "Test failed. First column should have summed to 0, but "
		        "instead summed to: " << field_0_total << endl;
	}
	if (field_1_total != Decimal("0"))
	{
		cout << "Test failed. Second column should have summed to 0, but "
		        "instead summed to: " << field_1_total << endl;
	}
	if (field_0_total == Decimal("0") && field_1_total == Decimal("0"))
	{
		cout << "Test succeeded. Both columns of Decimals summed to 0, as "
		        "expected." << endl;
	}
	return;
}

}  // namespace detail

}  // namespace jewel

