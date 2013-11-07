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

#include "assert.hpp"
#include "log.hpp"
#include "exception_special_tests.hpp"
#include <cstring>
#include <iostream>
#include <string>

using std::cout;
using std::endl;
using std::strcmp;
using std::string;


namespace jewel
{

namespace detail
{

void test_exception_macros()
{
	try
	{
		JEWEL_THROW(TrialException0, "Here's a TrialException0.");
	}
	catch (std::exception&)
	{
	}

	try
	{
		throw TrialException1("Here's a TrialException1.");
	}
	catch (std::exception&)
	{
	}


	try
	{
		JEWEL_THROW(TrialException0_0, "Here's TrialException0_0.");
	}
	catch (std::exception&)
	{
	}

	try
	{
		JEWEL_THROW(TrialException0_0, "Here a TrialException0_0.");
	}
	catch (TrialException0& e)
	{
		JEWEL_HARD_ASSERT
		(	strcmp(e.message(), "Here a TrialException0_0.") == 0
		);
		JEWEL_HARD_ASSERT(strcmp(e.type(), "TrialException0_0") == 0);
		string const filepath(e.filepath());
		string const expected_filename = "exception_special_tests.cpp";
		JEWEL_HARD_ASSERT(expected_filename.size() - filepath.size());
		string::const_iterator const it =
			filepath.begin() + filepath.size() - expected_filename.size();
		JEWEL_HARD_ASSERT(string(it, filepath.end()) == expected_filename);
	}

	try
	{
		throw TrialException1_0("Here's TrialException1_0.");
	}
	catch (std::exception&)
	{
	}

	try
	{
		throw TrialException1_0();
	}
	catch (TrialException1&)
	{
	}

	try
	{
		JEWEL_THROW(TrialException1_0, "Here's a TrialException1_0.");
	}
	catch (TrialException0&)
	{
		// This should not be reached as TrialException0 should not
		// catch TrialException1_0.
		JEWEL_HARD_ASSERT (false);
	}
	catch (TrialException1&)
	{
		// But this should catch it.
	}

	try
	{
		throw TrialException1_0_1();
	}
	catch (TrialException1_0_1&)
	{
	}

	try
	{
		throw TrialException1_0_1("Here's a TrialException1_0_1.");
	}
	catch (TrialException1_0&)
	{
	}

	try
	{
		throw TrialException1_0_1("Here's a TrialException1_0_1.");
	}
	catch (TrialException1&)
	{
	}

	try
	{
		JEWEL_THROW(TrialException1_0_1, "Here's a TrialException1_0_1.");
	}
	catch (TrialException1_0_1& e)
	{
		JEWEL_ASSERT (string(e.what()) == string("Here's a TrialException1_0_1."));
	}

	try
	{
		throw std::exception();
	}
	catch (std::exception&)
	{
	}

	try
	{
		throw TrialException1_0_1("Here's a TrialException1_0_1.");
	}
	catch (std::exception&)
	{
	}

	cout << "Success! Exceptions classes created using these macros "
	     << "behaved as expected." << endl;

	return;
}




}  // namespace detail

}  // namespace jewel
