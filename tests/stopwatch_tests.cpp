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


#include "stopwatch.hpp"
#include <ctime>
#include <iostream>

#include <UnitTest++/UnitTest++.h>
#include "stopwatch.hpp"

using std::ostringstream;
using jewel::Stopwatch;

TEST(construct_Stopwatch)
{
	Stopwatch sw0;
	for (int i = 0; i != 10000000; ++i)
	{
		// do nothing
	}
	ostringstream oss0;
	sw0.log(oss0);
	double d0 = sw0.seconds_elapsed();
	CHECK(d0 >= 0);
	CHECK(oss0.str() != "");
}

