// Copyright (c) 2013, Matthew Harvey. All rights reserved.

#ifndef GUARD_on_windows_hpp
#define GUARD_on_windows_hpp


#	if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)

		/**
		 * Single macro that is defined as 1 if and only if we are
		 * on Windows, otherwise is not defined at all. This avoids having
		 * to type multiple underscore-related variations of "WIN32" every
		 * time we want to test whether we are on Windows.
		 */
#		define JEWEL_ON_WINDOWS 1

#	endif




#endif  // GUARD_on_windows_hpp
