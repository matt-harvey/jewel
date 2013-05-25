// Copyright (c) 2013, Matthew Harvey. All rights reserved.

#include "num_digits.hpp"

namespace jewel
{

std::size_t
NumDigits::num_digits(int x, int base)
{
	if (x < 0) x = -x;
	return num_digits_aux(x, base);
}

std::size_t
NumDigits::num_digits(short x, short base)
{
	if (x < 0) x = -x;
	return num_digits_aux(x, base);
}

std::size_t
NumDigits::num_digits(long x, long base)
{
	if (x < 0) x = -x;
	return num_digits_aux(x, base);
}

std::size_t
NumDigits::num_digits(long long x, long long base)
{
	if (x < 0) x = -x;
	return num_digits_aux(x, base);
}

std::size_t
NumDigits::num_digits(unsigned int x, unsigned int base)
{
	return num_digits_aux(x, base);
}

std::size_t
NumDigits::num_digits(unsigned short x, unsigned short base)
{
	return num_digits_aux(x, base);
}

std::size_t
NumDigits::num_digits(unsigned long x, unsigned long base)
{
	return num_digits_aux(x, base);
}

std::size_t
NumDigits::num_digits(unsigned long long x, unsigned long long base)
{
	return num_digits_aux(x, base);
}


}  // namespace jewel
