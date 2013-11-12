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

#include "assert.hpp"
#include "flag_set.hpp"
#include <UnitTest++/UnitTest++.h>
#include <iostream>

using jewel::FlagSet;
using jewel::InvalidFlagException;
using std::cout;
using std::endl;

enum Talent
{
	public_speaking = 1,
	walking = 2,
	eating = 4,
	running = 8,
	music = 16
};


TEST(flag_set_default_constructor)
{
	FlagSet<Talent, public_speaking | walking> fs0;
	CHECK_EQUAL(fs0.underlying(), static_cast<size_t>(0));
	CHECK(!fs0.test(public_speaking));
	CHECK(!fs0.test(walking));

	FlagSet
	<	Talent,
		music | music | eating | music | public_speaking,
		eating | public_speaking
	> fs1;
	CHECK(!fs1.test(music));
	CHECK(fs1.test(eating));
	CHECK(fs1.test(public_speaking));
	CHECK_EQUAL((int)fs1.underlying(), eating | public_speaking);
}

TEST(flag_set_set_test_clear_and_underlying)
{
	typedef FlagSet<Talent, eating | running | music> FS;
	FS fs0;
	CHECK(!fs0.test(eating));
	CHECK(!fs0.test(running));
	CHECK(!fs0.test(music));
	CHECK_EQUAL((int)fs0.underlying(), !eating | !running | !music);

	fs0.set(running);
	CHECK(!fs0.test(eating));
	CHECK(fs0.test(running));
	CHECK(!fs0.test(music));
	CHECK_EQUAL((int)fs0.underlying(), !eating | running | !music);

	fs0.set(music);
	CHECK(!fs0.test(eating));
	CHECK(fs0.test(running));
	CHECK(fs0.test(music));

	fs0.set(music);
	CHECK(!fs0.test(eating));
	CHECK(fs0.test(running));
	CHECK(fs0.test(music));

	fs0.clear(music);
	CHECK(!fs0.test(eating));
	CHECK(fs0.test(running));
	CHECK(!fs0.test(music));

	fs0.clear(eating).set(eating).clear(eating).set(eating);
	CHECK(fs0.test(eating));
	fs0.set(eating).set(eating).set(eating).clear(eating);
	CHECK(!fs0.test(eating));

	bool ok = false;
	try
	{
		fs0.set(public_speaking);
	}
	catch (InvalidFlagException&)
	{
		ok = true;
	}
	CHECK(ok);

	fs0.clear(static_cast<Talent>(music | eating | running));
	CHECK(!fs0.test(eating));
	CHECK(!fs0.test(running));
	CHECK(!fs0.test(music));

	fs0.set(static_cast<Talent>(music | eating));
	CHECK(fs0.test(music));
	CHECK(fs0.test(eating));
	CHECK(!fs0.test(running));

	ok = false;
	try
	{
		fs0.test(walking);
	}
	catch (InvalidFlagException&)
	{
		ok = true;
	}
	CHECK(ok);

	ok = false;
	try
	{
		fs0.clear(walking);
	}
	catch (InvalidFlagException&)
	{
		ok = true;
	}
	CHECK(ok);

	FS fs1;
	fs1.set(music);
	fs1.set(eating);
	FS::IntT i = fs1.underlying();

	FS fs2;
	fs2.set(static_cast<Talent>(i));
	CHECK(fs2.test(music));
	CHECK(fs2.test(eating));
	CHECK(!fs2.test(running));
}
