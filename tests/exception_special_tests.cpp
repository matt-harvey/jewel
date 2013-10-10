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
