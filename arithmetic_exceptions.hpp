#ifndef GUARD_arithmetic_exceptions_hpp
#define GUARD_arithmetic_exceptions_hpp

/** \file arithmetic_exceptions.hpp
 *
 * \brief Exceptions to be used with jewel::Decimal class
 *
 * \author Matthew Harvey
 * \date 15 May 2012
 *
 * Copyright (c) 2012, Matthew Harvey. All rights reserved.
 */


#include "exception_macros.hpp"
#include <stdexcept>
#include <string>


namespace jewel
{

 /**
 * Exception to be thrown if there is an attempt to perform unsafe
 * arithmetic.
 */
JEWEL_STANDARD_EXCEPTION(UnsafeArithmeticException);

}  // namespace jewel

#endif  // GUARD_arithmetic_exceptions_hpp


