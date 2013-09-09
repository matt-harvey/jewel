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
using std::cerr;
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

	template <size_t N>
	CappedString<N> truncate_c_str(char const* p_string)
	{
		return
			p_string?
			truncate(CappedString<N>(p_string)):
			CappedString<N>();
	}

}  // end anonymous namespace



Exception::Exception
(	char const* p_message,
	char const* p_type,
	char const* p_function,
	char const* p_filepath,
	long p_line
) throw():
	m_message(truncate_c_str<string_capacity>(p_message)),
	m_type(truncate_c_str<string_capacity>(p_type)),
	m_function(truncate_c_str<string_capacity>(p_function)),
	m_filepath(truncate_c_str<string_capacity>(p_filepath)),
	m_line(p_line)
{
}

Exception::Exception(Exception const& rhs) throw():
	m_message(rhs.m_message),
	m_type(rhs.m_type),
	m_function(rhs.m_function),
	m_filepath(rhs.m_filepath),
	m_line(rhs.m_line)
{
}

Exception::~Exception() throw()
{
}

char const*
Exception::what() const throw()
{
	return message();
}

char const*
Exception::message() const throw()
{
	return m_message.c_str();
}

char const*
Exception::type() const throw()
{
	return m_type.c_str();
}

char const*
Exception::function() const throw()
{
	return m_function.c_str();
}

char const*
Exception::filepath() const throw()
{
	return m_filepath.c_str();
}

long
Exception::line() const throw()
{
	return m_line;
}

size_t
Exception::max_message_size() throw()
{
	return string_capacity - truncation_stamp_capacity;
}


}  // namespace jewel
