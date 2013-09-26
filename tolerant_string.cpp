#include "tolerant_string.hpp"
#include "assert.hpp"
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <memory>
#include <utility>

using std::equal;
using std::move;
using std::nothrow;
using std::strcpy;
using std::strlen;
using std::unique_ptr;

namespace jewel
{

TolerantString::TolerantString():
	m_is_valid(true),
	m_len(0)
{
	JEWEL_ASSERT (!m_data);
	*m_standby = '\0';
}

TolerantString::TolerantString(char const* p_string):
	m_is_valid(true),
	m_len(0)
{
	JEWEL_ASSERT (!m_data);
	*m_standby = '\0';
	initialize_from_c_string(p_string);
}

TolerantString::TolerantString(TolerantString const& rhs):
	m_is_valid(true),
	m_len(0)
{
	JEWEL_ASSERT (!m_data);
	*m_standby = '\0';
	initialize_from_c_string(rhs.c_str());
}

TolerantString::TolerantString(TolerantString&& rhs):
	m_is_valid(move(rhs.m_is_valid)),
	m_len(move(rhs.m_len)),
	m_data(move(rhs.m_data))
{
	rhs.m_data = nullptr;
	*m_standby = '\0';
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

TolerantString&
TolerantString::operator=(TolerantString&& rhs)
{
	m_data = move(rhs.m_data);
	rhs.m_data = nullptr;	
	m_is_valid = move(rhs.m_is_valid);
	m_len = move(rhs.m_len);
	JEWEL_ASSERT (*m_standby == '\0');
	return *this;
}

TolerantString::~TolerantString()
{
}

bool
TolerantString::operator==(TolerantString const& rhs) const
{
	return
	(	(is_valid() == rhs.is_valid()) &&
		(size() == rhs.size()) &&
		equal(m_data.get(), m_data.get() + m_len, rhs.m_data.get())
	);
}

bool
TolerantString::operator!=(TolerantString const& rhs) const
{
	return !(*this == rhs);
}

char const*
TolerantString::c_str() const
{
	return m_data? m_data.get(): m_standby;
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
	m_data = nullptr;
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
	char* tmp = new (nothrow) char[m_len + 1];
	if (tmp)
	{
		m_data.reset(tmp);
		m_is_valid = true;
	}
	else
	{
		m_data.reset();
		m_is_valid = false;
	}
	strcpy(m_data.get(), p_string);
	JEWEL_ASSERT(m_standby[0] == '\0');
	return;
}



}  // namespace jewel
