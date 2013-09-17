#ifndef GUARD_checked_arithmetic_hpp_7987160937854629
#define GUARD_checked_arithmetic_hpp_7987160937854629

/** \file checked_arithmetic.hpp
 * 
 * \author Matthew Harvey
 * \date 2012-06-09
 *
 * Copyright (c) 2012, Matthew Harvey. All rights reserved.
 *
 * \brief Provides functions that check the safety of arithmetic
 * operations. In the case of signed arithmetic, we are checking
 * for overflow; in the case of unsigned arithmetic, we are
 * checking for wrap-around. In the case of division and remainder
 * operations, we are also checking for divide-by-zero.
 *
 * The parameters to these functions must be of one of the supported types,
 * and must be of the same type.
 * Failure to provide the required types will result in compilation
 * failure.
 * Automatic conversion to the required types will \e never occur.
 *
 * Supported types are:\n
 * <tt>
 * int\n
 * short\n
 * long\n
 * long long\n
 * signed char\n
 * unsigned int\n
 * unsigned long\n
 * unsigned long long\n
 * unsigned short\n
 * unsigned char\n
 * </tt>
 * @param x first number that would be added (or subtracted, or etc.).
 * @param y second number that would be added (or subtracted, or etc.).
 * @returns \c true if and only if it \e would be
 * unsafe to add \c x and \c y (or subtract \c y from \c x, or multiply \c x
 * by \c y, or divide \c x by \c y) otherwise returns false.
 * The operation being tested is not actually performed.
 *
 * Exception safety: <em>nothrow guarantee</em> is offered by all compilable
 * instantiations of the function templates in this header.
 */

#include <boost/cstdint.hpp>
#include "detail/checked_arithmetic_detail.hpp"


namespace jewel
{


// INTERFACE

/// \name Check addition, subtraction, multiplication and division
/// operations for or overflow other unsafe conditions.
 //@{
/** 
 * @returns true if and only if it is unsafe to perform the
 * operation <tt>x + y</tt>.
 */
template <typename T>
bool addition_is_unsafe(T x, T y);

/**
 * @returns true if and only if it is unsafe to perform the operation
 * <tt>x - y</tt>.
 */
template <typename T>
bool subtraction_is_unsafe(T x, T y);

/**
 * @returns true if and only if it is unsafe to perform the operation
 * <tt>x * y</tt>.
 */
template <typename T>
bool multiplication_is_unsafe(T x, T y);

/**
 * @returns true if and only if is unsafe to perform the operation
 * <tt>x / y</tt>.
 */
template <typename T>
bool division_is_unsafe(T x, T y);

/** See documentation for file checked_arithmetic.hpp.
 * 
 * @returns \e true if and only if it is unsafe to perform
 * the operation <tt>x % y</tt>.
 */
template <typename T>
bool remainder_is_unsafe(T x, T y);

//@}


//@cond

// IMPLEMENTATIONS

template <typename T>
inline
bool addition_is_unsafe(T x, T y)
{
	return detail::CheckedArithmetic::addition_is_unsafe(x, y);
}

template <typename T>
inline
bool subtraction_is_unsafe(T x, T y)
{
	return detail::CheckedArithmetic::subtraction_is_unsafe(x, y);
}

template <typename T>
inline
bool multiplication_is_unsafe(T x, T y)
{
	return detail::CheckedArithmetic::multiplication_is_unsafe(x, y);
}

template <typename T>
inline
bool division_is_unsafe(T x, T y)
{
	return detail::CheckedArithmetic::division_is_unsafe(x, y);
}

template <typename T>
inline
bool remainder_is_unsafe(T x, T y)
{
	return detail::CheckedArithmetic::remainder_is_unsafe(x, y);
}

//@endcond

}  // namespace jewel

#endif  // GUARD_checked_arithmetic_hpp_7987160937854629
