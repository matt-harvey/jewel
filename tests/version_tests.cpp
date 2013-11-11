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

#include "version.hpp"
#include <UnitTest++/UnitTest++.h>
#include <iterator>
#include <sstream>

using jewel::Version;
using std::ostringstream;
using std::wostringstream;
using std::begin;
using std::end;

TEST(version_basic)
{
	Version v0(1, 0, 0);
	CHECK_EQUAL(v0.major(), 1U);
	CHECK_EQUAL(v0.minor(), 0U);
	CHECK_EQUAL(v0.patch(), 0U);

	Version const v1(23, 8, 10);
	CHECK_EQUAL(v1.major(), 23U);
	CHECK_EQUAL(v1.minor(), 8U);
	CHECK_EQUAL(v1.patch(), 10U);

	Version const v2(v1);
	CHECK_EQUAL(v2.major(), v1.major());
	CHECK_EQUAL(v2.minor(), v1.minor());
	CHECK_EQUAL(v2.patch(), v1.patch());
	CHECK_EQUAL(v2.patch(), 10U);

	v0 = v2;
	CHECK_EQUAL(v0.major(), v2.major());
	CHECK_EQUAL(v0.major(), 23U);
	CHECK_EQUAL(v0.minor(), 8U);
	CHECK_EQUAL(v0.patch(), v2.patch());
}

TEST(version_output)
{
	Version const v0(12, 0, 306);
	ostringstream oss0;
	oss0 << v0;
	CHECK_EQUAL(oss0.str(), "12.0.306");

	Version const v1(0, 69, 50);
	ostringstream oss1;
	oss1 << v1;
	CHECK_EQUAL(oss1.str(), "0.69.50");

	wostringstream woss1;
	woss1 << v1;
	CHECK(woss1.str() == L"0.69.50");
}

TEST(version_comparison)
{
	CHECK (Version(0, 0, 0) < Version(1, 1, 1));
	CHECK (Version(1, 0, 0) > Version(0, 14, 0));
	CHECK (Version(30, 12, 6) == Version(30, 12, 6));
	CHECK (Version(0, 12, 3) != Version(3, 12, 0));
	CHECK (Version(3, 2, 1) <= Version(3, 2, 1));
	CHECK (Version(18, 3, 6) >= Version(17, 6, 3));
	Version const versions[] =
	{	Version(0, 0, 0),
		Version(0, 0, 1),
		Version(0, 0, 2986),
		Version(0, 1, 0),
		Version(0, 1, 1),
		Version(0, 1, 90),
		Version(1, 0, 0),
		Version(1, 9, 3),
		Version(2, 0, 5),
		Version(29768, 0, 0),
		Version(297681, 0, 0),
		Version(297681, 0, 3),
		Version(297681, 2, 0)
	};
	size_t const sz = end(versions) - begin(versions);
	for (size_t i = 0; i != sz; ++i)
	{
		for (size_t j = 0; j != sz; ++j)
		{
			if (i == j)
			{
				CHECK_EQUAL(versions[i], versions[j]);
				CHECK(versions[i] <= versions[j]);
				CHECK(versions[j] <= versions[i]);
				CHECK(versions[i] >= versions[j]);
				CHECK(versions[j] >= versions[i]);
				CHECK(!(versions[i] < versions[j]));
				CHECK(!(versions[j] < versions[i]));
				CHECK(!(versions[i] > versions[j]));
				CHECK(!(versions[i] > versions[j]));
				CHECK(!(versions[i] != versions[j]));
				CHECK(!(versions[j] != versions[i]));
			}
			else if (i < j)
			{
				CHECK(versions[i] < versions[j]);
				CHECK(versions[i] <= versions[j]);
				CHECK(versions[i] != versions[j]);
				CHECK(versions[j] > versions[i]);
				CHECK(versions[j] >= versions[i]);
				CHECK(!(versions[j] < versions[i]));
				CHECK(!(versions[j] <= versions[i]));
				CHECK(!(versions[j] == versions[i]));
				CHECK(!(versions[i] == versions[j]));
				CHECK(!(versions[i] > versions[j]));
				CHECK(!(versions[i] >= versions[j]));
			}
			else if (i > j)
			{
				CHECK(versions[j] < versions[i]);
				CHECK(versions[j] <= versions[i]);
				CHECK(versions[j] != versions[i]);
				CHECK(versions[i] > versions[j]);
				CHECK(versions[i] >= versions[j]);
				CHECK(!(versions[i] < versions[j]));
				CHECK(!(versions[i] <= versions[j]));
				CHECK(!(versions[i] == versions[j]));
				CHECK(!(versions[j] == versions[i]));
				CHECK(!(versions[j] > versions[i]));
				CHECK(!(versions[j] >= versions[i]));
			}
		}
	}
}

