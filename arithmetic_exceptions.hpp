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
 * DecimalException. (Done.) Document accordingly (done) and test
 * accordingly (not yet done). (I have tested that the base
 * DecimalException exception is thrown in the right places, but the
 * tests currently do not test the finer grained exception classes.)
 * Then change this header from
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

JEWEL_DERIVED_EXCEPTION(DecimalException, Exception);
	JEWEL_DERIVED_EXCEPTION(DecimalRangeException, DecimalException);
	JEWEL_DERIVED_EXCEPTION(DecimalAdditionException, DecimalException);
	JEWEL_DERIVED_EXCEPTION(DecimalSubtractionException, DecimalException);
	JEWEL_DERIVED_EXCEPTION(DecimalMultiplicationException, DecimalException);
	JEWEL_DERIVED_EXCEPTION(DecimalDivisionException, DecimalException);
		JEWEL_DERIVED_EXCEPTION
		(	DecimalDivisionByZeroException,
			DecimalDivisionException
		);
	JEWEL_DERIVED_EXCEPTION(DecimalIncrementationException, DecimalException);
	JEWEL_DERIVED_EXCEPTION(DecimalDecrementationException, DecimalException);
	JEWEL_DERIVED_EXCEPTION(DecimalUnaryMinusException, DecimalException);
	JEWEL_DERIVED_EXCEPTION(DecimalFromStringException, DecimalException);
	JEWEL_DERIVED_EXCEPTION(DecimalStreamReadException, DecimalException);



}  // namespace jewel

#endif  // GUARD_arithmetic_exceptions_hpp


