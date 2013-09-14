// Copyright (c) 2013, Matthew Harvey. All rights reserved.

#ifndef GUARD_capped_string_hpp_6740592125216774
#define GUARD_capped_string_hpp_6740592125216774

#include "array_utilities.hpp"
#include "detail/smallest_sufficient_unsigned_type.hpp"
#include "log.hpp"
#include <algorithm>
#include <cstddef>
#include <istream>
#include <ostream>
#include <string>

namespace jewel
{


/**
 * A template for string classes encapsulating a limited length string of
 * \e char, which is safer than a C-style string, more convenient than a
 * boost::array<char, N> or a std::array<char, N>, and can be copied
 * safely.
 *
 * The string is limited to a certain length; but for purposes of
 * assessing the length of the string, and comparing strings with each
 * other, etc., only the characters up to null are considered relevant.
 *
 * No heap allocation is performed by CappedString.
 *
 * None of the member functions of this class will ever throw an exception.
 * (The only possible source of runtime failure is stack overflow or exploding
 * computer.)
 *
 * If an attempt is made to initialize a CappedString<N> from a built-in
 * string or std::string that exceeds N in length, then the resulting
 * CappedString will be truncated relative to that original string.
 * You can tell whether a CappedString is thus truncated by calling its
 * \e is_truncated() member. Internally, CappedString will always
 * null-terminate its internal char array, even if it is truncated
 * relative to the original string.
 *
 * When comparing strings using \e operator==, truncation is ignored.
 * I.e., two CappedString<N> are considered equal if they have the
 * same char contents (in the same order), even if one is truncated and
 * the other is not.
 *
 * You can obtain a pointer to the internal null-terminated char array
 * by calling \e c_str() - with caveats as per the member function in
 * std::string of the same name.
 *
 * Note the template parameter N represents the maximum number of characters
 * in the CappedString NOT including the null-terminator (which is considered
 * an implementation detail). This is the same number that is returned by
 * the member function \e capacity().
 *
 * TODO Allow a CappedString<N> to be initialized and/or copied from
 * a CappedString<M> where M != N. We might also allow two such strings
 * to be compared.
 *
 * TODO implement at(); but note this will mean we would have to qualify
 * blanket guarantee that no member functions throw.
 *
 * THOUGHTS... A major motivation for using CappedString instead of
 * std::string is
 * that std::string can throw an exception, viz. std::bad_alloc, in case of
 * memory allocation failure e.g. while being copied, whereas CappedString
 * cannot throw an exception.
 * There are contexts in which an exception being thrown would result in
 * undefined behaviour (e.g. after another exception has already been thrown),
 * and in these cases it is good to rule out even the slightest possibility
 * of an exception being thrown. Hence a non-throwing string class,
 * CappedString. However, a CappedString<N> cannot have a very large N without
 * blowing the stack and crashing the program. Crashing is better than
 * undefined behaviour but - could we have the best of both worlds by using
 * new(std::nothrow), or std::malloc, to attempt whatever dynamic allocation
 * we need without danger of an exception being thrown? If the returned
 * pointer is null, then
 * we can set the truncation flag, and try again with a lower size, etc..
 * until we allocate the largest reasonably possible amount of memory.. etc..
 * So we could achieve the non-throwing-ness of CappedString, while allowing
 * for much larger possible strings (as the upper limit on the size of
 * heap-allocable objects appears to be much larger than the upper limit on
 * the size of stack-allocable objects). BUT: CappedString is a pretty
 * simple solution, and for the use case I have in mind (error messages
 * for placement inside exception objects) we're not going to need more
 * than about a CappedString<1000> tops, so it should be fine. The
 * non-throwing heap-based solution would be more complex to implement -
 * it's not worth it.
 */
template <std::size_t N>
class CappedString
{
public:

	typedef char value_type;
	typedef value_type const* const_iterator;
	typedef value_type* iterator;
	typedef typename detail::SmallestSufficientUnsignedType<N + 1>::Result size_type;
	typedef std::ptrdiff_t difference_type;
	typedef value_type& reference;
	typedef value_type const& const_reference;
	// typedef ??? const_reverse_iterator;  // TODO <---
	// typedef ??? reverse_iterator;  // TODO <---

	CappedString();

	/**
	 * Initializes CappedString with characters up to the first null
	 * character in the array pointed to by p_string.
	 */
	CappedString(char const* p_string);

	/**
	 * If p_string is contains a null character, then even if there
	 * characters \e after that null character, these will
	 * still be copied over into the CappedString.
	 */
	explicit CappedString(std::string const& p_string);

	CappedString(CappedString const& rhs);

	// Use compiler-generated destructor.

	CappedString& operator=(CappedString const& rhs);
	bool operator==(CappedString const& rhs) const;
	bool operator!=(CappedString const& rhs) const;
	CappedString& operator+=(CappedString const& rhs);
	CappedString const operator+(CappedString const& rhs) const;
	const_reference operator[](size_type p_index) const;
	reference operator[](size_type p_index);

	const_iterator begin() const;
	iterator begin();
	const_iterator end() const;
	iterator end();

	char const* c_str() const;

	size_type capacity() const;
	size_type size() const;
	bool empty() const;

	bool is_truncated() const;

	void clear();

	/**
	 * If the CappedString has reached its capacity, then calling push_back
	 * will simply mark it as truncated, without otherwise altering its
	 * contents.
	 */
	void push_back(CappedString::value_type p_value);

	/**
	 * If CappedString is empty, behaviour is undefined. If the string is
	 * marked as truncated, then calling pop_back will cause it to /e cease
	 * being marked as truncated.
	 */
	void pop_back();

	/**
	 * Like std::string::resize(...) in behaviour. Note that if the
	 * CappedString is resized to its \e current size, then any truncation
	 * flag will be cleared, i.e. it will be marked as \e not truncated
	 * after resizing,
	 * even if it is at is capacity and was marked as truncated before
	 * the "resizing" operation.
	 * If the string is resized to a size greater than its capacity(),
	 * then it will only grow to its capacity, and will be marked as
	 * truncated.
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

/**
 * Read from an output stream.
 *
 * TODO Implement this.
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
		JEWEL_ASSERT(m_len < jewel::num_elements(m_data));
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
