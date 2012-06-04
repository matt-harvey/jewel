#ifndef GUARD_checked_arithmetic_hpp
#define GUARD_checked_arithmetic_hpp

/** \file checked_arithmetic.hpp
 *
 * \brief Provides functions that check the safety of arithmetic operations.
 *
 * \author Matthew Harvey
 * \date 2012-05-13
 * 
 * Copyright (c) 2012, Matthew Harvey. All rights reserved.
 */

#include <cassert>
#include <climits>
#include <cstdlib>
#include <cmath>
#include <limits>

namespace jewel
{





/**
 * \class CheckedArithmetic
 * \brief Provides functions that check the safety of arithmetic
 * operations.
 *
 * This class provides a number of static functions which check whether
 * certain operations would be safe to perform.
 * 
 * Note these functions can only be passed arguments of the types explicitly
 * provided for in their signatures. Any attempt to pass another type will
 * result in a compiler error, and will \e never result in the arguments being
 * automatically converted to a provided-for type. This is deliberate as the
 * correctness of the checks depends on the exact types of the arguments being
 * preserved.
 *
 * This class can't be instantiated.
 *
 * @todo Ensure unit tests for CheckedArithmetic::multiplication_is_unsafe
 * cover every branch (combinations of positives and negatives) in the
 * function. Think carefully through each of the cases, working through
 * examples with a pen and paper. This function needs to be rock solid!
 */

/*
Note function declarations containing long long have not been provided because
this type is not in the C++ standard prior to C++11 (even though it is in
C99 and most compilers support it).
*/

class CheckedArithmetic
{
public:
	/// \name Check addition for overflow
	//@{	
	static bool addition_is_unsafe(int, int);
	static bool addition_is_unsafe(long, long);
	static bool addition_is_unsafe(short, short);
	static bool addition_is_unsafe(unsigned int, unsigned int);
	static bool addition_is_unsafe(unsigned long, unsigned long);
	static bool addition_is_unsafe(unsigned short, unsigned short);
	//@}

	///\name Check subtraction for overflow
	//@{
	static bool subtraction_is_unsafe(int, int);
	static bool subtraction_is_unsafe(long, long);
	static bool subtraction_is_unsafe(short, short);
	static bool subtraction_is_unsafe(unsigned int, unsigned int);
	static bool subtraction_is_unsafe(unsigned long, unsigned long);
	static bool subtraction_is_unsafe(unsigned short, unsigned short);
	//@}

	///\name Check multiplication for overflow
	//@{
	static bool multiplication_is_unsafe(int, int);
	static bool multiplication_is_unsafe(long, long);
	static bool multiplication_is_unsafe(short, short);
	static bool multiplication_is_unsafe(unsigned int, unsigned int);
	static bool multiplication_is_unsafe(unsigned long, unsigned long);
	static bool multiplication_is_unsafe(unsigned short, unsigned short);
	//@}

private:

	// Private, unimplemented constructor, to prevent this class from being
	// instantiated
	CheckedArithmetic();


	// The following "..._is_unsafe" function templates are
	// deliberately unimplemented. The intention is to capture any
	// arguments passed to the overloaded "..._is_unsafe" functions
	// that do not _exactly_ match the types explicitly provided for
	// in the overloaded public "..._is_unsafe" functions. A compiler
	// error will then be raised, in virtue of a private function
	// being called.
	
	template <typename T>
	static bool addition_is_unsafe(T, T);

	template <typename T>
	static bool subtraction_is_unsafe(T, T);

	template <typename T>
	static bool multiplication_is_unsafe(T, T);

	template <typename T>
	static bool division_is_unsafe(T, T);


	// The following function templates are called by the public
	// functions of CheckedArithmetic, and contain the detailed
	// implementations of the checks. For each operation there may be
	// several function templates; the one that is called depends
	// on the type of the arguments being checked (whether signed,
	// unsigned or etc.).

	template <typename T> static bool
	addition_is_unsafe_signed_integral_types(T, T);

	template <typename T> static bool
	addition_is_unsafe_unsigned_integral_types(T, T);

	template <typename T> static bool
	addition_is_unsafe_floating_point_types(T, T);

	template <typename T> static bool
	subtraction_is_unsafe_signed_integral_types(T, T);

	template <typename T> static bool
	subtraction_is_unsafe_unsigned_integral_types(T, T);

	template <typename T> static bool
	subtraction_is_unsafe_floating_point_types(T, T);

