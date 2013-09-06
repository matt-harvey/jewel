// Copyright (c) 2013, Matthew Harvey. All rights reserved.

#include "assert.hpp"
#include "tolerant_string.hpp"
#include <UnitTest++/UnitTest++.h>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>

using jewel::TolerantString;
using jewel::Log;
using std::cout;
using std::clog;
using std::endl;
using std::ostringstream;
using std::strcmp;
using std::strlen;


TEST(tolerant_string_default_constructor)
{
	TolerantString ts0;
	CHECK_EQUAL(ts0.c_str(), std::string().c_str());
	CHECK(ts0.empty());
	CHECK(ts0.is_valid());
	CHECK_EQUAL(ts0.size(), 0);
	CHECK_EQUAL(strlen(ts0.c_str()), 0);
	CHECK_EQUAL(strcmp(ts0.c_str(), ""), 0);
	ts0.clear();
	CHECK_EQUAL(ts0.size(), 0);
	CHECK(ts0.empty());
	CHECK(ts0.is_valid());
}

TEST(tolerant_string_constructor_from_char_const_ptr)
{
	TolerantString ts0("Hello there!");
	if (ts0.is_valid())
	{
		CHECK_EQUAL(std::string(ts0.c_str()), std::string("Hello there!"));
		CHECK_EQUAL(ts0, TolerantString("Hello there!"));
		CHECK_EQUAL(strcmp(ts0.c_str(), "Hello there!"), 0);
		CHECK_EQUAL(ts0.size(), 12);
		CHECK(!ts0.empty());
		ts0.clear();
		CHECK(ts0.empty());
		CHECK_EQUAL(std::string(ts0.c_str()), std::string());
	}
	TolerantString ts1("");
	if (ts1.is_valid())
	{
		TolerantString ts1b;
		CHECK_EQUAL(ts1, ts1b);
		CHECK_EQUAL(ts1.size(), 0);
		CHECK_EQUAL(strlen(ts1.c_str()), 0);
		CHECK_EQUAL(strcmp(ts1.c_str(), ""), 0);
	}
	TolerantString const ts2 = "Hello there!";
	if (ts2.is_valid())
	{
		CHECK_EQUAL(std::string(ts2.c_str()), std::string("Hello there!"));
		CHECK_EQUAL(ts2, TolerantString("Hello there!"));
		CHECK_EQUAL(strcmp(ts2.c_str(), "Hello there!"), 0);
		CHECK_EQUAL(ts2.size(), 12);
		CHECK(!ts2.empty());
	}
}
