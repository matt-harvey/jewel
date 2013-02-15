#ifndef GUARD_decimal_exceptions_hpp
#define GUARD_decimal_exceptions_hpp

/** \file decimal_exceptions.hpp
 *
 * \brief Exceptions to be used with jewel::Decimal class
 *
 * \author Matthew Harvey
 * \date 15 May 2012
 *
 * Copyright (c) 2012, Matthew Harvey. All rights reserved.
 */


#include "exception.hpp"


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

#endif  // GUARD_decimal_exceptions_hpp


