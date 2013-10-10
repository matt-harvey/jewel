/*
 * This file is part of the Jewel project and is distributed under the
 * terms of the License contained in the file LICENSE.txt distributed
 * with this package.
 * 
 * Author: Matthew Harvey <matthew@matthewharvey.net>
 *
 * Copyright (c) 2012-2013, Matthew Harvey.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */


#include "assert.hpp"
#include "exception.hpp"
#include "log.hpp"
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

					
	
