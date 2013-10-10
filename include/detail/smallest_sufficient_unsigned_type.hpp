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
 *
 * @todo Move the static_asserts into a separate test file,
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

}  // namespace detail
}  // namespace jewel

#endif  // GUARD_smallest_sufficient_unsigned_type_hpp_07093942774037804
