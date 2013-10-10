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

#ifndef GUARD_optional_hpp_9131848518332396
#define GUARD_optional_hpp_9131848518332396

/** \file optional.hpp
 *
 * \brief Provides functions to be used with boost::optional.
 *
 * \author Matthew Harvey
 * \date 2012-11-06
 * 
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
	JEWEL_THROW
	(	UninitializedOptionalException,
		"Attempted to access unitialized boost::optional."
	);
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



#endif  // GUARD_optional_hpp_9131848518332396
