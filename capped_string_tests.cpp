// Copyright (c) 2013, Matthew Harvey. All rights reserved.

#include "assert.hpp"
#include "capped_string.hpp"
#include "log.hpp"
#include <UnitTest++/UnitTest++.h>
#include <cstring>
#include <iostream>
#include <string>

using jewel::CappedString;
using jewel::Log;
using std::cout;
using std::endl;
using std::strcmp;
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

TEST(capped_string_constructor_from_std_string)
{
	std::string const s;
	CappedString0 const c0(s);
	CappedString1 const c1(s);
	CappedString5 const c5(s);
	CappedString50 const c50(s);
	CappedString303 const c303(s);
	CappedString40938 const c40938(s);
	CHECK(!c0.is_truncated());
	CHECK_EQUAL(c0.size(), 0);
	CHECK(!c1.is_truncated());
	CHECK_EQUAL(c1.size(), 0);
	CHECK_EQUAL(c303.size(), 0);
	CHECK(c303.empty());
	CHECK(!c303.is_truncated());
	CHECK_EQUAL(std::string(c40938.c_str()), std::string(c0.c_str()));
	CHECK(c40938.empty());

	std::string const str0("Hey");
	CappedString0 d0(str0);
	CappedString1 d1(str0);
	CappedString5 d5(str0);
	CappedString50 const d50(str0);
	CappedString303 const d303(str0);
	CappedString40938 const d40938(str0);
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
	CHECK_EQUAL(d0, CappedString0());
	CHECK_EQUAL(d1, CappedString1(std::string("H")));
	CHECK_EQUAL(d5, CappedString5(std::string("Hey")));

	std::string str1(900, 'x');
	CappedString0 const e0(str1);
	CappedString1 const e1(str1);
	CappedString5 const e5(str1);
	CappedString50 const e50(str1);
	CappedString303 const e303(str1);
	CappedString40938 const e40938(str1);
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

	std::string const str2(303, 'y');
	CappedString0 const f0(str2);
	CHECK(f0.is_truncated());
	CappedString1 const f1(str2);
	CHECK_EQUAL(f1.size(), 1);
	CappedString5 const f5(str2);
	CHECK_EQUAL(f5.size(), 5);
	CHECK(f5.is_truncated());
	CHECK_EQUAL(std::string(f5.c_str()), std::string(5, 'y'));
	CappedString50 const f50(str2);
	CHECK_EQUAL(f50.size(), 50);
	CHECK(f50.is_truncated());
	CappedString303 const f303(str2);
	CHECK_EQUAL(f303.size(), 303);
	CHECK_EQUAL(f303.capacity(), f303.size());
	CappedString40938 const f40938(str2);
	CHECK_EQUAL(f40938.size(), 303);

	std::string const str3("lailasdf8907\n\t\a --    '");
	CHECK_EQUAL(CappedString50(str3), CappedString50(str3.c_str()));
	CHECK_EQUAL(CappedString40938(str3.c_str()), CappedString40938(str3));
	CappedString40938 g40938(str3);
	CHECK_EQUAL(g40938, CappedString40938(str3.c_str()));
}

TEST(capped_string_copy_constructor)
{
	std::string const s0("heoa");
	std::string const s1("890jk'l2j\n\n\n\"");
	std::string const s2("yes");
	std::string const s3(688, ' ');
	std::string const s4("lkuioasdfhjkasdf'asdfasdfhjk'asdasdfhasdf'");

	CappedString0 const c0(s0);
	CappedString1 const c1(s1.c_str());
	CappedString5 const c5(s2);
	CappedString50 const c50(s3.c_str());
	CappedString303 const c303(s4);
	CappedString40938 const c40938(s4.c_str());

	CappedString0 d0 = c0;
	CappedString0 e0(c0);
	CappedString1 d1 = c1;
	CappedString1 const e1(c1);
	CappedString50 const d50 = c50;
	CappedString50 e50(c50);
	CappedString303 const d303(c303);
	CappedString303 e303 = c303;
	CappedString40938 d40938(c40938);
	CappedString40938 const e40938 = c40938;

	CHECK_EQUAL(d0, c0);
	CHECK_EQUAL(e0, c0);
	CHECK_EQUAL(d1, c1);
	CHECK_EQUAL(d50, c50);
	CHECK_EQUAL(e50, c50);
	CHECK_EQUAL(d303, c303);
	CHECK_EQUAL(e303, c303);
	CHECK_EQUAL(d40938, c40938);
	CHECK_EQUAL(e40938, c40938);

	CHECK_EQUAL(d0.size(), c0.size());
	CHECK_EQUAL(std::string(e0.c_str()), std::string(c0.c_str()));
	CHECK_EQUAL(d1.size(), c1.size());
	CHECK_EQUAL(e303.size(), c303.size());

}

