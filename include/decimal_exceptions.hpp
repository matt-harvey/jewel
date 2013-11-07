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


