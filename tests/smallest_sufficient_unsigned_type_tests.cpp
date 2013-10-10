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