TEST(capped_string_assignment)
{
	char const* const nc1("890jk'12j\n\n\n\"");
	CappedString0 const c0("heoa");
	CappedString1 c1(nc1);
	CappedString50 const c2("yes");
	CappedString303 c3(std::string(688, ' '));
	CappedString40938 const c4("lkuioasdfhjkasdf'asdfasdfhjk'asdasdfhasdf'");
	CappedString<1000000> c5(std::string(2000000, 'x'));
	CappedString40938 c6;
	CappedString303 c7;
	CappedString0 c8;
	CappedString1 c9("                 ");

	c1 = c1;
	CHECK_EQUAL(c1, CappedString1(nc1));
	c3 = c7;
	CHECK_EQUAL(c3 = c3, c3);
	CHECK_EQUAL(c0, CappedString0());
	CHECK_EQUAL(c3.size(), 0);
	CHECK_EQUAL(c3.capacity(), 303);
	c6 = CappedString40938(c3.c_str());
	CHECK_EQUAL(c6, CappedString40938());
	CHECK_EQUAL
	(	(c6 = CappedString40938(c2.c_str())),
		CappedString40938("yes")
	);
	c7 = CappedString<303>(c2.c_str());
	CHECK_EQUAL(c7, CappedString303(c6.c_str()));
	c9 = c1;
	CHECK_EQUAL(c9, CappedString1(std::string(nc1)));
	CHECK(c9.is_truncated());
	char const* const nc9 = c9.c_str();
	CappedString303 cc9(nc9);
	c7 = CappedString303(cc9);
	CHECK_EQUAL(c7, CappedString303("8"));
	CHECK(!c7.is_truncated());
	c5 = CappedString<1000000>(nc1);
	CHECK(!c5.is_truncated());
	CHECK_EQUAL(CappedString<1000000>(nc1), c5);

}

TEST(capped_string_equality_and_inequality)
{
	std::string const s0("heoa");
	std::string const s1("890jk'l2j\n\n\n\"");
	std::string const s2("yes");
	std::string const s3(688, ' ');
	std::string const s4("lkuioasdfhjkasdf'asdfasdfhjk'asdasdfhasdf'");

	CappedString<9> c0a(s0);
	CappedString<9> c0b(s0);
	CappedString<9> c1a(s1);
	CappedString<9> c1b(s1.c_str());
	CappedString<9> c2a(s2);
	CappedString<9> c2b(s2);
	CappedString<7000> const c3a(s3);
	CappedString<7000> const c3b(s3);
	CappedString<12> const c4a(s4);
	CappedString<12> const c4b(s4);
	CappedString<12> const c5;

	CHECK(c0a == c0b);
	CHECK(c0b == c0a);
	CHECK(c1a != c0a);
	CHECK(c1a == c1b);
	CHECK(c1b != c0b);
	CHECK(c2b == c2a);
	CHECK(c2b != c0a);
	CHECK(c3a == c3b);
	CHECK(c4b != c5);
	CHECK(c4a == c4b);
}

TEST(capped_string_c_str)
{
	char const* const s0("heoa");
	char const* const s1("890jk'l2j\n\n\n\"");
	std::string const s2("yes");
	std::string const s3(688, ' ');
 	char const* const s4("lkuioasdfhjkasdf'asdfasdfhjk'asdasdfhasdf'");

	CappedString<9> c0(s0);
	CappedString<9> c1(s1);
	CappedString<9> c2(s2);
	CappedString<7000> const c3(s3);
	CappedString<12> const c4(s4);
	CappedString<12> const c5;

	CHECK_EQUAL(strcmp(c0.c_str(), s0), 0);
	CHECK_EQUAL(strcmp(c1.c_str(), std::string(s1, s1 + 9).c_str()), 0);
	CHECK_EQUAL(strcmp(c2.c_str(), s2.c_str()), 0);
	CHECK_EQUAL(strcmp(c3.c_str(), s3.c_str()), 0);
	CHECK_EQUAL(strcmp(c4.c_str(), std::string(s4, s4 + 12).c_str()), 0);
	CHECK_EQUAL(strcmp(c5.c_str(), ""), 0);
	
}

