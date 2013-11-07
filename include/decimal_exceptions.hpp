/*
 * Copyright 2012-2013 Matthew Harvey
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef GUARD_decimal_exceptions_hpp_7516391215620745
#define GUARD_decimal_exceptions_hpp_7516391215620745

/** \file decimal_exceptions.hpp
 *
 * \brief Exceptions to be used with jewel::Decimal class
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

#endif  // GUARD_decimal_exceptions_hpp_7516391215620745


