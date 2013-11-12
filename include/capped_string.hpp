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

#ifndef GUARD_capped_string_hpp_6740592125216774
#define GUARD_capped_string_hpp_6740592125216774


#include "detail/smallest_sufficient_unsigned_type.hpp"
#include "log.hpp"
#include <algorithm>
#include <cstddef>
#include <istream>
#include <iterator>
#include <ostream>
#include <string>

namespace jewel
{


/**
 * @class CappedString.
 *
 * @brief Provides for a limited length string of char that avoids heap
 * allocation.
 *
 * A template for string classes encapsulating a limited length string of
 * \e char, which is safer than a C-style string, more convenient than a
 * boost::array<char, N> or a std::array<char, N>, and can be copied
 * safely. The string is limited to a certain maximum length, which is
 * provided to the template parameter N.
 *
 * No heap allocation is performed by CappedString. This avoids the
 * possibility of std::bad_alloc being thrown during construction or
 * copying. This allows most or all member functions to offer the
 * no-throw guarantee (see documentation of individual functions to confirm
 * specific exception guarantees). Thus CappedString may be preferred over
 * std::string in situations where the no-throw guarantee is important, and
 * where there is reasonable upper limit to the size of the string
 * (and this limit is not so large as to risk blowing the stack).
 *
 * If an attempt is made to initialize a CappedString<N> from a built-in
 * string or std::string that exceeds N in length, then the resulting
 * CappedString will be truncated relative to that original string.
 * You can tell whether a CappedString is thus truncated by calling its
 * \e is_truncated() member. (Internally, CappedString will always
 * null-terminate its internal char array, even if it is truncated
 * relative to the original string.)
 *
 * @todo LOW PRIORITY Allow a CappedString<N> to be initialized and/or copied
 * from a CappedString<M> where M != N. We might also allow two such strings
 * to be compared.
 *
 * @todo LOW PRIORITY implement at().
 */
template <std::size_t N>
class CappedString
{
public:

	typedef char value_type;
	typedef value_type const* const_iterator;
	typedef value_type* iterator;

	/**
	 * Depending on the value of \e N, it is possible that size_type is
	 * <em> unsigned char</em>. Thus when an instance of \e size_type is
	 * output to a stream, it may appear not as a number but rather as
	 * a character - which will not usually be the desired result.
	 * Therefore it is recommended to cast \e size_type to an explicitly
	 * numeric type before printing to a stream.
	 */
	typedef
		typename detail::SmallestSufficientUnsignedType<N + 1>::Result
		size_type;

	typedef std::ptrdiff_t difference_type;
	typedef value_type& reference;
	typedef value_type const& const_reference;
	// typedef ??? const_reverse_iterator;  // TODO LOW PRIORITY
	// typedef ??? reverse_iterator;  // TODO LOW PRIORITY

	/**
	 * Constructs an empty CappedString.
	 *
	 * Never throws.
	 */
	CappedString();

	/**
	 * Initializes CappedString with characters up to the first null
	 * character in the array pointed to by p_string.
	 *
	 * Never throws.
	 */
	CappedString(char const* p_string);

	/**
	 * If p_string is contains a null character, then even if there
	 * characters \e after that null character, these will
	 * still be copied over into the CappedString.
	 *
	 * Never throws.
	 */
	explicit CappedString(std::string const& p_string);

	/**
	 * Copy constructor.
	 *
	 * Never throws.
	 */
	CappedString(CappedString const& rhs);

	/**
	 * Assignment.
	 *
	 * Never throws.
	 */
	CappedString& operator=(CappedString const& rhs);

	// Move constructor and move assignment are deliberately
	// undeclared.

	/**
	 * Never throws.
	 */
	~CappedString() = default;

	/** Equality.
	 *
	 * CappedStrings compare equal if their contents are the
	 * same, even if one is truncated and the other isn't.
	 *
	 * Never throws.
	 */
	bool operator==(CappedString const& rhs) const;

	/**
	 * Inequality.
	 *
	 * Never throws.
	 */
	bool operator!=(CappedString const& rhs) const;

	/**
	 * Concetenate \e rhs to an existing CappedString.
	 *
	 * Never throws.
	 */
	CappedString& operator+=(CappedString const& rhs);

	/**
	 * @returns the concatenation of two CappedStrings.
	 *
	 * Never throws.
	 */
	CappedString const operator+(CappedString const& rhs) const;

	/**
	 * Read a char by indexing. Behaviour is undefined if out
	 * of range.
	 *
	 * Never throws.
	 */
	const_reference operator[](size_type p_index) const;

	/**
	 * Write a char to position p_index. Behaviour is undefined if
	 * out of range.
	 *
	 * Never throws.
	 */
	reference operator[](size_type p_index);

	/**
	 * @return a const_iterator pointing to the first character
	 * of the CappedString.
	 *
	 * Never throws.
	 */
	const_iterator begin() const;

	/**
	 * @return an iterator pointing to the first character of the
	 * CappedString.
	 *
	 * Never throws.
	 */
	iterator begin();

