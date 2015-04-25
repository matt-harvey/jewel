/*
 * Copyright 2013 Matthew Harvey
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

#include "exception.hpp"

namespace jewel
{

/// @class jewel::DecimalException
/// @extends jewel::Exception
/// @cond
JEWEL_DERIVED_EXCEPTION(DecimalException, Exception);
/// @endcond

    /// @class jewel::DecimalRangeException
    /// @extends jewel::DecimalException
    /// @cond
    JEWEL_DERIVED_EXCEPTION(DecimalRangeException, DecimalException);
    /// @endcond

    /// @class jewel::DecimalAdditionException
    /// @extends jewel::DecimalException
    /// @cond
    JEWEL_DERIVED_EXCEPTION(DecimalAdditionException, DecimalException);
    /// @endcond

    /// @class jewel::DecimalSubtractionException
    /// @extends jewel::DecimalException
    /// @cond
    JEWEL_DERIVED_EXCEPTION(DecimalSubtractionException, DecimalException);
    /// @endcond

    /// @class jewel::DecimalMultiplicationException
    /// @extends jewel::DecimalException
    /// @cond
    JEWEL_DERIVED_EXCEPTION(DecimalMultiplicationException, DecimalException);
    /// @endcond

    /// @class jewel::DecimalDivisionException
    /// @extends jewel::DecimalException
    /// @cond
    JEWEL_DERIVED_EXCEPTION(DecimalDivisionException, DecimalException);
    /// @endcond

        /// @class jewel::DecimalDivisionByZeroException
        /// @extends jewel::DecimalDivisionException
        /// @cond
        JEWEL_DERIVED_EXCEPTION
        (   DecimalDivisionByZeroException,
            DecimalDivisionException
        );
        /// @endcond

    /// @class jewel::DecimalIncrementationException
    /// @extends jewel::DecimalException
    /// @cond
    JEWEL_DERIVED_EXCEPTION(DecimalIncrementationException, DecimalException);
    /// @endcond

    /// @class jewel::DecimalDecrementationException
    /// @extends jewel::DecimalException
    /// @cond
    JEWEL_DERIVED_EXCEPTION(DecimalDecrementationException, DecimalException);
    /// @endcond

    /// @class jewel::DecimalUnaryMinusException
    /// @extends jewel::DecimalException
    /// @cond
    JEWEL_DERIVED_EXCEPTION(DecimalUnaryMinusException, DecimalException);
    /// @endcond

    /// @class jewel::DecimalFromStringException
    /// @extends jewel::DecimalException
    /// @cond
    JEWEL_DERIVED_EXCEPTION(DecimalFromStringException, DecimalException);
    /// @endcond

    /// @class jewel::DecimalStreamReadException
    /// @extends jewel::DecimalException
    /// @cond
    JEWEL_DERIVED_EXCEPTION(DecimalStreamReadException, DecimalException);
    /// @endcond

}  // namespace jewel

#endif  // GUARD_decimal_exceptions_hpp_7516391215620745


