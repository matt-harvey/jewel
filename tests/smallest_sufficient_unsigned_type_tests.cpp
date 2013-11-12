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

#include "detail/smallest_sufficient_unsigned_type.hpp"
#include <type_traits>

using std::is_same;

namespace jewel
{
namespace detail
{

static_assert
(	is_same
		<	SmallestSufficientUnsignedType<5>::Result,
			unsigned char
		>::value,
	"Unexpected result when instantiating SmallestSufficientUnsignedType."
);

static_assert
(	is_same
		<	SmallestSufficientUnsignedType<UINT_MAX>::Result,
			unsigned int
		>::value,
	"Unexpected result when instantiating SmallestSufficientUnsignedType."
);

static_assert
(	is_same
		<	SmallestSufficientUnsignedType<USHRT_MAX>::Result,
			unsigned short
		>::value,
	"Unexpected result when instantiating SmallestSufficientUnsignedType."
);

static_assert
(	is_same
		<	SmallestSufficientUnsignedType<500>::Result,
			unsigned short
		>::value,
	"Unexpected result when instantiating SmallestSufficientUnsignedType."
);

static_assert
(	is_same
		<	SmallestSufficientUnsignedType<0>::Result,
			unsigned char
		>::value,
	"Unexpected result when instantiating SmallestSufficientUnsignedType."
);

}  // namespace detail
}  // namespace jewel

