#include "checked_arithmetic_detail.hpp"
#include <boost/cstdint.hpp>

namespace jewel
{

namespace detail
{

bool CheckedArithmetic::addition_is_unsafe(int x, int y)
{
	return addition_is_unsafe_signed_integral_types(x, y);
}

/*
bool CheckedArithmetic::addition_is_unsafe(long x, long y)
{
	return addition_is_unsafe_signed_integral_types(x, y);
}
*/

bool CheckedArithmetic::addition_is_unsafe(boost::int64_t x, boost::int64_t y)
{
	return addition_is_unsafe_signed_integral_types(x, y);
}

bool CheckedArithmetic::addition_is_unsafe(short x, short y)
{
	return addition_is_unsafe_signed_integral_types(x, y);
}

bool CheckedArithmetic::addition_is_unsafe(unsigned int x, unsigned int y)
{
	return addition_is_unsafe_unsigned_integral_types(x, y);
}

/*
bool CheckedArithmetic::addition_is_unsafe(unsigned long x, unsigned long y)
{
	return addition_is_unsafe_unsigned_integral_types(x, y);
}
*/

bool
CheckedArithmetic::addition_is_unsafe(boost::uint64_t x,
  boost::uint64_t y)
{
	return addition_is_unsafe_unsigned_integral_types(x, y);
}

bool CheckedArithmetic::addition_is_unsafe(unsigned short x, unsigned short y)
{
	return addition_is_unsafe_unsigned_integral_types(x, y);
}

bool CheckedArithmetic::subtraction_is_unsafe(int x, int y)
{
	return subtraction_is_unsafe_signed_integral_types(x, y);
}

/*
bool CheckedArithmetic::subtraction_is_unsafe(long x, long y)
{
	return subtraction_is_unsafe_signed_integral_types(x, y);
}
*/

bool CheckedArithmetic::subtraction_is_unsafe
(	boost::int64_t x,
	boost::int64_t y
)
{
	return subtraction_is_unsafe_signed_integral_types(x, y);
}

bool CheckedArithmetic::subtraction_is_unsafe(short x, short y)
{
	return subtraction_is_unsafe_signed_integral_types(x, y);
}

bool CheckedArithmetic::subtraction_is_unsafe(unsigned int x, unsigned int y)
{
	return subtraction_is_unsafe_unsigned_integral_types(x, y);
}

/*
bool
CheckedArithmetic::subtraction_is_unsafe(unsigned long x, unsigned long y)
{
	return subtraction_is_unsafe_unsigned_integral_types(x, y);
}
*/

bool
CheckedArithmetic::subtraction_is_unsafe(boost::uint64_t x,
  boost::uint64_t y)
{
	return subtraction_is_unsafe_unsigned_integral_types(x, y);
}

bool
CheckedArithmetic::subtraction_is_unsafe(unsigned short x, unsigned short y)
{
	return subtraction_is_unsafe_unsigned_integral_types(x, y);
}

bool CheckedArithmetic::multiplication_is_unsafe(int x, int y)
{
	return multiplication_is_unsafe_signed_integral_types(x, y);
}

/*
bool CheckedArithmetic::multiplication_is_unsafe(long x, long y)
{
	return multiplication_is_unsafe_signed_integral_types(x, y);
}
*/

bool CheckedArithmetic::multiplication_is_unsafe
(	boost::int64_t x,
	boost::int64_t y
)
{
	return multiplication_is_unsafe_signed_integral_types(x, y);
}

bool CheckedArithmetic::multiplication_is_unsafe(short x, short y)
{
	return multiplication_is_unsafe_signed_integral_types(x, y);
}

bool
CheckedArithmetic::multiplication_is_unsafe(unsigned int x, unsigned int y)
{
	return multiplication_is_unsafe_unsigned_integral_types(x, y);
}

/*
bool
CheckedArithmetic::multiplication_is_unsafe(unsigned long x, unsigned long y)
{
	return multiplication_is_unsafe_unsigned_integral_types(x, y);
}
*/

bool
CheckedArithmetic::multiplication_is_unsafe(boost::uint64_t x,
  boost::uint64_t y)
{
	return multiplication_is_unsafe_unsigned_integral_types(x, y);
}

bool
CheckedArithmetic::
  multiplication_is_unsafe(unsigned short x, unsigned short y)
{
	return multiplication_is_unsafe_unsigned_integral_types(x, y);
}


}  // namespace detail

}  // namespace jewel


