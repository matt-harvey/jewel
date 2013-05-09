#ifndef GUARD_checked_arithmetic_hpp
#define GUARD_checked_arithmetic_hpp

/** \file checked_arithmetic.hpp
 * 
 * \author Matthew Harvey
 * \date 2012-06-09
 *
 * Copyright (c) 2012, Matthew Harvey. All rights reserved.
 *
 * \brief Provides functions that check the safety of arithmetic
 * operations.
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
 * unsigned int\n
 * unsigned long\n
 * unsigned long long\n
 * unsigned short\n
 * </tt>
 * @param x first number that would be added (or subtracted, or multiplied).
 * @param y second number that would be added (or subtracted, or multiplied).
 * @returns \c true if and only if it \e would be
 * unsafe to add \c x and \c y (or subtract \c y from \c x, or multiply \c x
 * by \c y) (i.e. would give rise to overflow), otherwise returns false.
 * The operation being tested is not actually performed.
 *
 * Exception safety: <em>nothrow guarantee</em> is offered by all compilable
 * instantiations of the function templates in this header.
 */

#include <boost/cstdint.hpp>
#include "checked_arithmetic_detail.hpp"


namespace jewel
{


// INTERFACE

/// \name Check addition, subtraction and multiplication 
/// operations for overflow.
 //@{
/** See documentation for file checked_arithmetic.hpp.
 */
template <typename T>
bool addition_is_unsafe(T x, T y);

/** See documentation for file checked_arithmetic.hpp.
 */
template <typename T>
bool subtraction_is_unsafe(T x, T y);

/** See documentation for file checked_arithmetic.hpp.
 */
template <typename T>
bool multiplication_is_unsafe(T x, T y);
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

//@endcond

}  // namespace jewel

#endif  // GUARD_checked_arithmetic_hpp
