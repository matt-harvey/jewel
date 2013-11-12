/*
 * Copyright 2012, 2013 Matthew Harvey
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

int test_exception_macros()
{
	cout << "Now running special tests of exception related macros..." << endl;

	int num_failures = 0;

	try
	{
		JEWEL_THROW(TrialException0, "Here's a TrialException0.");
	}
	catch (std::exception&)
	{
	}
	catch (...)
	{
		JEWEL_LOG_MESSAGE(Log::error, "Failed test");
		++num_failures;
	}

	try
	{
		throw TrialException1("Here's a TrialException1.");
	}
	catch (std::exception&)
	{
	}
	catch (...)
	{
		JEWEL_LOG_MESSAGE(Log::error, "Failed test");
		++num_failures;
	}

	try
	{
		JEWEL_THROW(TrialException0_0, "Here's TrialException0_0.");
	}
	catch (std::exception&)
	{
	}
	catch (...)
	{
		JEWEL_LOG_MESSAGE(Log::error, "Failed test");
		++num_failures;
	}

	try
	{
		JEWEL_THROW(TrialException0_0, "Here a TrialException0_0.");
	}
	catch (TrialException0& e)
	{
		if (strcmp(e.message(), "Here a TrialException0_0.") != 0)
		{
			JEWEL_LOG_MESSAGE(Log::error, "Failed test");
			++num_failures;
		}
		if (strcmp(e.type(), "TrialException0_0") != 0)
		{
			JEWEL_LOG_MESSAGE(Log::error, "Failed test");
			++num_failures;
		}
		string const filepath(e.filepath());
		string const expected_filename = "exception_special_tests.cpp";
		JEWEL_HARD_ASSERT (filepath.size() > expected_filename.size());
		string::const_iterator const it =
			filepath.begin() + filepath.size() - expected_filename.size();
		if (string(it, filepath.end()) != expected_filename)
		{
			JEWEL_LOG_MESSAGE(Log::error, "Failed test");
			++num_failures;
		}
	}
	catch (...)
	{
		JEWEL_LOG_MESSAGE(Log::error, "Failed test");
		++num_failures;
	}

	try
	{
		throw TrialException1_0("Here's TrialException1_0.");
		++num_failures;
	}
	catch (std::exception&)
	{
	}
	catch (...)
	{
		JEWEL_LOG_MESSAGE(Log::error, "Failed test");
		++num_failures;
	}

	try
	{
		throw TrialException1_0();
	}
	catch (TrialException1&)
	{
	}
	catch (...)
	{
		JEWEL_LOG_MESSAGE(Log::error, "Failed test");
		++num_failures;
	}

	try
	{
		JEWEL_THROW(TrialException1_0, "Here's a TrialException1_0.");
	}
	catch (TrialException0&)
	{
		// This should not be reached as TrialException0 should not
		// catch TrialException1_0.
		JEWEL_LOG_MESSAGE(Log::error, "Failed test");
		++num_failures;
	}
	catch (TrialException1&)
	{
		// But this should catch it.
	}
	catch (...)
	{
		JEWEL_LOG_MESSAGE(Log::error, "Failed test");
		++num_failures;
	}

	try
	{
		throw TrialException1_0_1();
	}
	catch (TrialException1_0_1&)
	{
	}
	catch (...)
	{
		JEWEL_LOG_MESSAGE(Log::error, "Failed test");
		++num_failures;
	}

	try
	{
		throw TrialException1_0_1("Here's a TrialException1_0_1.");
	}
	catch (TrialException1_0&)
	{
	}
	catch (...)
	{
		JEWEL_LOG_MESSAGE(Log::error, "Failed test");
		++num_failures;
	}

	try
	{
		throw TrialException1_0_1("Here's a TrialException1_0_1.");
	}
	catch (TrialException1&)
	{
	}
	catch (...)
	{
		JEWEL_LOG_MESSAGE(Log::error, "Failed test");
		++num_failures;
	}

	try
	{
		JEWEL_THROW(TrialException1_0_1, "Here's a TrialException1_0_1.");
	}
	catch (TrialException1_0_1& e)
	{
		JEWEL_ASSERT (string(e.what()) == string("Here's a TrialException1_0_1."));
	}
	catch (...)
	{
		JEWEL_LOG_MESSAGE(Log::error, "Failed test");
		++num_failures;
	}

	try
	{
		throw std::exception();
	}
	catch (std::exception&)
	{
	}
	catch (...)
	{
		JEWEL_LOG_MESSAGE(Log::error, "Failed test");
		++num_failures;
	}

	try
	{
		throw TrialException1_0_1("Here's a TrialException1_0_1.");
	}
	catch (std::exception&)
	{
	}
	catch (...)
	{
		JEWEL_LOG_MESSAGE(Log::error, "Failed test");
		++num_failures;
	}

	if (num_failures == 0)
	{
		cout << "Success! Exceptions classes created using these macros "
			 << "behaved as expected." << endl;
	}
	else
	{
		JEWEL_HARD_ASSERT (num_failures > 0);
		cout << "There were "
		     << num_failures
			 << " failed tests in test_exception_macros() function.\n"
			 << "Points of failure have been logged to the test log file."
			 << endl;
	}

	return num_failures;
}

}  // namespace detail
}  // namespace jewel
