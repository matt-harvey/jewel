// Copyright (c) 2013, Matthew Harvey. All rights reserved.

#include "on_windows.hpp"

#include <UnitTest++/UnitTest++.h>


#include <iostream>

TEST(on_windows_test)
{
	bool on_windows = false;
#	if defined(_WIN32)
		on_windows = JEWEL_ON_WINDOWS;
		CHECK(on_windows);
#	endif

#	if _WIN32
		on_windows = JEWEL_ON_WINDOWS;
		CHECK(on_windows);
#	endif

#	if defined(__WIN32__)
		on_windows = JEWEL_ON_WINDOWS;
		CHECK(on_windows);
#	endif

#	if __WIN32__
		on_windows = JEWEL_ON_WINDOWS;
		CHECK(on_windows);
#	endif

#	if defined(WIN32)
		on_windows = JEWEL_ON_WINDOWS;
		CHECK(on_windows);
#	endif

#	if WIN32
		on_windows = JEWEL_ON_WINDOWS;
		CHECK(on_windows);
#	endif

#	ifdef JEWEL_ON_WINDOWS
#		if !defined(WIN32) && !defined(_WIN32) && !defined(__WIN32__)
			CHECK(false);
#		endif
#	endif

}
