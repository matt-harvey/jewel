#include "exception.hpp"
#include <cassert>
#include <cstring>
#include <string>
#include <unittest++/UnitTest++.h>

using jewel::Exception;
using std::string;
using std::strlen;

TEST(error_message_truncation_point)
{
	string const trunc_msg("[TRUNCATED]");
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
				string const last_bit(it, msg_str.end());
				CHECK_EQUAL(last_bit, trunc_msg);
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

					
	
