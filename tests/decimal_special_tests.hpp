/*
 * Copyright 2012-2013 Matthew Harvey
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

#ifndef GUARD_decimal_special_tests_hpp_4550346198748027
#define GUARD_decimal_special_tests_hpp_4550346198748027


/**
 * This file contains special testing functions (not managed by UnitTest++),
 * which should only be invoked from main.cpp.
 */

namespace jewel
{

namespace detail
{

/** Speed test for arithmetic operations using Decimal class
 * Outputs to std::cout results of test.
 */
void decimal_speed_test();

/** Helper function used in decimal_csv_test.
 */
bool is_dagger(char c);

/** Test processing of many Decimals from a csv file. These are two
 * digit decimals typical of accounting data, in two delimited columns,
 * which should sum to zero.
 */
void decimal_csv_test();



}  // namespace detail

}  // namespace jewel



#endif  // GUARD_decimal_special_tests_hpp_4550346198748027
