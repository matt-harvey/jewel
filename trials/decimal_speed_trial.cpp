/*
 * Copyright 2013 Matthew Harvey
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

#include "decimal.hpp"
#include "stopwatch.hpp"
#include <iostream>
#include <string>
#include <vector>

using jewel::Decimal;
using jewel::Stopwatch;
using std::cout;
using std::endl;
using std::string;
using std::vector;

void reset(Decimal& x, Decimal& y)
{
    x = Decimal(0, 0);
    y = Decimal(0, 0);
    return;
}

int decimal_speed_trial()
{
    cout << "Running Decimal speed trial." << endl;

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
    (   vector<string>::const_iterator it = ctest_vec.begin();
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
    (   vector<string>::const_iterator it = ctest_vec.begin();
         it != ctest_vec.end();
        ++it
    )
    {
        d0 = Decimal(*it);
    }
    cout << ctest_lim * 5 << " calls to Decimal constructor-from-string"
         << " take " << sw_ctest.seconds_elapsed() - ctest_base_case
         << " seconds." << endl;

    return 0;
}

int main()
{
    return decimal_speed_trial();
}
