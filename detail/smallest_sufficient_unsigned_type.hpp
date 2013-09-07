// Copyright (c) 2013, Matthew Harvey. All rights reserved.

#ifndef GUARD_smallest_sufficient_unsigned_type_hpp
#define GUARD_smallest_sufficient_unsigned_type_hpp

#include <boost/static_assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include <cstddef>
#include <climits>


namespace jewel
{
namespace detail
{

// Client code can ignore what's in detail namespace.
// See below for API viz. SmallestSufficientUnsignedType template.

namespace detail
{

template <typename T>
struct UnsignedTypeTraits
{
};

template <>
struct UnsignedTypeTraits<unsigned char>
{
	enum { max = UCHAR_MAX };
	typedef unsigned short NextLargestType;
};

template <>
struct UnsignedTypeTraits<unsigned short>
{
	enum { max = USHRT_MAX };
	typedef unsigned int NextLargestType;
};

template <>
struct UnsignedTypeTraits<unsigned int>
{
	enum { max = UINT_MAX };
	typedef unsigned long NextLargestType;
};

template <>
struct UnsignedTypeTraits<unsigned long>
{
	enum { max = ULONG_MAX };
	typedef unsigned long long NextLargestType;
};

template <>
struct UnsignedTypeTraits<unsigned long long>
{
	enum { max = ULLONG_MAX };
};

template <bool test, typename ResultIfTrue, typename ResultIfFalse>
struct Ternary
{
	typedef ResultIfTrue Result;
};

template <typename ResultIfTrue, typename ResultIfFalse>
struct Ternary<false, ResultIfTrue, ResultIfFalse>
{
	typedef ResultIfFalse Result;
};

}  // namespace detail


/**
 * If you instantiate this with a \e size_t N, then
 * <em>SmallestSufficientUnsignedType<N>::Result</em> will be a typedef
 * for the smallest unsigned type capable of holding N.
 *
 * For example, SmallestSufficientUnsignedType<300>::Result will
 * be a typedef for <em>unsigned short</em>
 *
 * @todo Move the BOOST_STATIC_ASSERTS into a separate test file,
 * and expand them to be more comprehensive.
 */
template <size_t N, typename Begin = unsigned char>
struct SmallestSufficientUnsignedType
{
	typedef typename detail::Ternary
		<	(N <= detail::UnsignedTypeTraits<Begin>::max),
			Begin,
			typename SmallestSufficientUnsignedType
			<	N,
				typename detail::UnsignedTypeTraits<Begin>::NextLargestType
			>::Result
		>::Result
		Result;
};

template <size_t N>
struct SmallestSufficientUnsignedType<N, size_t>
{
	typedef size_t Result;
};


BOOST_STATIC_ASSERT
((	boost::is_same
	<	SmallestSufficientUnsignedType<5>::Result,
		unsigned char
	>::value
));

BOOST_STATIC_ASSERT
((	boost::is_same
	<	SmallestSufficientUnsignedType<UINT_MAX>::Result,
		unsigned int
	>::value
));

BOOST_STATIC_ASSERT
((	boost::is_same
	<	SmallestSufficientUnsignedType<USHRT_MAX>::Result,
		unsigned short
	>::value
));

BOOST_STATIC_ASSERT
((	boost::is_same
	<	SmallestSufficientUnsignedType<500>::Result,
		unsigned short
	>::value
));

BOOST_STATIC_ASSERT
((	boost::is_same
	<	SmallestSufficientUnsignedType<0>::Result,
		unsigned char
	>::value
));


}  // namespace detail
}  // namespace jewel

#endif  // GUARD_smallest_sufficient_unsigned_type_hpp
