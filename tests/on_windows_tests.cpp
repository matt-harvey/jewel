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


#include "on_windows.hpp"

#include <UnitTest++/UnitTest++.h>


#include <iostream>

TEST(on_windows_test)
{
	bool on_windows = false;

	(void)on_windows;   // prevent warning re. unused variable

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
