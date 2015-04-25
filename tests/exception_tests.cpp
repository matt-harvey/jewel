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

                    
    
