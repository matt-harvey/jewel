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

