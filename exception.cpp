// Copyright (c) 2013, Matthew Harvey. All rights reserved.

#include "exception.hpp"
#include "array_utilities.hpp"
#include "assert.hpp"
#include "log.hpp"
#include <boost/static_assert.hpp>
#include <algorithm>
#include <cstring>
#include <iostream>
#include <string>

using jewel::num_elements;
using std::copy;
using std::endl;

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
	CappedString<truncation_flag_capacity> const trunc_flag =
		truncation_flag();
	char truncated_message[message_capacity + 1];
	char* out_it = copy
	(	m_message.begin(),
		m_message.begin() + (message_capacity - trunc_flag.size()),
		truncated_message
	);
	out_it = copy
	(	trunc_flag.begin(),
		trunc_flag.end(),
		out_it
	);
	*out_it = '\0';
	m_message = truncated_message;
	return;
}

size_t Exception::max_message_size() throw()
{
	return message_capacity - truncation_flag_capacity;
}








}  // namespace jewel
