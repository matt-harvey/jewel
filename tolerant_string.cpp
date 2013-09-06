#include "tolerant_string.hpp"
#include "assert.hpp"
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <utility>

using std::equal;
using std::free;
using std::malloc;
using std::strcpy;
using std::strlen;

namespace jewel
{

TolerantString::TolerantString():
	m_is_valid(true),
	m_len(0),
	m_data(0)
{
	*m_standby = '\0';
}

TolerantString::TolerantString(char const* p_string):
	m_is_valid(true),
	m_len(0),
	m_data(0)
{
	*m_standby = '\0';
	initialize_from_c_string(p_string);
}

TolerantString::TolerantString(TolerantString const& rhs):
	m_is_valid(true),
	m_len(0),
	m_data(0)
{
	*m_standby = '\0';
	initialize_from_c_string(rhs.c_str());
}

TolerantString::~TolerantString()
{
	free(m_data);
	m_data = 0;
}

TolerantString&
TolerantString::operator=(TolerantString const& rhs)
{
	if (this != &rhs)
	{
		initialize_from_c_string(rhs.c_str());
	}
	JEWEL_ASSERT(m_standby[0] == '\0');
	return *this;
}

bool
TolerantString::operator==(TolerantString const& rhs) const
{
	return
	(	(is_valid() == rhs.is_valid()) &&
		(size() == rhs.size()) &&
		equal(m_data, m_data + m_len, rhs.m_data)
	);
}

char const*
TolerantString::c_str() const
{
	return m_data? m_data: m_standby;
}

TolerantString::size_type
TolerantString::size() const
{
	return m_len;
}

bool
TolerantString::empty() const
{
	return m_len == 0;
}

bool
TolerantString::is_valid() const
{
	return m_is_valid;
}

void
TolerantString::clear()
{
	free(m_data);
	m_data = 0;
	m_is_valid = true;
	m_len = 0;
	JEWEL_ASSERT(m_standby[0] == '\0');
	return;
}

void
TolerantString::swap(TolerantString& rhs)
{
	using std::swap;
	swap(m_is_valid, rhs.m_is_valid);
	swap(m_len, rhs.m_len);
	swap(m_data, rhs.m_data);
	JEWEL_ASSERT(m_standby[0] == '\0');
	JEWEL_ASSERT(rhs.m_standby[0] == '\0');
	return;
}

void
TolerantString::initialize_from_c_string(char const* p_string)
{
	m_len = strlen(p_string);
	m_data = static_cast<char*>(malloc(sizeof(char) * (m_len + 1)));
	m_is_valid = (m_data != 0);
	strcpy(m_data, p_string);
	JEWEL_ASSERT(m_standby[0] == '\0');
	return;
}



}  // namespace jewel
