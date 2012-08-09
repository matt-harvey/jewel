#ifndef GUARD_arithmetic_exceptions_hpp
#define GUARD_arithmetic_exceptions_hpp

/** \file arithmetic_exceptions.hpp
 *
 * \brief Exceptions to be used with jewel::Decimal class
 *
 * \author Matthew Harvey
 * \date 15 May 2012
 *
 * Exceptions to be thrown if there is an attempt to perform unsafe
 * arithmetic.

 * @todo HIGH PRIORITY Work through decimal.hpp making functions
 * throw the finer-grained exception classes derived from
 * DecimalException. Document and test accordingly. Then get rid
 * of old UnsafeArithmeticException class. Then change this header from
 * "arithmetic_exceptions.hpp" to "decimal_exception.hpp". Make sure that
 * no-one other than jewel::Decimal is using this header though!
 *
 * Copyright (c) 2012, Matthew Harvey. All rights reserved.
 */


#include "exception.hpp"
#include <stdexcept>
#include <string>


namespace jewel
{

JEWEL_DERIVED_EXCEPTION(UnsafeArithmeticException, Exception);

JEWEL_DERIVED_EXCEPTION(DecimalException, Exception);

JEWEL_DERIVED_EXCEPTION(DecimalAdditionException, DecimalException);
JEWEL_DERIVED_EXCEPTION(DecimalSubtractionException, DecimalException);
JEWEL_DERIVED_EXCEPTION(DecimalMultiplicationException, DecimalException);
JEWEL_DERIVED_EXCEPTION(DecimalDivisionException, DecimalException);
JEWEL_DERIVED_EXCEPTION(DecimalIncrementationException, DecimalException);
JEWEL_DERIVED_EXCEPTION(DecimalDecrementationException, DecimalException);
JEWEL_DERIVED_EXCEPTION(DecimalUnaryMinusException, DecimalException);
JEWEL_DERIVED_EXCEPTION(DecimalConstructionException, DecimalException);
JEWEL_DERIVED_EXCEPTION(DecimalStreamReadException, DecimalException);



}  // namespace jewel

#endif  // GUARD_arithmetic_exceptions_hpp


