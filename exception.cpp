// Copyright (c) 2013, Matthew Harvey. All rights reserved.

#include "exception.hpp"
#include "array_utilities.hpp"
#include "assert.hpp"
#include "log.hpp"
#include <boost/static_assert.hpp>
#include <algorithm>
#include <cstring>
#include <iostream>
#include <iterator>
#include <string>

using jewel::num_elements;
using std::back_inserter;
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
	if (m_message.size() > max_message_size()) mark_message_as_truncated();
}

Exception::Exception(Exception const& rhs) throw():
	m_message(rhs.m_message)
{
	if (m_message.size() > max_message_size()) mark_message_as_truncated();
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
Exception::mark_message_as_truncated()
{
	m_message.resize(max_message_size());
	m_message += truncation_flag().c_str();
	JEWEL_ASSERT (m_message.size() == extended_message_capacity);
	return;
}

size_t Exception::max_message_size() throw()
{
	return extended_message_capacity - truncation_flag_capacity;
}








}  // namespace jewel