	/**
	 * @return a const_iterator pointing to "one past the end" of the
	 * CappedString.
	 *
	 * Never throws.
	 */
	const_iterator end() const;

	/**
	 * @return an iterator pointing to "one past the end" of the
	 * CappedString.
	 *
	 * Never throws.
	 */
	iterator end();

	/**
	 * @return a pointer to the internal char array - with caveats as
	 * per std::string.
	 *
	 * Never throws.
	 */
	char const* c_str() const;

	/**
	 * @returns the maximum number of characters that may be stored in
	 * CappedString without truncation. This is the same as the value N
	 * passed to the CappedString class template.
	 * The function is provided mainly for consistency with the standard
	 * library container interface.
	 *
	 * @see size_type
	 *
	 * Never throws.
	 */
	size_type capacity() const;

	/**
	 * @returns the length of the string currently stored in CappedString.
	 * This might be anything between 0 and capacity().
	 *
	 * @see size_type
	 *
	 * Never throws.
	 */
	size_type size() const;

	/**
	 * @returns \e true if and only if \e size() is 0.
	 *
	 * Never throws.
	 */
	bool empty() const;

	/**
	 * @returns \e true if and only if the string used to initialize the
	 * CappedString could not fit inside the CappedString given its capacity,
	 * and was therefore truncated.
	 *
	 * Never throws.
	 */
	bool is_truncated() const;

	/**
	 * Caused the CappedString to become empty.
	 *
	 * Never throws.
	 */
	void clear();

	/**
	 * Push a character onto the end of the CappedString. Normally this will
	 * increase its size by 1; but if the CappedString has already
	 * reached its capacity, then calling push_back will simply mark it as
	 * truncated, without otherwise altering its contents. If the CappedString
	 * is already marked as truncated, then calling this function will have
	 * no effect whatsoever (it will simply continue to be marked as
	 * truncated).
	 *
	 * Never throws.
	 */
	void push_back(CappedString::value_type p_value);

	/**
	 * Remove the last character from the CappedString. This will normally
	 * decrease its size by 1; however if the CappedString is empty,
	 * then behaviour is undefined. If the CappedString is
	 * marked as truncated, then calling pop_back will cause it to /e cease
	 * being marked as truncated. The last character will always be removed,
	 * whether truncated or not (provided size() if non-zero).
	 *
	 * Never throws.
	 */
	void pop_back();

	/**
	 * Like std::string::resize(...) in behaviour. Note that if the
	 * CappedString is resized to its \e current size, then this will
	 * simply have the effect that any truncation
	 * flag will be cleared, i.e. it will be marked as \e not truncated
	 * after resizing,
	 * even if it is at is capacity and was marked as truncated before
	 * the "resizing" operation.
	 * If the string is resized to a size greater than its capacity(),
	 * then it will only grow to its capacity, and will be marked as
	 * truncated.
	 *
	 * Never throws.
	 */
	void resize(size_type p_new_size);

private:

	void truncate();
	void unchecked_assign(CappedString const& rhs);
	bool m_is_truncated;
	size_type m_len;
	char m_data[N + 1];
};


}  // namespace jewel

#include "assert.hpp"

