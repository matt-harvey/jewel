// Copyright (c) 2013, Matthew Harvey. All rights reserved.

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
