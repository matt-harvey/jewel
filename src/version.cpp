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

#include "version.hpp"
#include "assert.hpp"

namespace jewel
{

bool
operator==(Version const& lhs, Version const& rhs)
{
	return
		(lhs.major() == rhs.major()) &&
		(lhs.minor() == rhs.minor()) &&
		(lhs.patch() == rhs.patch());
}

bool
operator!=(Version const& lhs, Version const& rhs)
{
	return !(lhs == rhs);
}

bool
operator<(Version const& lhs, Version const& rhs)
{
	if (lhs.major() < rhs.major())
	{
		return true;
	}
	if (lhs.major() > rhs.major())
	{
		return false;
	}
	JEWEL_ASSERT (lhs.major() == rhs.major());
	if (lhs.minor() < rhs.minor())
	{
		return true;
	}
	if (lhs.minor() > rhs.minor())
	{
		return false;
	}
	JEWEL_ASSERT (lhs.minor() == rhs.minor());
	if (lhs.patch() < rhs.patch())
	{
		return true;
	}
	JEWEL_ASSERT (lhs.major() == rhs.major());
	JEWEL_ASSERT (lhs.minor() == rhs.minor());
	JEWEL_ASSERT (lhs.patch() >= rhs.patch());
	return false;	
}

bool
operator<=(Version const& lhs, Version const& rhs)
{
	return (lhs < rhs) || (lhs == rhs);
}

bool
operator>(Version const& lhs, Version const& rhs)
{
	return rhs < lhs;	
}

bool
operator>=(Version const& lhs, Version const& rhs)
{
	return (rhs < lhs) || (rhs == lhs);
}

}  // namespace jewel
