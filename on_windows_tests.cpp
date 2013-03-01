#include "on_windows.hpp"

#include <UnitTest++/UnitTest++.h>


#include <iostream>

TEST(on_windows_test)
{
#	if defined(_WIN32)
		bool const on_windows = JEWEL_ON_WINDOWS;
		CHECK(on_windows);
#	endif

#	if _WIN32
		bool const on_windows = JEWEL_ON_WINDOWS;
		CHECK(on_windows)
#	endif

#	if defined(__WIN32__)
		bool const on_windows = JEWEL_ON_WINDOWS;
		CHECK(on_windows);
#	endif

#	if __WIN32__
		bool const on_windows = JEWEL_ON_WINDOWS;
		CHECK(on_windows)
#	endif

#	if defined(WIN32)
		bool const on_windows = JEWEL_ON_WINDOWS;
		CHECK(on_windows);
#	endif

#	if WIN32
		bool const on_windows = JEWEL_ON_WINDOWS;
		CHECK(on_windows)
#	endif

#	ifdef JEWEL_ON_WINDOWS
#		if !defined(WIN32) && !defined(_WIN32) && !defined(__WIN32__)
			CHECK(false);
#		endif
#	endif

}
