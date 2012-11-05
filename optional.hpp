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
 * @returns The value stored in x
 *
 * @throws UninitializedOptionalException if x is
 * uninitialized.
 *
 * This provides a wrapper around the operator* which
 * is provided by boost::optional. Using this operator without
 * the wrapper to obtain the value of x will result in an assertion
 * failure (rather than an exception being thrown) in the event that
 * x is uninitialized. There is a small performance penalty from using
 * this wrapper, however.
 */
template <typename T>
T
value(boost::optional<T> x);


JEWEL_DERIVED_EXCEPTION(UninitializedOptionalException, Exception);



// Implementations

template <typename T>
inline
T
value(boost::optional<T> x)
{
	if (x)
	{
		return *x;
	}
	throw UninitializedOptionalException();
}


}  // namespace jewel



#endif  // GUARD_optional_hpp
