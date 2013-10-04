// Copyright (c) 2013, Matthew Harvey. All rights reserved.

#include "../assert.hpp"
#include "../tolerant_string.hpp"
#include <UnitTest++/UnitTest++.h>
#include <cstring>
#include <exception>
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
using std::terminate;

namespace
{
	void handle_invalidity()
	{
		std::cerr << "Unexpected memory allocation failure during "
		          << "unit tests for jewel::TolerantString."
				  << endl;
		std::terminate();
	}
		
}  // end anonymous namespace

TEST(tolerant_string_default_constructor)
{
	TolerantString ts0;
	CHECK_EQUAL(ts0.c_str(), std::string().c_str());
	CHECK(ts0.empty());
	CHECK(ts0.is_valid());
	CHECK_EQUAL(ts0.size(), static_cast<size_t>(0));
	CHECK_EQUAL(strlen(ts0.c_str()), static_cast<size_t>(0));
	CHECK_EQUAL(strcmp(ts0.c_str(), ""), 0);
	ts0.clear();
	CHECK_EQUAL(ts0.size(), static_cast<size_t>(0));
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
		CHECK_EQUAL(ts0.size(), static_cast<size_t>(12));
		CHECK(!ts0.empty());
		ts0.clear();
		CHECK(ts0.empty());
		CHECK_EQUAL(std::string(ts0.c_str()), std::string());
	}
	else
	{
		handle_invalidity();
	}
	TolerantString ts1("");
	if (ts1.is_valid())
	{
		TolerantString ts1b;
		CHECK_EQUAL(ts1, ts1b);
		CHECK_EQUAL(ts1.size(), static_cast<size_t>(0));
		CHECK_EQUAL(strlen(ts1.c_str()), static_cast<size_t>(0));
		CHECK_EQUAL(strcmp(ts1.c_str(), ""), 0);
	}
	else
	{
		handle_invalidity();
	}
	TolerantString const ts2 = "Hello there!";
	if (ts2.is_valid())
	{
		CHECK_EQUAL(std::string(ts2.c_str()), std::string("Hello there!"));
		CHECK_EQUAL(ts2, TolerantString("Hello there!"));
		CHECK_EQUAL(strcmp(ts2.c_str(), "Hello there!"), 0);
		CHECK_EQUAL(ts2.size(), static_cast<size_t>(12));
		CHECK(!ts2.empty());
	}
	else
	{
		handle_invalidity();
	}
}

TEST(tolerant_string_copy_constructor)
{
	TolerantString const ts0;
	TolerantString ts1(std::string(589760, 'b').c_str());
	TolerantString const ts2("\n   \t lkjj");
	TolerantString ts3("Hello!!!!");
	TolerantString const ts4 = "100293k";
	TolerantString ts0b(ts0);
	TolerantString const ts1b = ts1;
	TolerantString ts2b(ts2);
	TolerantString const ts3b = ts3;
	TolerantString ts4b(ts4);
	if
	(	ts0.is_valid() &&
		ts1.is_valid() &&
		ts2.is_valid() &&
		ts3.is_valid() &&
		ts4.is_valid() &&
		ts0b.is_valid() &&
		ts1b.is_valid() &&
		ts2b.is_valid() &&
		ts3b.is_valid() &&
		ts4b.is_valid()
	)
	{
		CHECK_EQUAL(ts0b, ts0);
		CHECK_EQUAL(ts1b, ts1);
		CHECK_EQUAL(ts2b, ts2);
		CHECK_EQUAL(ts3b, ts3);
		CHECK_EQUAL(ts4b, ts4);
		CHECK(ts0b.empty());
		CHECK_EQUAL(ts1b.size(), static_cast<size_t>(589760));
		CHECK_EQUAL(std::string(ts4b.c_str()), "100293k");
	}
	else
	{
		handle_invalidity();
	}
}

TEST(tolerant_string_copy_assignment)
{
	TolerantString ts0;
	TolerantString ts1(std::string(589760, 'b').c_str());
	TolerantString ts2("\n   \t lkjj");
	TolerantString const ts3("Hello!!!!");
	TolerantString ts4 = "100293k";

	ts0 = ts4;
	ts1 = ts2;
	ts2 = ts3;
	ts4 = ts4;

	if
	(	ts0.is_valid() &&
		ts1.is_valid() &&
		ts2.is_valid() &&
		ts4.is_valid()
	)
	{
		CHECK_EQUAL(ts0, ts4);
		CHECK_EQUAL(ts2, ts3);
		CHECK_EQUAL(ts2, ts3);
		CHECK_EQUAL(ts4, ts4);
		CHECK_EQUAL(ts0.size(), ts4.size());
	}
	else
	{
		handle_invalidity();
	}

	ts4 = TolerantString();
	if (ts4.is_valid())
	{
		CHECK_EQUAL(ts4, TolerantString());
		CHECK_EQUAL(ts4.size(), static_cast<size_t>(0));
	}
	else
	{
		handle_invalidity();
	}
}

