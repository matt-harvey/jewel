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


#include "decimal.hpp"
#include "decimal_special_tests.hpp"
#include "stopwatch.hpp"
#include <boost/algorithm/string.hpp>

#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

using boost::algorithm::split;
using std::accumulate;
using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::string;
using std::vector;



namespace jewel
{

namespace detail
{

namespace
{
	void reset(Decimal& x, Decimal& y)
	{
		x = Decimal(0, 0);
		y = Decimal(0, 0);
		return;
	}
}

void decimal_speed_test()
{
	int const lim = 1000000;
	vector<Decimal> vec;
	for (int i = 0; i != lim; ++i)
	{
		vec.push_back(Decimal("3.20"));
		vec.push_back(Decimal("98.35"));
	}

	Decimal d0, d1;

	// Base case... we will factor this out when
	// measuring the speed of the arithmetic operations
	Stopwatch sw_base_case;
	for (int i = 0; i != lim; ++i)
	{
		d0 = vec[i];
		d1 = vec[i + 1];
	}
	double const base_case = sw_base_case.seconds_elapsed();


	reset(d0, d1);
	Stopwatch sw_multiplication;
	for (int i = 0; i != lim; ++i)
	{
		d0 = vec[i];
		d1 = vec[i + 1];
		d0 *= d1;
	}
	cout << lim << " multiplications take "
	     << sw_multiplication.seconds_elapsed() - base_case
	     << " seconds." << endl;

	reset(d0, d1);
	Stopwatch sw_division;
	for (int i = 0; i != lim; ++i)
	{
		d0 = vec[i];
		d1 = vec[i + 1];
		d0 /= d1;
	}
	cout << lim << " divisions take "
	     << sw_division.seconds_elapsed() - base_case
		 << " seconds." << endl;

	
	reset(d0, d1);
	Stopwatch sw_addition;
	for (int i = 0; i != lim; ++i)
	{
		d0 = vec[i];
		d1 = vec[i + 1];
		d0 += d1;
	}
	cout << lim << " additions take "
	     << sw_addition.seconds_elapsed() - base_case
	     << " seconds." << endl;

		
	reset(d0, d1);
	Stopwatch sw_subtraction;
	for (int i = 0; i != lim; ++i)
	{
		d0 = vec[i];
		d1 = vec[i + 1];
		d0 -= d1;
	}
	cout << lim << " subtractions take "
	     << sw_subtraction.seconds_elapsed() - base_case
	     << " seconds." << endl;


	// Measure construction
	int const ctest_lim = 1000000 / 5;
	vector<string> ctest_vec;
	for (int i = 0; i != ctest_lim; ++i)
	{
		ctest_vec.push_back("-0.9871967");
		ctest_vec.push_back("13987.870");
		ctest_vec.push_back("23.2");
		ctest_vec.push_back("19");
		ctest_vec.push_back("-1234.197423");
	}
	Stopwatch sw_ctest_base;
	for
	(	vector<string>::const_iterator it = ctest_vec.begin();
		it != ctest_vec.end();
		++it
	)
	{
		string s = *it;
	}
	double const ctest_base_case = sw_ctest_base.seconds_elapsed();
	reset(d0, d1);
	Stopwatch sw_ctest;
	for
	(	vector<string>::const_iterator it = ctest_vec.begin();
	 	it != ctest_vec.end();
		++it
	)
	{
		d0 = Decimal(*it);
	}
	cout << ctest_lim * 5 << " calls to Decimal constructor-from-string"
	     << " take " << sw_ctest.seconds_elapsed() - ctest_base_case
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