	template <typename T> static bool
	multiplication_is_unsafe_signed_integral_types(T, T);

	template <typename T> static bool
	multiplication_is_unsafe_unsigned_integral_types(T, T);

	template <typename T> static bool
	multiplication_is_unsafe_floating_point_types(T, T);

	template <typename T> static bool
	division_is_unsafe_signed_integral_types(T, T);

	template <typename T> static bool
	division_is_unsafe_unsigned_integral_types(T, T);

	template <typename T> static bool
	division_is_unsafe_floating_point_types(T, T);

	// These function templates calculate highest significant bit
	// of the absolute value of their argument. These are called by
	// some of the checking functions above. Which one is called depends
	// on the types of the arguments.

	template <typename T> static size_t
	highest_bit_unsigned_integral_types(T);

	template <typename T> static size_t
	highest_bit_signed_integral_types(T);

};  // class CheckedArithmetic



// IMPLEMENTATIONS OF TEMPLATE AND INLINE FUNCTIONS

template <typename T>
inline
bool CheckedArithmetic::addition_is_unsafe_signed_integral_types(T x, T y)
{
	return
	  ((y > 0) && (x > std::numeric_limits<T>::max() - y)) ||
	  ((y < 0) && (x < std::numeric_limits<T>::min() - y));
}

template <typename T>
inline
bool CheckedArithmetic::addition_is_unsafe_unsigned_integral_types(T x, T y)
{
	return (x > std::numeric_limits<T>::max() - y);
}

template <typename T>
inline
bool CheckedArithmetic::subtraction_is_unsafe_signed_integral_types(T x, T y)
{
	return
	  ((y < 0) && (x > std::numeric_limits<T>::max() + y)) ||
	  ((y > 0) && (x < std::numeric_limits<T>::min() + y));
}

template <typename T>
inline
bool
CheckedArithmetic::subtraction_is_unsafe_unsigned_integral_types(T x, T y)
{
	return (x < std::numeric_limits<T>::min() + y);
}

template <typename T>
inline
bool
CheckedArithmetic::multiplication_is_unsafe_signed_integral_types(T x, T y)
{
	// Deal with easy, common cases
	if ((x == 0) || (y == 0) || (x == 1) || (y == 1))
	{
		return false;
	}
	T const tmin = std::numeric_limits<T>::min();
	// Deal with case of smallest possible number
	// It's dangerous to multiply this by anything except 1 or 0!
	if (x == tmin)
	{
		return (y != 0) && (y != 1);
	}
	if (y == tmin)
	{
		return (x != 0) && (y != 1);
	}
	// Deal with ordinary cases
	assert (x != 0);
	assert (y != 0);
	T const tmax = std::numeric_limits<T>::max();
	if (x > 0)
	{
		if (y > 0)
		{
			return tmax / x < y;
		}
		assert (y < 0);
		return tmin / x > y;
	}
	if (x < 0)
	{
		if (y < 0)
		{
			return tmax / -x < -y;
		}
	}
	assert ((x < 0) && (y > 0));
	return tmin / -x > -y;
}

template <typename T>
inline
bool
CheckedArithmetic::multiplication_is_unsafe_unsigned_integral_types(T x, T y)
{
	// Deal with easy, common cases 
	if ((x == 0) || (y == 0) || (x == 1) || (y == 1))
	{
		return false;
	}
	// Deal with general cases
	assert (x != 0);
	assert (y != 0);
	return std::numeric_limits<T>::max() / x < y;
}

template <typename T>
size_t CheckedArithmetic::highest_bit_signed_integral_types(T x)
{
	
	if (x < 0)
	{
		// The shenanigans here are required because we
		// want the absolute value of x, but we can't
		// get it if x is the least possible (negative)
		// value - so we settle for the _greatest_ possible
		// (positive) value.
		if (x != std::numeric_limits<T>::min())
		{
			x = -x;
		}
		else
		{
			x = std::numeric_limits<T>::max();
		}
	}
	size_t bits = 0;
	while (x != 0)
	{
		++bits;
		x >>= 1;
	}
	return bits;
}

template <typename T>
size_t CheckedArithmetic::highest_bit_unsigned_integral_types(T x)
{
	size_t bits = 0;
	while (x != 0) 
	{
		++bits;
		x >>= 1;
	}
	return bits;
}


}  // namespace jewel

#endif  // GUARD_checked_arithmetic_hpp

