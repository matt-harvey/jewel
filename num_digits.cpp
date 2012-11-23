#include "num_digits.hpp"
#include <boost/cstdint.hpp>

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

/*
std::size_t
NumDigits::num_digits(long x, long base)
{
	if (x < 0) x = -x;
	return num_digits_aux(x, base);
}
*/

std::size_t
NumDigits::num_digits(boost::int64_t x, boost::int64_t base)
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

/*
std::size_t
NumDigits::num_digits(unsigned long x, unsigned long base)
{
	return num_digits_aux(x, base);
}
*/

std::size_t
NumDigits::num_digits(boost::uint64_t x, boost::uint64_t base)
{
	return num_digits_aux(x, base);
}


}  // namespace jewel