namespace jewel
{

// NON-MEMBER FUNCTIONS

/**
 * Write to an output stream.
 */
template <typename traits, std::size_t N>
std::basic_ostream<typename CappedString<N>::value_type, traits>&
operator<<
(	std::basic_ostream<typename CappedString<N>::value_type, traits>& p_os,
	CappedString<N> const& p_str
);

/*
 * Read from an input stream.
 *
 * TODO LOW PRIORITY Implement this.
template <typename traits, std::size_t N>
std::basic_istream<typename CappedString<N>::value_type, traits>&
operator>>
(	std::basic_istream<typename CappedString<N>::value_type, traits> p_is,
	CappedString<N> const& p_str
);
*/



// FUNCTION IMPLEMENTATIONS

template <std::size_t N>
inline
CappedString<N>::CappedString(): m_is_truncated(false), m_len(0)
{
	*m_data = '\0';
}

template <std::size_t N>
CappedString<N>::CappedString(char const* p_string)
{
	m_is_truncated = false;
	m_len = 0;
	for ( ; (m_data[m_len] = p_string[m_len]) != '\0'; ++m_len)
	{
		if (m_len == capacity())
		{
			truncate();
			break;
		}
	}
	return;
}

template <std::size_t N>
CappedString<N>::CappedString(std::string const& p_string):
	m_is_truncated(false)
{
	if (p_string.size() > capacity())
	{
		m_len = capacity();
		m_is_truncated = true;
	}
	else
	{
		m_len = p_string.size();
		m_is_truncated = false;
	}
	*std::copy(p_string.begin(), p_string.begin() + m_len, m_data) = '\0';
	return;
}

template <std::size_t N>
inline
CappedString<N>::CappedString(CappedString const& rhs)
{
	unchecked_assign(rhs);
}

template <std::size_t N>
inline
CappedString<N>&
CappedString<N>::operator=(CappedString const& rhs)
{
	if (&rhs != this) unchecked_assign(rhs);
	return *this;
}

template <std::size_t N>
bool
CappedString<N>::operator==(CappedString const& rhs) const
{
	return
	(	(m_len == rhs.m_len) &&
		std::equal(m_data, m_data + m_len, rhs.m_data)
	);
}

template <std::size_t N>
inline
bool
CappedString<N>::operator!=(CappedString const& rhs) const
{
	return !(*this == rhs);
}

template <std::size_t N>
CappedString<N>&
CappedString<N>::operator+=(CappedString const& rhs)
{
	size_type new_length = size() + rhs.size();
	if (new_length > capacity())
	{
		new_length = capacity();
		m_is_truncated = true;
	}
	if (rhs.is_truncated())
	{
		// Take care of the case where we are concatenating via an implicit
		// conversion from char const* on the rhs - client would expect this
		// to be marked as truncated if the char const* is longer than
		// capacity.
		JEWEL_ASSERT (rhs.capacity() == capacity());
		m_is_truncated = true;
	}
	size_type const num_extra_elements = new_length - size();
	std::copy(rhs.begin(), rhs.begin() + num_extra_elements, m_data + size());
	m_len = new_length;
	m_data[m_len] = '\0';
	return *this;
}

template <std::size_t N>
CappedString<N> const
CappedString<N>::operator+(CappedString const& rhs) const
{
	return CappedString<N>(*this) += rhs;
}

template <std::size_t N>
inline
typename CappedString<N>::const_reference
CappedString<N>::operator[](size_type p_index) const
{
	return m_data[p_index];
}

template <std::size_t N>
inline
typename CappedString<N>::reference
CappedString<N>::operator[](size_type p_index)
{
	return m_data[p_index];
}

template <std::size_t N>
inline
typename CappedString<N>::const_iterator
CappedString<N>::begin() const
{
	return m_data;
}

template <std::size_t N>
inline
typename CappedString<N>::iterator
CappedString<N>::begin()
{
	return m_data;
}

template <std::size_t N>
inline
typename CappedString<N>::const_iterator
CappedString<N>::end() const
{
	return m_data + m_len;
}

template <std::size_t N>
inline
typename CappedString<N>::iterator
CappedString<N>::end()
{
	return m_data + m_len;
}

template <std::size_t N>
inline
char const*
CappedString<N>::c_str() const
{
	return m_data;
}

template <std::size_t N>
inline
typename CappedString<N>::size_type
CappedString<N>::capacity() const
{
	return static_cast<size_type>(N);
}

template <std::size_t N>
inline
typename CappedString<N>::size_type
CappedString<N>::size() const
{
	return m_len;
}

template <std::size_t N>
inline
bool
CappedString<N>::empty() const
{
	return m_len == 0;
}

template <std::size_t N>
inline
bool
CappedString<N>::is_truncated() const
{
	return m_is_truncated;
}

template <std::size_t N>
inline
void
CappedString<N>::clear()
{
	m_data[0] = '\0';
	m_len = 0;
	m_is_truncated = false;
	return;
}

template <std::size_t N>
inline
void
CappedString<N>::push_back(value_type p_value)
{
	if (size() == capacity())
	{
		m_is_truncated = true;
	}
	else
	{
		m_data[m_len] = p_value;
		++m_len;
		JEWEL_ASSERT(m_len < (std::end(m_data) - std::begin(m_data)));
		m_data[m_len] = '\0';
	}
	JEWEL_ASSERT(size() <= capacity());
	return;
}

template <std::size_t N>
void
CappedString<N>::pop_back()
{
	--m_len;
	m_data[m_len] = '\0';
	m_is_truncated = false;
	return;
}

template <std::size_t N>
void
CappedString<N>::resize(size_type p_new_size)
{
	size_type const old_size = m_len;
	if (p_new_size > capacity())
	{
		m_is_truncated = true;
		m_len = capacity();
	}
	else
	{
		m_is_truncated = false;
		m_len = p_new_size;
	}
	if (m_len > old_size)
	{
		std::fill(m_data + old_size, end(), char());
	}
	m_data[m_len] = '\0';
	return;
}

template <std::size_t N>
void
CappedString<N>::truncate()
{
	m_data[N] = '\0';
	m_is_truncated = true;
	return;
}

template <std::size_t N>
void
CappedString<N>::unchecked_assign(CappedString const& rhs)
{
	m_is_truncated = rhs.m_is_truncated;
	m_len = rhs.m_len;

	// With compiler optimization, this appears to be as fast as memcpy,
	// is also safer in case we ever allow non-POD char types.
	std::copy(rhs.m_data, rhs.m_data + m_len + 1, m_data);

	return;
}

template <typename traits, std::size_t N>
inline
std::basic_ostream<typename CappedString<N>::value_type, traits>&
operator<<
(	std::basic_ostream<typename CappedString<N>::value_type, traits>& os,
	CappedString<N> const& str
)
{
	return os << str.c_str();
}


}  // namespace jewel

#endif   // GUARD_capped_string_hpp_6740592125216774
