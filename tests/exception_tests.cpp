// Copyright (c) 2013, Matthew Harvey. All rights reserved.

#include "../exception.hpp"
#include "../assert.hpp"
#include "../log.hpp"
#include <cstring>
#include <string>
#include <UnitTest++/UnitTest++.h>

using jewel::Exception;
using jewel::Log;
using std::string;
using std::strlen;

TEST(error_message_truncation_point)
{
	string const trunc_msg("(TRUNCATED)");
	size_t const max_msg_size = Exception::max_message_size();
	for (string::size_type i = 0; i != max_msg_size * 2; ++i)
	{
		string const errmsg(i, 'x');
		try
		{
			throw Exception(errmsg.c_str());
		}
		catch (Exception& e)
		{
			size_t const len = std::strlen(e.what());
			if (len <= max_msg_size)
			{
				for (size_t m = 0; m != len; ++m)
				{
					CHECK_EQUAL( (e.what())[m], 'x');
					if (e.what()[m] != 'x')
					{
						JEWEL_LOG_VALUE(Log::info, len);
					}
				}
			}
			else
			{
				string const msg_str(e.what());
				string::const_iterator it = msg_str.begin();
				for (string::size_type j = 0; j != max_msg_size; ++j)
				{
					CHECK_EQUAL(*it, 'x');
					++it;
				}
				string const last_part(it, msg_str.end());
				CHECK_EQUAL(last_part, trunc_msg);
			}
		}
	}
	try
	{
		throw Exception("Something's cooking.");
	}
	catch (Exception& e)
	{
		CHECK_EQUAL(e.what(), "Something's cooking.");
	}
}

					
	
