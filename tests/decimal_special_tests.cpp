/*
 * Copyright 2012, 2013 Matthew Harvey
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

#include "assert.hpp"
#include "decimal.hpp"
#include "decimal_special_tests.hpp"
#include <boost/algorithm/string.hpp>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

using boost::algorithm::split;
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

bool is_dagger(char c)
{
	return c == '|';
}

int decimal_csv_test()
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
		Decimal const d0(record_vec[0]);
		Decimal const d1(record_vec[1]);
		field_0_total += d0;
		field_1_total += d1; 
	}
	if (field_0_total != Decimal("0"))
	{
		cout << "Test failed. First column should have summed to 0, but "
		        "instead summed to: " << field_0_total << endl;
		return 1;
	}
	if (field_1_total != Decimal("0"))
	{
		cout << "Test failed. Second column should have summed to 0, but "
		        "instead summed to: " << field_1_total << endl;
		return 1;
	}
	if (field_0_total == Decimal("0") && field_1_total == Decimal("0"))
	{
		cout << "Test succeeded. Both columns of Decimals summed to 0, as "
		        "expected." << endl;
		return 0;
	}
	JEWEL_HARD_ASSERT (false);
}

}  // namespace detail
}  // namespace jewel

