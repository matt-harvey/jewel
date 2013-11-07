/*
 * This file is part of the Jewel project and is distributed under the
 * terms of the License contained in the file LICENSE.txt distributed
 * with this package.
 * 
 * Author: Matthew Harvey <matthew@matthewharvey.net>
 *
 * Copyright (c) 2012-2013, Matthew Harvey.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef GUARD_checked_arithmetic_detail_hpp_8232921365756971
#define GUARD_checked_arithmetic_detail_hpp_8232921365756971

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
 */

#include "../assert.hpp"
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
	
	/// \name Check addition
	//@{	
	// If other types are added, the documentation for the wrapper
	// functions in checked_arithmetic.hpp needs to be updated
	// accordingly.
	static bool addition_is_unsafe(int, int);
	static bool addition_is_unsafe(long, long);
	static bool addition_is_unsafe(long long, long long);
	static bool addition_is_unsafe(short, short);
	static bool addition_is_unsafe(signed char, signed char);
	static bool addition_is_unsafe(unsigned int, unsigned int);
	static bool addition_is_unsafe(unsigned long, unsigned long);
	static bool addition_is_unsafe(unsigned long long, unsigned long long);
	static bool addition_is_unsafe(unsigned short, unsigned short);
	static bool addition_is_unsafe(unsigned char, unsigned char);
	//@}

	///\name Check subtraction
	//@{
	// If other types are added, the documentation for the wrapper
	// functions in checked_arithmetic.hpp needs to be updated
	// accordingly.
	static bool subtraction_is_unsafe(int, int);
	static bool subtraction_is_unsafe(long, long);
	static bool subtraction_is_unsafe(long long, long long);
	static bool subtraction_is_unsafe(short, short);
	static bool subtraction_is_unsafe(signed char, signed char);
	static bool subtraction_is_unsafe(unsigned int, unsigned int);
	static bool subtraction_is_unsafe(unsigned long, unsigned long);
	static bool subtraction_is_unsafe(unsigned long long, unsigned long long);
	static bool subtraction_is_unsafe(unsigned short, unsigned short);
	static bool subtraction_is_unsafe(unsigned char, unsigned char);
	//@}

	///\name Check multiplication
	//@{
	// If other types are added, the documentation for the wrapper
	// functions in checked_arithmetic.hpp needs to be updated
	// accordingly.
	static bool multiplication_is_unsafe(int, int);
	static bool multiplication_is_unsafe(long, long);
	static bool multiplication_is_unsafe(long long, long long);
	static bool multiplication_is_unsafe(short, short);
	static bool multiplication_is_unsafe(signed char, signed char);
	static bool multiplication_is_unsafe(unsigned int, unsigned int);
	static bool multiplication_is_unsafe(unsigned long, unsigned long);
	static bool multiplication_is_unsafe
	(	unsigned long long,
		unsigned long long
	);
	static bool multiplication_is_unsafe(unsigned short, unsigned short);
	static bool multiplication_is_unsafe(unsigned char, unsigned char);
	//@}

	///\name Check division
	//@{
	// If other types are added, the documentation for the wrapper
	// functions in checked_arithmetic.hpp needs to be updated
	// accordingly.
	static bool division_is_unsafe(int, int);
	static bool division_is_unsafe(long, long);
	static bool division_is_unsafe(long long, long long);
	static bool division_is_unsafe(short, short);
	static bool division_is_unsafe(signed char, signed char);
	static bool division_is_unsafe(unsigned int, unsigned int);
	static bool division_is_unsafe(unsigned long, unsigned long);
	static bool division_is_unsafe
	(	unsigned long long,
		unsigned long long
	);
	static bool division_is_unsafe(unsigned short, unsigned short);
	static bool division_is_unsafe(unsigned char, unsigned char);
	//@}

	///\name Check remainder
	//@{
	// If other types are added, the documentation for the wrapper
	// functions in checked_arithmetic.hpp needs to be updated
	// accordingly.
	static bool remainder_is_unsafe(int, int);
	static bool remainder_is_unsafe(long, long);
	static bool remainder_is_unsafe(long long, long long);
	static bool remainder_is_unsafe(short, short);
	static bool remainder_is_unsafe(signed char, signed char);
	static bool remainder_is_unsafe(unsigned int, unsigned int);
	static bool remainder_is_unsafe(unsigned long, unsigned long);
	static bool remainder_is_unsafe
	(	unsigned long long,
		unsigned long long
	);
	static bool remainder_is_unsafe(unsigned short, unsigned short);
	static bool remainder_is_unsafe(unsigned char, unsigned char);
	//@}

//@endcond
private:

	// Private, unimplemented constructor, to prevent this class from being
	// instantiated
	CheckedArithmetic();


	// The following "..._is_unsafe" function templates are
	// deliberately unimplemented. The intention is to capture any
	// arguments passed to the overloaded "..._is_unsafe" functions
	// that do not \e exactly match the types explicitly provided for
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

	template <typename T>
	static bool remainder_is_unsafe(T, T);


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
	subtraction_is_unsafe_signed_integral_types(T, T);

	template <typename T> static bool
	subtraction_is_unsafe_unsigned_integral_types(T, T);

	template <typename T> static bool
	multiplication_is_unsafe_signed_integral_types(T, T);

	template <typename T> static bool
	multiplication_is_unsafe_unsigned_integral_types(T, T);

	template <typename T> static bool
	division_is_unsafe_signed_integral_types(T, T);

	template <typename T> static bool
	division_is_unsafe_unsigned_integral_types(T, T);

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
	if (x > 0)
	{
		if (y > 0)
		{
			return x > (std::numeric_limits<T>::max() / y);
		}
		else
		{
			return y < (std::numeric_limits<T>::min() / x);
		}
	}
	else
	{
		if (y > 0)
		{
			return x < (std::numeric_limits<T>::min() / y);
		}
		else
		{
			return (x != 0) && (y < (std::numeric_limits<T>::max() / x));
		}
	}
	JEWEL_HARD_ASSERT (false);
}

template <typename T>
inline
bool
CheckedArithmetic::multiplication_is_unsafe_unsigned_integral_types(T x, T y)
{
	if ((x == 0) || (y == 0))
	{
		return false;
	}
	JEWEL_ASSERT (x > 0);
	JEWEL_ASSERT (y > 0);
	return x > (std::numeric_limits<T>::max() / y);
}

template <typename T>
inline
bool
CheckedArithmetic::division_is_unsafe_signed_integral_types(T x, T y)
{
	return
		(y == 0) ||
		((y == -1) && (x == std::numeric_limits<T>::min()));
}

template <typename T>
inline
bool
CheckedArithmetic::division_is_unsafe_unsigned_integral_types(T x, T y)
{
	(void)x;  // Silence compiler warning re. unused variable.
	return y == 0;
}

}  // namespace detail
}  // namespace jewel

#endif  // GUARD_checked_arithmetic_detail_hpp_8232921365756971

