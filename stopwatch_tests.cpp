// Copyright (c) 2013, Matthew Harvey. All rights reserved.

#include "stopwatch.hpp"
#include <boost/lexical_cast.hpp>
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

