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

#include "num_digits.hpp"
#include "assert.hpp"

namespace jewel
{

std::size_t
NumDigits::num_digits(int x, int base)
{
	JEWEL_ASSERT (base > 1);
	return num_digits_aux(x, base);
}

std::size_t
NumDigits::num_digits(short x, short base)
{
	JEWEL_ASSERT (base > 1);
	return num_digits_aux(x, base);
}

std::size_t
NumDigits::num_digits(long x, long base)
{
	JEWEL_ASSERT (base > 1);
	return num_digits_aux(x, base);
}

std::size_t
NumDigits::num_digits(long long x, long long base)
{
	JEWEL_ASSERT (base > 1);
	return num_digits_aux(x, base);
}

std::size_t
NumDigits::num_digits(signed char x, signed char base)
{
	JEWEL_ASSERT (base > 1);
	return num_digits_aux(x, base);
}

std::size_t
NumDigits::num_digits(unsigned int x, unsigned int base)
{
	JEWEL_ASSERT (base > 1);
	return num_digits_aux(x, base);
}

std::size_t
NumDigits::num_digits(unsigned short x, unsigned short base)
{
	JEWEL_ASSERT (base > 1);
	return num_digits_aux(x, base);
}

std::size_t
NumDigits::num_digits(unsigned long x, unsigned long base)
{
	JEWEL_ASSERT (base > 1);
	return num_digits_aux(x, base);
}

std::size_t
NumDigits::num_digits(unsigned long long x, unsigned long long base)
{
	JEWEL_ASSERT (base > 1);
	return num_digits_aux(x, base);
}

std::size_t
NumDigits::num_digits(unsigned char x, unsigned char base)
{
	JEWEL_ASSERT (base > 1);
	return num_digits_aux(x, base);
}


}  // namespace jewel
