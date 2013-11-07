/*
 * Copyright 2012-2013 Matthew Harvey
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

#include "exception.hpp"
#include "assert.hpp"
#include "log.hpp"
#include <algorithm>
#include <cstddef>
#include <cstring>
#include <iostream>
#include <iterator>
#include <string>
#include <utility>

using std::back_inserter;
using std::cerr;
using std::copy;
using std::endl;
using std::move;
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
		static_assert
		(	N > truncation_stamp_capacity,
			"In instantiation of CappedString<N>, N is too small to "
			"hold truncation stamp for jewel::Exception."
		);
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
) noexcept:
	m_message(truncate_c_str<string_capacity>(p_message)),
	m_type(truncate_c_str<string_capacity>(p_type)),
	m_function(truncate_c_str<string_capacity>(p_function)),
	m_filepath(truncate_c_str<string_capacity>(p_filepath)),
	m_line(p_line)
{
}

Exception::Exception(Exception const& rhs) noexcept:
	m_message(rhs.m_message),
	m_type(rhs.m_type),
	m_function(rhs.m_function),
	m_filepath(rhs.m_filepath),
	m_line(rhs.m_line)
{
}

Exception::Exception(Exception&& rhs) noexcept:
	m_message(move(rhs.m_message)),
	m_type(move(rhs.m_type)),
	m_function(move(rhs.m_function)),
	m_filepath(move(rhs.m_filepath)),
	m_line(move(rhs.m_line))
{
}

Exception::~Exception() noexcept
{
}

char const*
Exception::what() const noexcept
{
	return message();
}

char const*
Exception::message() const noexcept
{
	return m_message.c_str();
}

char const*
Exception::type() const noexcept
{
	return m_type.c_str();
}

char const*
Exception::function() const noexcept
{
	return m_function.c_str();
}

char const*
Exception::filepath() const noexcept
{
	return m_filepath.c_str();
}

long
Exception::line() const noexcept
{
	return m_line;
}

size_t
Exception::max_message_size() noexcept
{
	return string_capacity - truncation_stamp_capacity;
}


}  // namespace jewel
