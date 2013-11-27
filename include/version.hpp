/*
 * Copyright 2013 Matthew Harvey
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

#ifndef GUARD_version_hpp_3250343414605755
#define GUARD_version_hpp_3250343414605755

#include <cstddef>
#include <ostream>

namespace jewel
{

/**
 * Represents a version number for a piece of software, documentation or etc..
 *
 * <em>Exception safety</em>: All of the member and related non-member functions
 * in this
 * header offer the <em>nothrow guarantee</em>, apart from the stream output
 * operator,
 * which could throw an exception in case exceptions have been enabled on the
 * stream and an error occurs during output (unlikely).
 */
class Version
{
public:

	Version(unsigned p_major, unsigned p_minor, unsigned p_patch);
	
	Version(Version const& rhs) = default;
	Version(Version&& rhs) = default;
	Version& operator=(Version const& rhs) = default;
	Version& operator=(Version&& rhs) = default;
	~Version() = default;

	unsigned major() const;
	unsigned minor() const;
	unsigned patch() const;

private:
	unsigned m_major;
	unsigned m_minor;
	unsigned m_patch;

};  // class Version

/// @name Compare Versions by recency.
/// @relates Version
 //@{
/**
 */
bool operator==(Version const& lhs, Version const& rhs);

/**
 */
bool operator!=(Version const& lhs, Version const& rhs);

/**
 * @returns \e true if and only if \e rhs is more recent than \e lhs.
 */
bool operator<(Version const& lhs, Version const& rhs);

/**
 */
bool operator<=(Version const& lhs, Version const& rhs);

/**
 */
bool operator>(Version const& lhs, Version const& rhs);

/**
 */
bool operator>=(Version const& lhs, Version const& rhs);
//@}

/**
 * Write string representation of p_version to p_os as
 * "[major].[minor].[patch]", e.g. "2.0.1".
 */
template <typename charT, typename traits>
std::basic_ostream<charT, traits>&
operator<<(std::basic_ostream<charT, traits>& p_os, Version const& p_version);


// INLINE IMPLEMENTATIONS

inline
Version::Version(unsigned p_major, unsigned p_minor, unsigned p_patch):
	m_major(p_major),
	m_minor(p_minor),
	m_patch(p_patch)
{
}

inline
unsigned
Version::major() const
{
	return m_major;
}

inline
unsigned
Version::minor() const
{
	return m_minor;
}

inline
unsigned
Version::patch() const
{
	return m_patch;
}


// FUNCTION TEMPLATE IMPLEMENTATION

template <typename charT, typename traits>
std::basic_ostream<charT, traits>&
operator<<(std::basic_ostream<charT, traits>& p_os, Version const& p_version)
{
	charT const dot('.');
	p_os << p_version.major()
	     << dot
	     << p_version.minor()
	     << dot
	     << p_version.patch();
	return p_os;
}


}  // namespace jewel

#endif  // GUARD_version_hpp_3250343414605755
