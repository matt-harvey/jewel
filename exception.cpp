// Copyright (c) 2013, Matthew Harvey. All rights reserved.

#include "exception.hpp"

#include "log.hpp"
#include <cassert>
#include <cstring>
#include <iostream>
#include <string>

using std::endl;
using std::strlen;
using std::strcpy;
using std::strncpy;
using std::string;

namespace jewel
{


char const* Exception::s_truncation_flag = "[TRUNCATED]";


Exception::Exception() throw()
{
	m_message[0] = '\0';
}

Exception::Exception(char const* p_message) throw()
{
	char const* it = p_message;
	size_t i = 0;
	while (*it != '\0' && i != max_message_size())
	{
		m_message[i++] = *it++;
	}
	m_message[i] = '\0';
	if (*it != '\0')
	{
		assert (i == max_message_size());
		assert (i + strlen(s_truncation_flag) + 1 == s_message_buffer_size);
		strcpy(m_message + i, s_truncation_flag);
		assert (strlen(m_message) + 1 == s_message_buffer_size);
	}
	assert (strlen(m_message) < s_message_buffer_size);
}

Exception::~Exception() throw()
{
}

char const* Exception::what() const throw()
{
	return m_message;
}

size_t Exception::max_message_size() throw()
{
	static const size_t ret =
		s_message_buffer_size - strlen(s_truncation_flag) - 1;

	return ret;
}

Exception::Exception(Exception const& rhs) throw()
{
	strcpy(m_message, rhs.m_message);
	assert (strlen(m_message) < s_message_buffer_size);
}









}  // namespace jewel
