// Copyright (c) 2013, Matthew Harvey. All rights reserved.

#include "../on_windows.hpp"

#include <UnitTest++/UnitTest++.h>


#include <iostream>

TEST(on_windows_test)
{
	bool on_windows = false;
#	ifdef JEWEL_ON_WINDOWS
		on_windows = true;
#	endif

#	if defined(_WIN32)
		CHECK(on_windows);
#	endif

#	ifdef _WIN32
		CHECK(on_windows);
#	endif

#	if defined(__WIN32__)
		CHECK(on_windows);
#	endif

#	ifdef __WIN32__
		CHECK(on_windows);
#	endif

#	if defined(WIN32)
		CHECK(on_windows);
#	endif

#	ifdef WIN32
		CHECK(on_windows);
#	endif

#	ifdef JEWEL_ON_WINDOWS
#		if !defined(WIN32) && !defined(_WIN32) && !defined(__WIN32__)
			CHECK(false);
#		endif
#	endif

}
