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

#ifndef GUARD_on_windows_hpp_15737586289522046
#define GUARD_on_windows_hpp_15737586289522046


#	if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)

		/**
		 * Single macro that is defined if and only if we are
		 * on Windows, otherwise is not defined at all. This avoids having
		 * to type multiple underscore-related variations of "WIN32" every
		 * time we want to test whether we are on Windows.
		 */
#		define JEWEL_ON_WINDOWS

#	endif




#endif  // GUARD_on_windows_hpp_15737586289522046
