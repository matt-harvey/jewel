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

#ifndef GUARD_smallest_sufficient_unsigned_type_hpp_07093942774037804
#define GUARD_smallest_sufficient_unsigned_type_hpp_07093942774037804


#include <cstddef>
#include <climits>
#include <type_traits>


namespace jewel
{
namespace detail
{

// Client code can ignore what's in detail namespace.
// See below for API viz. SmallestSufficientUnsignedType template.

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

/**
 * If you instantiate this with a \e size_t N, then
 * <em>SmallestSufficientUnsignedType<N>::Result</em> will be a typedef
 * for the smallest unsigned type capable of holding N.
 *
 * For example, SmallestSufficientUnsignedType<300>::Result will
 * be a typedef for <em>unsigned short</em>
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

}  // namespace detail
}  // namespace jewel

#endif  // GUARD_smallest_sufficient_unsigned_type_hpp_07093942774037804
