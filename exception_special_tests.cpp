#include "exception_special_tests.hpp"
#include <cassert>
#include <iostream>
#include <stdexcept>

using std::cout;
using std::endl;
using std::exception;


namespace jewel
{

namespace detail
{

void test_exception_macros()
{
	try
	{
		throw TrialException0("Here's a TrialException0.");
	}
	catch (exception&)
	{
	}

	try
	{
		throw TrialException1("Here's a TrialException1.");
	}
	catch (exception&)
	{
	}

	try
	{
		throw TrialException0_0("Here's TrialException0_0.");
	}
	catch (exception&)
	{
	}

	try
	{
		throw TrialException0_0("Here a TrialException0_0.");
	}
	catch (TrialException0&)
	{
	}

	try
	{
		throw TrialException1_0("Here's TrialException1_0.");
	}
	catch (exception&)
	{
	}

	try
	{
		throw TrialException1_0("Here a TrialException1_0.");
	}
	catch (TrialException1&)
	{
	}

	try
	{
		throw TrialException1_0("Here's a TrialException1_0.");
	}
	catch (TrialException0&)
	{
		// This should be reached as TrialException0 should not
		// catch TrialException1_0.
		assert (false);
	}
	catch (TrialException1&)
	{
		// But this should catch it.
	}

	try
	{
		throw TrialException1_0_1("Here's a TrialException1_0_1.");
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
		throw TrialException1_0_1("Here's a TrialException1_0_1.");
	}
	catch (exception&)
	{
	}

	cout << "Success! Exceptions classes created using these macros "
	     << "behaved as expected." << endl;

	return;
}




}  // namespace detail

}  // namespace jewel