TEST(capped_string_capacity)
{
	char const* const s0("heoa");
	char const* const s1("890jk'l2j\n\n\n\"");
	std::string const s2("yes");
	std::string const s3(688, ' ');
 	char const* const s4("lkuioasdfhjkasdf'asdfasdfhjk'asdasdfhasdf'");

	CappedString<9> c0(s0);
	CappedString<9> const c1(s1);
	CappedString<9> c2(s2);
	CappedString<7000> const c3(s3);
	CappedString<12> const c4(s4);
	CappedString<12> c5;

	CHECK_EQUAL(c0.capacity(), 9);
	CHECK_EQUAL(c1.capacity(), 9);
	CHECK_EQUAL(c2.capacity(), 9);
	CHECK_EQUAL(c3.capacity(), 7000);
	CHECK_EQUAL(c4.capacity(), 12);
	CHECK_EQUAL(c5.capacity(), 12);

	c5 = CappedString<12>
	(	std::string("lasdfjkflkajsl;dfkjasl;dfjas") +
		std::string(60238, '\n')
	);
	CHECK_EQUAL(c5.capacity(), 12);
	c5 = CappedString<12>();
	CHECK_EQUAL(c5.capacity(), 12);
}

TEST(capped_string_size)
{
	char const* const s0("heoa");
	char const* const s1("890jk'l2j\n\n\n\"");
	std::string const s2("yes");
	std::string const s3(688, ' ');
 	char const* const s4("lkuioasdfhjkasdf'asdfasdfhjk'asdasdfhasdf'");

	CappedString<9> c0(s0);
	CappedString<9> const c1(s1);
	CappedString<9> const c2(s2);
	CappedString<7000> c3(s3);
	CappedString<12> const c4(s4);
	CappedString<12> c5;

	CHECK_EQUAL(c0.size(), 4);
	CHECK_EQUAL(c1.size(), 9);
	CHECK_EQUAL(c2.size(), 3);
	CHECK_EQUAL(c3.size(), 688);
	CHECK_EQUAL(c4.size(), 12);
	CHECK_EQUAL(c5.size(), 0);
	c5 = CappedString<12>(c0.c_str());
	CHECK_EQUAL(c5.size(), 4);
	c3 = CappedString<7000>(std::string(7001, '\0'));
	CHECK_EQUAL(c3.size(), 0);
	c3 = CappedString<7000>(std::string(7001, '9'));
	CHECK_EQUAL(c3.size(), 7000);
	c3 = CappedString<7000>("Lkjlg\n");
	CHECK_EQUAL(c3.size(), 6);
}

TEST(capped_string_empty)
{
	char const* const s0("heoa");
	char const* const s1("890jk'l2j\n\n\n\"");
	std::string const s2("yes");
	std::string const s3(688, ' ');
 	char const* const s4("lkuioasdfhjkasdf'asdfasdfhjk'asdasdfhasdf'");

	CappedString<9> c0(s0);
	CappedString<9> const c1(s1);
	CappedString<9> const c2(s2);
	CappedString<7000> c3(s3);
	CappedString<12> c4(s4);
	CappedString<12> c5;
	CappedString<0> c6(s4);

	CHECK(!c0.empty());
	CHECK(!c1.empty());
	CHECK(!c2.empty());
	CHECK(!c3.empty());
	CHECK(!c4.empty());
	CHECK(c5.empty());
	CHECK(c6.empty());
	c4 = c5;
	CHECK(c4.empty());
	CHECK(CappedString<682>("").empty());
	CHECK(CappedString<0>(std::string()).empty());
}

TEST(capped_string_truncated)
{
	// Log::set_filepath("~/Workbench/versioned/jewel/scratch.log");
	// Log::set_threshold(Log::trace);

	CappedString<3> c("xxx");
	CHECK(!c.is_truncated());
	c = "xxxx";
	CHECK(c.is_truncated());
	c = "xxx";
	CHECK(!c.is_truncated());
	c = CappedString<3>();
	CHECK(!c.is_truncated());
	CappedString<0> d;
	CHECK(!d.is_truncated());
	d = "";
	CHECK(!d.is_truncated());
	d = "x";
	CHECK(d.is_truncated());
	d = "\0alsdkasdfljasdlfk";
	CHECK(!d.is_truncated());
	CappedString<456> e("lasdfk");
	CHECK(!e.is_truncated());
	std::string const s(456, 'x');
	e = s.c_str();
	CHECK(!e.is_truncated());
	std::string const t = s + "x";
	e = t.c_str();
	CHECK(e.is_truncated());
}

TEST(capped_string_clear)
{
	typedef CappedString<101115> CS;
	CS a("asdfy");
	CHECK(!a.empty());
	CHECK_EQUAL(a.size(), 5);
	a.clear();
	CHECK_EQUAL(a.size(), 0);
	CHECK_EQUAL(a, CS());
	CHECK(a.empty());
	a = CS(std::string(8907777, 'h'));
	CHECK(a.size() > 0);
	a.clear();
	CHECK(a.size() == 0);

	CappedString<0> b;
	b.clear();
	CHECK(b.empty());
}