TEST(tolerant_string_equality_and_inequality)
{
	TolerantString ts0;
	TolerantString ts0b = ts0;
	TolerantString ts1(std::string(589760, 'b').c_str());
	TolerantString ts1b(std::string(589760, 'B').c_str());
	TolerantString ts2("\n   \t lkjj");
	TolerantString ts2b("\n   \t lkjj");
	TolerantString const ts3("Hello!!!!");
	TolerantString ts3b("Hello!!!!");
	TolerantString ts4 = "100293k";
	TolerantString const ts4b("100293k");

	if
	(	ts0.is_valid() &&
		ts0b.is_valid() &&
		ts1.is_valid() &&
		ts1b.is_valid() &&
		ts2.is_valid() &&
		ts2b.is_valid() &&
		ts4.is_valid() &&
		ts4b.is_valid()
	)
	{
		CHECK(ts0b == ts0);
		CHECK(ts0b != ts1);
		CHECK(ts1b != ts1);
		CHECK(ts2 == ts2b);
		CHECK(ts3b == ts3);
		CHECK(ts3b != ts2);
		CHECK(ts4b == ts4);
	}
	else
	{
		handle_invalidity();
	}
}

TEST(tolerant_string_c_str)
{
	TolerantString ts0;
	TolerantString ts0b = ts0;
	TolerantString ts1(std::string(589760, 'b').c_str());
	TolerantString ts1b(std::string(12, 'B').c_str());
	TolerantString ts2("\n   \t lkjj");
	TolerantString const ts3("Hello!!!!");
	TolerantString const ts4 = "100293k";

	if
	(	ts0.is_valid() &&
		ts0b.is_valid() &&
		ts1.is_valid() &&
		ts1b.is_valid() &&
		ts2.is_valid() &&
		ts3.is_valid() &&
		ts4.is_valid()
	)
	{
		CHECK_EQUAL(strcmp(ts0.c_str(), ""), 0);
		CHECK_EQUAL(strcmp(ts0b.c_str(), std::string().c_str()), 0);
		CHECK_EQUAL(std::string(ts1.c_str()), std::string(589760, 'b'));
		CHECK_EQUAL(strcmp(ts1b.c_str(), std::string(12, 'B').c_str()), 0);
		CHECK_EQUAL(strcmp(ts2.c_str(), "\n   \t lkjj"), 0);
		CHECK_EQUAL(strcmp(ts3.c_str(), "Hello!!!!"), 0);
		CHECK(strcmp(ts3.c_str(), "Hello!!!") != 0);
		CHECK_EQUAL(strcmp(ts4.c_str(), "100293k"), 0);
	}
	else
	{
		handle_invalidity();
	}
}

TEST(tolerant_string_size)
{
	TolerantString const ts0;
	TolerantString const ts1("Hello");
	TolerantString ts2(std::string(9768, 'b').c_str());
	TolerantString const ts3("I dunno what to write here.");
	TolerantString ts4 = ts1;
	TolerantString ts5 = TolerantString("apkh");
	TolerantString ts6("AAAAAccc");
	ts5 = ts6;
	if
	(	ts0.is_valid() &&
		ts1.is_valid() &&
		ts2.is_valid() &&
		ts3.is_valid() &&
		ts4.is_valid() &&
		ts5.is_valid() &&
		ts6.is_valid()
	)
	{
		CHECK_EQUAL(ts0.size(), static_cast<size_t>(0));
		CHECK_EQUAL(ts1.size(), static_cast<size_t>(5));
		CHECK_EQUAL(ts2.size(), static_cast<size_t>(9768));
		CHECK_EQUAL(ts3.size(), strlen("I dunno what to write here."));
		CHECK_EQUAL(ts4.size(), ts1.size());
		CHECK_EQUAL(ts5.size(), static_cast<size_t>(8));
		CHECK_EQUAL(ts6.size(), ts5.size());
	}
	else
	{
		handle_invalidity();
	}
}

