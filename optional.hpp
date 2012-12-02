#ifndef GUARD_optional_hpp
#define GUARD_optional_hpp

/** \file optional.hpp
 *
 * \brief Provides functions to be used with boost::optional.
 *
 * \author Matthew Harvey
 * \date 2012-11-06
 * 
 * Copyright (c) 2012, Matthew Harvey. All rights reserved.
 */

#include "exception.hpp"
#include <boost/optional.hpp>

namespace jewel
{


// Declarations

/**
  *
 * This provides a wrapper around boost::optional<T>::operator*.
 * Using boost::optional<T>::operator* without
 * this wrapper to obtain the value of x will result in an assertion
 * failure in the event that
 * x is uninitialized. This wrapper throws an exception instead. There
 * is a small performance penalty from using this wrapper, however.
 *
 * @returns The value stored in x
 *
 * @throws UninitializedOptionalException if x is
 * uninitialized.
 *
 * Exception safety: <em>strong guarantee</em>.
 */
template <typename T>
T
value(boost::optional<T> const& x);

/**
 * Reverts x to an uninitialized state. (If is already in an
 * uninitialized state, this has no effect.)
 *
 * Exception safety: <em>nothrow guarantee</em>, provided the
 * <em>destructor</em> of T never throws.
 */
template <typename T>
void
clear(boost::optional<T>& x);


JEWEL_DERIVED_EXCEPTION(UninitializedOptionalException, Exception);



// Implementations

template <typename T>
inline
T
value(boost::optional<T> const& x)
{
	if (x)
	{
		return *x;
	}
	throw UninitializedOptionalException();
}

template <typename T>
inline
void
clear(boost::optional<T>& x)
{
	x = boost::optional<T>();
	return;
}

}  // namespace jewel



#endif  // GUARD_optional_hpp
