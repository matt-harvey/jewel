// Copyright (c) 2013, Matthew Harvey. All rights reserved.

#include "exception.hpp"
#include "array_utilities.hpp"
#include "assert.hpp"
#include "log.hpp"
#include <boost/static_assert.hpp>
#include <cstring>
#include <iostream>
#include <string>

using jewel::num_elements;
using std::endl;
using std::strlen;
using std::strncat;
using std::strncpy;
using std::string;

namespace jewel
{


Exception::Exception() throw()
{
}

Exception::Exception(char const* p_message) throw():
	m_message(p_message)
{
	if (m_message.size() > max_message_size()) truncate_message();
}

Exception::Exception(Exception const& rhs) throw():
	m_message(rhs.m_message)
{
	if (m_message.size() > max_message_size()) truncate_message();
}

Exception::~Exception() throw()
{
}

CappedString<Exception::truncation_flag_capacity>
Exception::truncation_flag()
{
	CappedString<truncation_flag_capacity> const ret("[TRUNCATED]");
	JEWEL_ASSERT (!ret.is_truncated());
	JEWEL_ASSERT (ret.size() == truncation_flag_capacity);
	return ret;
}

char const* Exception::what() const throw()
{
	return m_message.c_str();
}

void
Exception::truncate_message()
{
	// TODO This concatenation would be better handled within
	// CappedString.
	char buf[message_capacity + 1];
	size_t const truncation_flag_size = truncation_flag().size();
	size_t const truncated_message_size =
		message_capacity - truncation_flag_size;
	strncpy(buf, m_message.c_str(), truncated_message_size);
	buf[truncated_message_size] = '\0';
	JEWEL_ASSERT
	(	truncation_flag_size <=
		(message_capacity - truncated_message_size)
	);
	strcat(buf, truncation_flag().c_str());
	buf[message_capacity] = 1;  // Doesn't matter as long as it's not '\0';
	// We still want m_message to be marked as truncated!
	m_message = CappedString<message_capacity>(buf);
	return;
}

size_t Exception::max_message_size() throw()
{
	return message_capacity - truncation_flag_capacity;
}








}  // namespace jewel