TEST(tolerant_string_empty)
{
	TolerantString const ts0;
	TolerantString const ts1("Hello");
	TolerantString ts2(std::string(9768, 'b').c_str());
	TolerantString const ts3("I dunno what to write here.");
	TolerantString ts4(std::string(7889, '#').c_str());
	ts4 = "";
	if
	(	ts0.is_valid() &&
		ts1.is_valid() &&
		ts2.is_valid() &&
		ts3.is_valid() &&
		ts4.is_valid()
	)
	{
		CHECK(ts0.empty());
		CHECK(!ts1.empty());
		CHECK(!ts2.empty());
		CHECK(!ts3.empty());
		CHECK(ts4.empty());
	}
	else
	{
		handle_invalidity();
	}
}

TEST(tolerant_string_clear)
{
	TolerantString ts0;
	TolerantString ts1("Hello");
	TolerantString ts2(std::string(9768, 'b').c_str());
	TolerantString ts3("I dunno what to write here.");
	TolerantString ts4(std::string(7889, '#').c_str());
	if
	(	ts0.is_valid() &&
		ts1.is_valid() &&
		ts2.is_valid() &&
		ts3.is_valid() &&
		ts4.is_valid()
	)
	{
		CHECK(ts0.empty());
		CHECK(!ts1.empty());
		CHECK(!ts2.empty());
		CHECK(!ts3.empty());
		CHECK(!ts4.empty());
		ts0.clear();
		ts1.clear();
		ts2.clear();
		ts3.clear();
		ts4.clear();
		CHECK(ts0.is_valid());
		CHECK(ts0.empty());
		CHECK(ts1.empty());
		CHECK(ts1.is_valid());
		CHECK(ts1.size() == static_cast<size_t>(0));
		CHECK(ts2.empty());
		CHECK(ts2.is_valid());
		CHECK_EQUAL(ts2, TolerantString());
		CHECK(ts3.empty());
		CHECK(ts3.is_valid());
		CHECK(ts4.empty());
		CHECK(ts4.is_valid());
		CHECK_EQUAL(ts4.c_str(), "");
	}
	else
	{
		handle_invalidity();
	}
}

TEST(tolerant_string_swap)
{
	TolerantString ts0;
	TolerantString ts1("Hello");
	TolerantString ts2(std::string(9768, 'b').c_str());
	TolerantString ts3("I dunno what to write here.");
	TolerantString ts4(std::string(7889, '#').c_str());
	if
	(	ts0.is_valid() &&
		ts1.is_valid() &&
		ts2.is_valid() &&
		ts3.is_valid() &&
		ts4.is_valid()
	)
	{
		ts0.swap(ts1);
		ts2.swap(ts3);
		CHECK_EQUAL(ts0, "Hello");
		CHECK_EQUAL(ts1, TolerantString());
		CHECK_EQUAL(ts2, TolerantString("I dunno what to write here."));
		CHECK_EQUAL(ts3.size(), static_cast<size_t>(9768));
		CHECK_EQUAL(ts3, TolerantString(std::string(9768, 'b').c_str()));
		ts3.swap(ts4);
		CHECK_EQUAL(ts3, TolerantString(std::string(7889, '#').c_str()));
		CHECK_EQUAL(ts4.size(), static_cast<size_t>(9768));
		CHECK(ts0.is_valid());
		CHECK(ts1.is_valid());
		CHECK(ts2.is_valid());
		CHECK(ts3.is_valid());
		CHECK(ts4.is_valid());
	}
	else
	{
		handle_invalidity();
	}

}

TEST(tolerant_string_output_operator)
{
	TolerantString ts0;
	TolerantString ts1("Hello");
	TolerantString ts2(std::string(9768, 'b').c_str());
	TolerantString ts3("I dunno what to write here.");
	TolerantString ts4(std::string(7889, '#').c_str());
	if
	(	ts0.is_valid() &&
		ts1.is_valid() &&
		ts2.is_valid() &&
		ts3.is_valid() &&
		ts4.is_valid()
	)
	{
		ostringstream oss0;
		oss0 << ts0;
		CHECK_EQUAL(oss0.str(), std::string());
		oss0 << ts1;
		CHECK_EQUAL(oss0.str(), "Hello");
		oss0 << ts2;
		CHECK_EQUAL
		(	oss0.str(),
			std::string("Hello") + std::string(9768, 'b')
		);
		
		ostringstream oss1;
		oss1 << ts3 << ts3 << "\n" << ts1 << ts0 << endl;
		CHECK_EQUAL
		(	oss1.str(),
			std::string("I dunno what to write here.") +
				"I dunno what to write here.\n" +
				"Hello\n"
		);
	}
	else
	{
		handle_invalidity();
	}
}
