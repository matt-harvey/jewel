#include <cassert>
#include <cmath>
#include <iostream>
#include <vector>
#include <string>

#include <boost/foreach.hpp>
#include <unittest++/UnitTest++.h>
#include "power_vector.hpp"

using std::cerr;
using std::endl;
using std::pow;
using std::vector;
using std::string;

using jewel::power_vector;

// TESTS FOR POWERVECTOR FUNCTION

/// @cond (to hide this from Doxygen-generated docs)

struct PowerVectorFixture1
{
	// setup
	PowerVectorFixture1();

	// teardown
	~PowerVectorFixture1()
	{
	}

	// the "original vector"
	vector<int> vector0;

	// the members of vector0's power vector
	vector<int> vector0_0;
	vector<int> vector0_1;
	vector<int> vector0_2;
	vector<int> vector0_3;
	vector<int> vector0_4;
	vector<int> vector0_5;
	vector<int> vector0_6;
	vector<int> vector0_7;
	
	vector< vector<int> > power_vector0;
};

/// @endcond

PowerVectorFixture1::PowerVectorFixture1()
{
	// build vector0
	vector0.push_back(3);
	vector0.push_back(40);
	vector0.push_back(-8);
	assert(vector0.size() == 3);
	
	// build vector0_0
	// It's empty - nothing to push_back
	assert(vector0_0.size() == 0);

	// build vector0_1
	vector0_1.push_back(-8);
	assert(vector0_1.size() == 1);

	// build vector0_2
	vector0_2.push_back(40);
	assert(vector0_2.size() == 1);

	// build vector0_3
	vector0_3.push_back(40);
	vector0_3.push_back(-8);
	assert(vector0_3.size() == 2);

	// build vector0_4
	vector0_4.push_back(3);
	assert(vector0_4.size() == 1);

	// build vector0_5
	vector0_5.push_back(3);
	vector0_5.push_back(-8);
	assert(vector0_5.size() == 2);

	// build vector0_6
	vector0_6.push_back(3);
	vector0_6.push_back(40);
	assert(vector0_6.size() == 2);

	// build vector0_7
	vector0_7.push_back(3);
	vector0_7.push_back(40);
	vector0_7.push_back(-8);
	assert(vector0_7.size() == 3);

	// build power_vector0;
	power_vector0.push_back(vector0_0);
	power_vector0.push_back(vector0_1);
	power_vector0.push_back(vector0_2);
	power_vector0.push_back(vector0_3);
	power_vector0.push_back(vector0_4);
	power_vector0.push_back(vector0_5);
	power_vector0.push_back(vector0_6);
	power_vector0.push_back(vector0_7);
	assert(power_vector0.size() == 8);

}


// Test exact identity of vector0's powervector
TEST_FIXTURE(PowerVectorFixture1, TestPowerVectorID1)
{
	// UnitTest++ does not seem able to handle
	// this. 
	// Seems to be something to do with
	// inability to print params as string
	// CHECK_EQUAL(power_vector(vector0), power_vector0);
	// So testing with simple CHECK instead...

	CHECK(power_vector(vector0) == power_vector0);
}

// Test the cardinality of several powervectors
TEST_FIXTURE(PowerVectorFixture1, TestPowerVectorSize1)
{	
	CHECK(power_vector(vector0).size() == 8);
	CHECK(power_vector(vector0_0).size() == 1);
	CHECK(power_vector(vector0_1).size() == 2);
	CHECK(power_vector(vector0_2).size() == 2);
	CHECK(power_vector(vector0_3).size() == 4);
	CHECK(power_vector(vector0_4).size() == 2);
	CHECK(power_vector(vector0_5).size() == 4);
	CHECK(power_vector(vector0_6).size() == 4);
	CHECK(power_vector(vector0_7).size() == 8);
	CHECK(power_vector(power_vector0).size() ==
	  pow(double(2), double(8)));
}	


/// @cond (to hide this from Doxygen-generated docs)

struct PowerVectorFixture2
{
	// setup
	PowerVectorFixture2();

	// teardown
	~PowerVectorFixture2()
	{
	}

	// the "original vector"
	vector<string> vector0;

	// the members of vector0's powervector
	vector<string> vector0_0;
	vector<string> vector0_1;
	vector<string> vector0_2;
	vector<string> vector0_3;
	
	// vector0's powervector
	vector< vector<string> > power_vector0;
};

/// @endcond

PowerVectorFixture2::PowerVectorFixture2()
{
	// build vector0
	vector0.push_back("One");
	vector0.push_back("Two");
	assert(vector0.size() == 2);
	
	// build vector0_0
	// It's empty - nothing to push_back
	assert(vector0_0.size() == 0);

	// build vector0_1
	vector0_1.push_back("Two");
	assert(vector0_1.size() == 1);

	// build vector0_2
	vector0_2.push_back("One");
	assert(vector0_2.size() == 1);

	// build vector0_3
	vector0_3.push_back("One");
	vector0_3.push_back("Two");
	assert(vector0_3.size() == 2);
	
	// built power_vector0;
	power_vector0.push_back(vector0_0);
	power_vector0.push_back(vector0_1);
	power_vector0.push_back(vector0_2);
	power_vector0.push_back(vector0_3);
	assert(power_vector0.size() == 4);

}

TEST_FIXTURE(PowerVectorFixture2, TestPowerVectorID2)
{
	CHECK(power_vector(vector0) == power_vector0);
}

// Test the cardinality of several power_vector
TEST_FIXTURE(PowerVectorFixture2, TestPowerVectorSize2)
{	
	CHECK(power_vector(vector0).size() == 4);
	CHECK(power_vector(vector0_0).size() == 1);
	CHECK(power_vector(vector0_1).size() == 2);
	CHECK(power_vector(vector0_2).size() == 2);
	CHECK(power_vector(vector0_3).size() == 4);
	CHECK(power_vector(power_vector0).size() ==
	  pow(double(2), double(4)));
}	

	
