#include "exception.hpp"

#include <cstring>

using std::strlen;

namespace jewel
{


char const* Exception::s_truncation_flag = "[TRUNCATED]";



Exception::Exception() throw()
{
}

Exception::Exception(char const* p_message) throw()
{
	size_t i = 0;
	for
	(	char const* it = p_message;
		*it != '\0' && i != max_message_size();
		++it, ++i
	)
	{
		m_message[i] = *it;
	}
	if (i >= max_message_size())
	{
		for
		(	char const* jt = s_truncation_flag;
			*jt != '\0';
			++jt, ++i
		)
		{
			m_message[i] = *jt;
		}
		m_message[i] = '\0';
	}
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
		s_message_buffer_size -
		strlen(s_truncation_flag);

	return ret;
}

Exception::Exception(Exception const& rhs) throw()
{
	if (&rhs != this)
	{
		size_t i = 0;
		for (char const* it = rhs.m_message; *it != '\0'; ++it, ++i)
		{
			m_message[i] = *it;
		}
		m_message[i] = '\0';
	}
}









}  // namespace jewel
