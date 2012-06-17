#ifndef GUARD_checked_arithmetic_detail_hpp
#define GUARD_checked_arithmetic_detail_hpp

/** \file checked_arithmetic_detail.hpp
 *
 * \brief Provides implementations of functions that check the
 * safety of arithmetic operations.
 *
 * This file provides implementation detail for the non-member functions in
 * checked_arithmetic.hpp, via static member functions of the class
 * CheckedArithmetic. CheckedArithmetic and its static member functions should
 * not be used directly. Client code should call the non-member wrapper
 * functions provided in checked_arithmetic.hpp.
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


namespace detail
{


/**
 * \class CheckedArithmetic
 * \brief Provides static functions that implement checking
 * the safety of arithmetic operations.
 *
 * CheckedArithmetic and its static member functions should
 * not be used directly. Client code should call the non-member wrapper
 * functions provided in checked_arithmetic.hpp.
 *
 * This class can't be instantiated.
 */
class CheckedArithmetic
{
//@cond
public:
	
	/// \name Check addition for overflow
	//@{	
	// If other types are added, the documentation for the wrapper
	// functions in checked_arithmetic.hpp needs to be updated
	// accordingly.
	static bool addition_is_unsafe(int, int);
	static bool addition_is_unsafe(long, long);
	static bool addition_is_unsafe(long long, long long);
	static bool addition_is_unsafe(short, short);
	static bool addition_is_unsafe(unsigned int, unsigned int);
	static bool addition_is_unsafe(unsigned long, unsigned long);
	static bool addition_is_unsafe(unsigned long long, unsigned long long);
	static bool addition_is_unsafe(unsigned short, unsigned short);
	//@}

	///\name Check subtraction for overflow
	//@{
	// If other types are added, the documentation for the wrapper
	// functions in checked_arithmetic.hpp needs to be updated
	// accordingly.
	static bool subtraction_is_unsafe(int, int);
	static bool subtraction_is_unsafe(long, long);
	static bool subtraction_is_unsafe(long long, long long);
	static bool subtraction_is_unsafe(short, short);
	static bool subtraction_is_unsafe(unsigned int, unsigned int);
	static bool subtraction_is_unsafe(unsigned long, unsigned long);
	static bool subtraction_is_unsafe(unsigned long long, unsigned long long);
	static bool subtraction_is_unsafe(unsigned short, unsigned short);
	//@}

	///\name Check multiplication for overflow
	//@{
	// If other types are added, the documentation for the wrapper
	// functions in checked_arithmetic.hpp needs to be updated
	// accordingly.
	static bool multiplication_is_unsafe(int, int);
	static bool multiplication_is_unsafe(long, long);
	static bool multiplication_is_unsafe(long long, long long);
	static bool multiplication_is_unsafe(short, short);
	static bool multiplication_is_unsafe(unsigned int, unsigned int);
	static bool multiplication_is_unsafe(unsigned long, unsigned long);
	static bool multiplication_is_unsafe(unsigned long long,
	  unsigned long long);
	static bool multiplication_is_unsafe(unsigned short, unsigned short);
	//@}

//@endcond
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
bool
CheckedArithmetic::multiplication_is_unsafe_signed_integral_types(T x, T y)
{
	// Deal with easy, common cases
	if ((x == 0) || (y == 0) || (x == 1) || (y == 1))
	{
		return false;
	}
	T const tmin = std::numeric_limits<T>::min();
	assert ((x != 0) && (y != 0) && (x != 1) && (y != 1));
	// Deal with case of smallest possible number
	// It's dangerous to multiply this by anything except 1 or 0!
	if ((x == tmin) || (y == tmin))
	{
		return true;
	}
	if ((x == -1) || (y == -1))
	{
		// Then multiplication is safe given tmin is not involved
		return false;
	}
	assert ((x != tmin) && (y != tmin));
	// Deal with ordinary cases
	T const tmax = std::numeric_limits<T>::max();
	assert ((x != -1) && (y != -1));  // Avoids errors with tmin below
	if (x > 0)
	{
		if (y > 0)
		{
			return y > tmax / x;
		}
		assert (y < 0);
		return y < tmin / x;
	}
	if (x < 0)
	{
		if (y < 0)
		{
			return y < tmax / x;
		}
	}
	assert ((x < 0) && (y > 0));
	return y > tmin / x;
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


}  // namespace detail

}  // namespace jewel

#endif  // GUARD_checked_arithmetic_detail_hpp

