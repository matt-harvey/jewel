// Copyright (c) 2013, Matthew Harvey. All rights reserved.

#include "exception.hpp"
#include "array_utilities.hpp"
#include "assert.hpp"
#include "log.hpp"
#include <boost/static_assert.hpp>
#include <algorithm>
#include <cstddef>
#include <cstring>
#include <iostream>
#include <iterator>
#include <string>

using jewel::num_elements;
using std::back_inserter;
using std::copy;
using std::endl;
using std::size_t;
using std::strlen;

namespace jewel
{

namespace
{
	enum
	{
		truncation_stamp_capacity = 11
	};

	template <size_t N>
	CappedString<N> truncate(CappedString<N> p_string)
	{
		static CappedString<truncation_stamp_capacity> const stamp =
			"(TRUNCATED)";
		JEWEL_ASSERT (!stamp.is_truncated());
		BOOST_STATIC_ASSERT((N > truncation_stamp_capacity));	
		p_string.resize(N - truncation_stamp_capacity);
		p_string += stamp.c_str();
		JEWEL_ASSERT (!p_string.is_truncated());
		JEWEL_ASSERT (p_string.size() == p_string.capacity());
		return p_string;
	}

}  // end anonymous namespace


Exception::Exception() throw()
{
}

Exception::Exception(char const* p_message) throw():
	m_message(truncate(CappedString<extended_message_capacity>(p_message)))
{
}

Exception::Exception(Exception const& rhs) throw(): m_message(rhs.m_message)
{
}

Exception::~Exception() throw()
{
}

char const* Exception::what() const throw()
{
	return m_message.c_str();
}

size_t Exception::max_message_size() throw()
{
	return extended_message_capacity - truncation_stamp_capacity;
}








}  // namespace jewel
