/*
 * Copyright 2012 Matthew Harvey
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

#include "num_digits.hpp"
#include <UnitTest++/UnitTest++.h>
#include <climits>

using jewel::NumDigits;

TEST(num_digits_with_default_base_int)
{
	CHECK_EQUAL(NumDigits::num_digits(0), static_cast<size_t>(1));
	CHECK_EQUAL(NumDigits::num_digits(0, 10), static_cast<size_t>(1));
	CHECK_EQUAL(NumDigits::num_digits(1), static_cast<size_t>(1));
	CHECK_EQUAL(NumDigits::num_digits(-1), static_cast<size_t>(1));
	CHECK_EQUAL(NumDigits::num_digits(2), static_cast<size_t>(1));
	CHECK_EQUAL(NumDigits::num_digits(6), static_cast<size_t>(1));
	CHECK_EQUAL(NumDigits::num_digits(-6), static_cast<size_t>(1));
	CHECK_EQUAL(NumDigits::num_digits(6, 10), static_cast<size_t>(1));
	CHECK_EQUAL(NumDigits::num_digits(-6, 10), static_cast<size_t>(1));
	CHECK_EQUAL(NumDigits::num_digits(9), static_cast<size_t>(1));
	CHECK_EQUAL(NumDigits::num_digits(-9), static_cast<size_t>(1));
	CHECK_EQUAL(NumDigits::num_digits(10), static_cast<size_t>(2));
	CHECK_EQUAL(NumDigits::num_digits(-10), static_cast<size_t>(2));
	CHECK_EQUAL(NumDigits::num_digits(34), static_cast<size_t>(2));
	CHECK_EQUAL(NumDigits::num_digits(-34), static_cast<size_t>(2));
	CHECK_EQUAL(NumDigits::num_digits(100), static_cast<size_t>(3));
	CHECK_EQUAL(NumDigits::num_digits(-100), static_cast<size_t>(3));
	CHECK_EQUAL(NumDigits::num_digits(999), static_cast<size_t>(3));
	CHECK_EQUAL(NumDigits::num_digits(-999), static_cast<size_t>(3));
	CHECK_EQUAL(NumDigits::num_digits(1000), static_cast<size_t>(4));
	CHECK_EQUAL(NumDigits::num_digits(-1000), static_cast<size_t>(4));
	CHECK_EQUAL(NumDigits::num_digits(10798778), static_cast<size_t>(8));
	CHECK_EQUAL(NumDigits::num_digits(-10798778), static_cast<size_t>(8));
	CHECK_EQUAL(NumDigits::num_digits(999999999), static_cast<size_t>(9));
	CHECK_EQUAL(NumDigits::num_digits(-999999999), static_cast<size_t>(9));
	CHECK_EQUAL
	(	NumDigits::num_digits(INT_MAX),
		NumDigits::num_digits(INT_MIN)
	);
}

TEST(num_digits_with_base_of_2_int)
{
	CHECK_EQUAL(NumDigits::num_digits(0, 2), static_cast<size_t>(1));
	CHECK_EQUAL(NumDigits::num_digits(1, 2), static_cast<size_t>(1));
	CHECK_EQUAL(NumDigits::num_digits(-1, 2), static_cast<size_t>(1));
	CHECK_EQUAL(NumDigits::num_digits(2, 2), static_cast<size_t>(2));
	CHECK_EQUAL(NumDigits::num_digits(-2, 2), static_cast<size_t>(2));
	CHECK_EQUAL(NumDigits::num_digits(3, 2), static_cast<size_t>(2));
	CHECK_EQUAL(NumDigits::num_digits(4, 2), static_cast<size_t>(3));
	CHECK_EQUAL(NumDigits::num_digits(7, 2), static_cast<size_t>(3));
	CHECK_EQUAL(NumDigits::num_digits(-8, 2), static_cast<size_t>(4));
}

TEST(num_digits_with_base_of_10_short)
{
	short i0 = 0;
	short i10 = 10;
	short i123 = 123;
	short i1243= 1243;
	short i_0 = -0;
	short i_12342 = -2343;
	CHECK_EQUAL(NumDigits::num_digits(i0), static_cast<size_t>(1));
	CHECK_EQUAL(NumDigits::num_digits(i10), static_cast<size_t>(2));
	CHECK_EQUAL(NumDigits::num_digits(i123), static_cast<size_t>(3));
	CHECK_EQUAL(NumDigits::num_digits(i1243), static_cast<size_t>(4));
	CHECK_EQUAL(NumDigits::num_digits(i_0), static_cast<size_t>(1));
	CHECK_EQUAL(NumDigits::num_digits(i_12342), static_cast<size_t>(4));
}

TEST(num_digits_with_base_of_10_long)
{
	long i0 = 0;
	long i10 = 10;
	long i123 = 123;
	long i1243= 1243;
	long i_0 = -0;
	long i_12342 = -2343;
	long i_976926943 = -976926943;
	CHECK_EQUAL(NumDigits::num_digits(i0), static_cast<size_t>(1));
	CHECK_EQUAL(NumDigits::num_digits(i10), static_cast<size_t>(2));
	CHECK_EQUAL(NumDigits::num_digits(i123), static_cast<size_t>(3));
	CHECK_EQUAL(NumDigits::num_digits(i1243), static_cast<size_t>(4));
	CHECK_EQUAL(NumDigits::num_digits(i_0), static_cast<size_t>(1));
	CHECK_EQUAL(NumDigits::num_digits(i_12342), static_cast<size_t>(4));
	CHECK_EQUAL(NumDigits::num_digits(i_976926943), static_cast<size_t>(9)); 
}

TEST(num_digits_with_base_of_10_unsigned_int)
{
	unsigned int i0 = 0;
	unsigned int i10 = 10;
	unsigned int i123 = 123;
	unsigned int i1243 = 1243;
	unsigned int i887423 = 887423;
	CHECK_EQUAL(NumDigits::num_digits(i0), static_cast<size_t>(1));
	CHECK_EQUAL(NumDigits::num_digits(i10), static_cast<size_t>(2));
	CHECK_EQUAL(NumDigits::num_digits(i123), static_cast<size_t>(3));
	CHECK_EQUAL(NumDigits::num_digits(i1243), static_cast<size_t>(4));
	CHECK_EQUAL(NumDigits::num_digits(i887423), static_cast<size_t>(6));
}

TEST(num_digits_with_base_of_10_unsigned_short)
{
	unsigned short i0 = 0;
	unsigned short i10 = 10;
	unsigned short i123 = 123;
	unsigned short i1243 = 1243;
	unsigned short i9762 = 9762;
	CHECK_EQUAL(NumDigits::num_digits(i0), static_cast<size_t>(1));
	CHECK_EQUAL(NumDigits::num_digits(i10), static_cast<size_t>(2));
	CHECK_EQUAL(NumDigits::num_digits(i123), static_cast<size_t>(3));
	CHECK_EQUAL(NumDigits::num_digits(i1243), static_cast<size_t>(4));
	CHECK_EQUAL(NumDigits::num_digits(i9762), static_cast<size_t>(4));
}

TEST(num_digits_with_base_of_10_unsigned_long)
{
	unsigned long i0 = 0;
	unsigned long i10 = 10;
	unsigned long i123 = 123;
	unsigned long i1243 = 1243;
	unsigned long i887423 = 887423;
	CHECK_EQUAL(NumDigits::num_digits(i0), static_cast<size_t>(1));
	CHECK_EQUAL(NumDigits::num_digits(i10), static_cast<size_t>(2));
	CHECK_EQUAL(NumDigits::num_digits(i123), static_cast<size_t>(3));
	CHECK_EQUAL(NumDigits::num_digits(i1243), static_cast<size_t>(4));
	CHECK_EQUAL(NumDigits::num_digits(i887423), static_cast<size_t>(6));
}



