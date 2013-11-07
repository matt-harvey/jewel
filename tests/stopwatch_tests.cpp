/*
 * Copyright 2012-2013 Matthew Harvey
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

