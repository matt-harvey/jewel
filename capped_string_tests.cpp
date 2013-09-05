// Copyright (c) 2013, Matthew Harvey. All rights reserved.

#include "assert.hpp"
#include "capped_string.hpp"
#include <UnitTest++/UnitTest++.h>
#include <cstring>
#include <string>

using jewel::CappedString;
using std::strlen;

typedef CappedString<0> CappedString0;
typedef CappedString<1> CappedString1;
typedef CappedString<5> CappedString5;
typedef CappedString<50> CappedString50;
typedef CappedString<303> CappedString303;
typedef CappedString<40938> CappedString40938;
typedef CappedString<UINT_MAX> CappedStringUIMax;

TEST(capped_string_default_constructor)
{
	CappedString0 const c0;
	CappedString1 const c1;
	CappedString5 const c5;
	CappedString50 const c50;
	CappedString303 const c303;
	CappedString40938 const c40938;

	CHECK(c0.empty());
	CHECK(c1.empty());
	CHECK(c5.empty());
	CHECK(c50.empty());
	CHECK(c303.empty());
	CHECK(c40938.empty());

	std::string const d("");
	std::string const e;
	std::string const f("");
	JEWEL_ASSERT(d == e);
	JEWEL_ASSERT(e == f);
	JEWEL_ASSERT(f == d);

	CHECK_EQUAL(c0, CappedString0(d));
	CHECK_EQUAL(c0, CappedString0(e));
	CHECK_EQUAL(c1, CappedString1(d));
	CHECK_EQUAL(c1, CappedString1(e));
	CHECK_EQUAL(c5, CappedString5(d));
	CHECK_EQUAL(c5, CappedString5(e));
	CHECK_EQUAL(c50, CappedString50(d));
	CHECK_EQUAL(c50, CappedString50(e));
	CHECK_EQUAL(c303, CappedString303(d));
	CHECK_EQUAL(c40938, CappedString40938(d));
	CHECK_EQUAL(c40938, CappedString40938(e));

	CHECK_EQUAL(c0.size(), 0);
	CHECK_EQUAL(c1.size(), 0);
	CHECK_EQUAL(c5.size(), 0);
	CHECK_EQUAL(c50.size(), 0);
	CHECK_EQUAL(c303.size(), 0);
	CHECK_EQUAL(c40938.size(), 0);
	
	CHECK_EQUAL(c0.capacity(), 0);
	CHECK_EQUAL(c1.capacity(), 1);
	CHECK_EQUAL(c5.capacity(), 5);
	CHECK_EQUAL(c50.capacity(), 50);
	CHECK_EQUAL(c40938.capacity(), 40938);

	CHECK(!c0.is_truncated());
	CHECK(!c1.is_truncated());
	CHECK(!c5.is_truncated());
	CHECK(!c50.is_truncated());
	CHECK(!c40938.is_truncated());
}

TEST(capped_string_constructor_from_char_const_ptr)
{
	CappedString0 const c0("");
	CappedString1 const c1("");
	CappedString5 const c5("");
	CappedString50 const c50("");
	CappedString303 const c303("");
	CappedString40938 const c40938("");
	CHECK(!c0.is_truncated());
	CHECK_EQUAL(c0.size(), 0);
	CHECK(!c1.is_truncated());
	CHECK_EQUAL(c1.size(), 0);
	CHECK_EQUAL(c303.size(), 0);
	CHECK(c303.empty());
	CHECK(!c303.is_truncated());
	CHECK_EQUAL(std::string(c40938.c_str()), std::string(c0.c_str()));
	CHECK(c40938.empty());

	char const* const cstr0 = "Hey";
	CappedString0 d0(cstr0);
	CappedString1 d1(cstr0);
	CappedString5 d5(cstr0);
	CappedString50 const d50(cstr0);
	CappedString303 const d303(cstr0);
	CappedString40938 const d40938(cstr0);
	CHECK_EQUAL(d0.size(), 0);
	CHECK(d0.is_truncated());
	CHECK_EQUAL(d1.size(), 1);
	CHECK(d1.is_truncated());
	CHECK_EQUAL(d5.size(), 3);
	CHECK(!d5.is_truncated());
	CHECK_EQUAL(d50.size(), 3);
	CHECK_EQUAL(d303.size(), 3);
	CHECK_EQUAL(d40938.size(), 3);
	CHECK(!d40938.is_truncated());
	CHECK_EQUAL(d0, CappedString0(""));
	CHECK_EQUAL(d1, CappedString1("H"));
	CHECK_EQUAL(d5, CappedString5("Hey"));

	std::string str1(900, 'x');
	char const* const cstr1 = str1.c_str();
	CappedString0 const e0(cstr1);
	CappedString1 const e1(cstr1);
	CappedString5 const e5(cstr1);
	CappedString50 const e50(cstr1);
	CappedString303 const e303(cstr1);
	CappedString40938 const e40938(cstr1);
	CHECK_EQUAL(strlen(e0.c_str()), 0);
	CHECK(e0.empty());
	CHECK(e0.is_truncated());
	CHECK_EQUAL(e1.size(), 1);
	CHECK_EQUAL(e1.c_str()[0], 'x');
	CHECK_EQUAL(e1, CappedString1("x"));
	CHECK_EQUAL(e5, CappedString5(std::string(5, 'x')));
	CHECK(e5.is_truncated());
	CHECK_EQUAL(e50.size(), 50);
	CHECK_EQUAL(e303.size(), 303);
	CHECK(e303.is_truncated());
	CHECK(!e303.empty());
	CHECK(!e40938.is_truncated());
	CHECK_EQUAL(std::string(e40938.c_str()), str1);

	std::string str2(303, 'y');
	char const* const cstr2 = str2.c_str();
	CappedString0 const f0(cstr2);
	CHECK(f0.is_truncated());
	CappedString1 const f1(cstr2);
	CHECK_EQUAL(f1.size(), 1);
	CappedString5 const f5(cstr2);
	CHECK_EQUAL(f5.size(), 5);
	CHECK(f5.is_truncated());
	CHECK_EQUAL(std::string(f5.c_str()), std::string(5, 'y'));
	CappedString50 const f50(cstr2);
	CHECK_EQUAL(f50.size(), 50);
	CHECK(f50.is_truncated());
	CappedString303 const f303(cstr2);
	CHECK_EQUAL(f303.size(), 303);
	CHECK_EQUAL(f303.capacity(), f303.size());
	CappedString40938 const f40938(cstr2);
	CHECK_EQUAL(f40938.size(), 303);
}
