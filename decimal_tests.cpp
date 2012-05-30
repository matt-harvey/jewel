#include "decimal.hpp"
#include "arithmetic_exceptions.hpp"
#include "num_digits.hpp"

#include <limits>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <boost/lexical_cast.hpp>
#include <unittest++/UnitTest++.h>

using boost::lexical_cast;
using jewel::Decimal;
using jewel::NumDigits;
using jewel::UnsafeArithmeticException;
using jewel::round;
using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::istringstream;
using std::ostringstream;
using std::numeric_limits;
using std::ostringstream;
using std::string;

typedef Decimal::int_type int_type;


TEST(decimal_parameterless_constructor)
{
	Decimal d0;
	CHECK_EQUAL(d0, Decimal("0"));
	CHECK_EQUAL(d0, Decimal("-0"));
	CHECK_EQUAL(d0 + d0, d0);
	CHECK_EQUAL(d0 - d0, d0);
}

/* The function being tested here is now dormant and commented out. 
TEST(decimal_int_type_constructor_with_default_places)
{
	Decimal::int_type i0 = 3;	
	Decimal d0(i0);
	CHECK_EQUAL(d0, Decimal("3"));
	Decimal d1(int_type(50));
	CHECK_EQUAL(d1, Decimal("50"));
	Decimal d2(int_type(-234));
	CHECK_EQUAL(d2, Decimal("-234"));
	Decimal d3(int_type(0));
	CHECK_EQUAL(d3, Decimal("0"));
	Decimal d4(int_type(-0));
	CHECK_EQUAL(d4, Decimal("0"));
}
*/

/* The function being tested here is now dormant and commented out. 
TEST(decimal_int_type_constructor_with_specified_places)
{
	Decimal::int_type i0 = 394;
	unsigned short us0 = 4;
	Decimal d0(i0, us0);
	CHECK_EQUAL(d0, Decimal("0.0394"));
	Decimal::int_type i1 = -234234;
	unsigned short us1 = 1;
	Decimal d1(i1, us1);
	CHECK_EQUAL(d1, Decimal("-23423.4"));
	Decimal::int_type i2 = -23;
	unsigned short us2 = 10;
	CHECK_THROW(Decimal d2(i2, us2), UnsafeArithmeticException);
	Decimal::int_type i3(-23);
	unsigned short us3 = 9;
	Decimal d3(i3, us3);
	CHECK_EQUAL(d3, Decimal("-.000000023"));
}
*/

/* The function being tested here is now dormant and commented out.
TEST(decimal_int_constructor_with_default_places)
{
	Decimal d0(3);
	CHECK_EQUAL(d0, Decimal("3"));
	Decimal d1(50);
	CHECK_EQUAL(d1, Decimal("50"));
}
*/

/* The function being tested here is now dormant and commented out.
TEST(decimal_int_constructor_with_specified_places)
{
	Decimal d0(3, 2);
	CHECK_EQUAL(d0, Decimal("0.03"));
	Decimal d1(-4340, 3);
	CHECK_EQUAL(d1, Decimal("-4.34"));
	CHECK_EQUAL(d1, Decimal("-4.340"));
}
*/


/* The function being tested here is now dormant and commented out.
TEST(decimal_double_constructor)
{
	double doub0 = 300.9;
	CHECK_THROW(Decimal d0(doub0), UnsafeArithmeticException);
	float float0 = float(300.9);
	CHECK_THROW(Decimal d1(float0), UnsafeArithmeticException);
	double doub1 = 0;
	CHECK_THROW(Decimal d2(doub1), UnsafeArithmeticException);
	float float1 = 0;
	CHECK_THROW(Decimal d3(float1), UnsafeArithmeticException);
}
*/


TEST(decimal_string_constructor)
{
	// Test basic functionality
	Decimal d0("0");
	Decimal d0b;
	Decimal d0c("-0.0");
	CHECK_EQUAL(d0, Decimal("0"));
	CHECK_EQUAL(d0b, d0);
	CHECK_EQUAL(d0c, d0);
	Decimal d1("-98.234");
	CHECK(d0 != d1);
	CHECK_EQUAL(d1, Decimal("-98.23400"));
	
	// Test behaviour with empty string
	CHECK_THROW(Decimal d10(""), UnsafeArithmeticException);

	// Test behaviour with prohibited characters in string
	CHECK_THROW(Decimal d11("9.90b0"), UnsafeArithmeticException);
	CHECK_THROW(Decimal d12("6,8"), UnsafeArithmeticException);
	CHECK_THROW(Decimal d13("Llama"), UnsafeArithmeticException);
	CHECK_THROW(Decimal d13b("9.0.3"), UnsafeArithmeticException);
	CHECK_THROW(Decimal d13c("-79-"), UnsafeArithmeticException);
	CHECK_THROW(Decimal d13d("0-0"), UnsafeArithmeticException);
	
	// Test behaviour with attempted Decimal having too large a would-be
	// underlying integer.
	CHECK_THROW(Decimal d14("9792347897"), UnsafeArithmeticException);
	CHECK_THROW(Decimal d15("-.9000000000"), UnsafeArithmeticException);
	CHECK_THROW(Decimal d16("-.1234124121767123"), UnsafeArithmeticException);
	// But these should't throw
	Decimal d18("100000030");
	Decimal d19("1.00000030");
	Decimal d20("-1.00000030");
	Decimal d20b("0000000001");
	Decimal d21(".000000430");
	Decimal d22("-.999999489");
	Decimal d23(".000000000");
}

/* The function being tested here is now dormant and commented out.
TEST(decimal_read_parts_from_stream)
{
	istringstream is0("90 3");
	Decimal d0;
	d0.read_parts_from_stream(is0);
	CHECK_EQUAL(d0, Decimal(".090"));
	istringstream is1("90 3 -100 0");
	Decimal d1a;
	Decimal d1b;
	d1a.read_parts_from_stream(is1);
	CHECK_EQUAL(d1a, Decimal("0.090"));
	d1b.read_parts_from_stream(is1);
	CHECK_EQUAL(d1b, Decimal("-100"));
}
*/

/* The function being tested here is now dormant and commented out.
TEST(decimal_write_parts_to_stream)
{	
	ostringstream os;
	Decimal d0("-90.234");
	d0.write_parts_to_stream(os);
	CHECK_EQUAL(os.str(), "-90234 3");
	Decimal d1(423798);
	os << endl;
	d1.write_parts_to_stream(os);
	CHECK_EQUAL(os.str(), "-90234 3\n423798 0");
}
*/

TEST(decimal_assignment)
{
	Decimal d0("3074.2340");
	Decimal d1("34.234");
	d1 = d0;
	CHECK_EQUAL(d0, d1);
	CHECK_EQUAL(d1, Decimal("3074.2340"));
	CHECK(d0 != Decimal("34.234"));
}

TEST(decimal_plus_equals)
{
	Decimal d0("907.89");
	Decimal d1("-9.0878");
	d0 += d1;
	CHECK_EQUAL(d0, Decimal("898.8022"));
	CHECK_EQUAL(d1, Decimal("-9.0878"));
	Decimal d2("8.890971");
	Decimal d3("5.7");
	d2 += d3;
	CHECK_EQUAL(d2, Decimal("14.590971"));
	CHECK_EQUAL(d3, Decimal("5.7"));

	// Check behaviour for unsafe operations

	// With precision loss
	Decimal d10("100000000");
	Decimal d11("0.00000001");
	CHECK_THROW(d10 += d11, UnsafeArithmeticException);
	Decimal d12("7927439");
	CHECK_THROW(d12 += Decimal(".001"), UnsafeArithmeticException);
	Decimal d13("-.900009");
	CHECK_THROW(d13 += Decimal("10000"), UnsafeArithmeticException);
	Decimal d14("09.009423");
	CHECK_THROW(d14 += Decimal("2924"), UnsafeArithmeticException);
	Decimal d15("-.98984790");
	CHECK_THROW(d15 += Decimal("-2342422"), UnsafeArithmeticException);
	// With straightforward overflow
	ostringstream oss;
	oss << numeric_limits<int_type>::max() / 11;
	Decimal large_num(oss.str());
	Decimal large_neg_num = -large_num;
	Decimal large_num_b = large_num;
	Decimal large_neg_num_b = large_neg_num;
	CHECK_THROW(for (int i = 0; i != 12; ++i) large_num += large_num,
	  UnsafeArithmeticException);
	CHECK_THROW(for (int i = 0; i != 12; ++i) large_neg_num += large_neg_num,
	  UnsafeArithmeticException);
	// But these shouldn't throw
	Decimal d17(".9324");
	d17 += Decimal("24.24");
	Decimal d18("1000000");
	d18 += Decimal("9284792");
	Decimal d19(".0242234");
	d19 += Decimal("19.12");
	Decimal d20("100000");
	d20 += Decimal(".001");
	large_num_b += Decimal("10");
	large_neg_num_b += large_num_b;
	large_num_b += (-large_num_b);
}

TEST(decimal_addition)
{
	Decimal d0("-.30010");
	Decimal d1("-400.678");
	Decimal d2 = d0 + d1;
	CHECK_EQUAL(d2, Decimal("-400.97810"));
	CHECK_EQUAL(d2, Decimal("-400.9781"));
	Decimal d3("-.00000001");
	Decimal d4("100000000");
	CHECK_THROW(Decimal d5 = d3 + d4, UnsafeArithmeticException);

	// Check behaviour for unsafe operations

	// With precision loss
	Decimal d10("100000000");
	Decimal d11("0.00000001");
	CHECK_THROW(d10 = d10 + d11, UnsafeArithmeticException);
	Decimal d12("7927439");
	CHECK_THROW(Decimal d12b = d12 + Decimal(".001"),
	  UnsafeArithmeticException);
	Decimal d13("-.900009");
	CHECK_THROW(Decimal d13b = d13 + Decimal("10000"),
	  UnsafeArithmeticException);
	Decimal d14("09.009423");
	CHECK_THROW(d14 = d14 + Decimal("2924"), UnsafeArithmeticException);
	Decimal d15("-.98984790");
	CHECK_THROW(Decimal d15b = d15 + Decimal("-2342422"),
	  UnsafeArithmeticException);
	// With straightforward overflow
	ostringstream oss;
	oss << numeric_limits<int_type>::max() / 11;
	Decimal large_num(oss.str());
	Decimal large_neg_num = -large_num;
	Decimal large_num_b = large_num;
	Decimal large_neg_num_b = large_neg_num;
	CHECK_THROW(for (int i = 0; i != 12; ++i)
	  large_num = large_num + large_num, UnsafeArithmeticException);
	CHECK_THROW(for (int i = 0; i != 12; ++i)
	  large_neg_num = large_neg_num + large_neg_num,
	  UnsafeArithmeticException);
	// But these shouldn't throw
	Decimal d17(".9324");
	d17 += Decimal("24.24");
	Decimal d18("1000000");
	Decimal d18b = d18 + Decimal("9284792");
	Decimal d19(".0242234");
	Decimal d19b = d19 + Decimal("19.12");
	Decimal d20("100000");
	d20 = d20 + Decimal(".001");
	large_num_b = large_num_b + Decimal("10");
	Decimal large_neg_num_c = large_neg_num_b + large_num_b;
	large_num_b = large_num_b + (-large_num_b);


}

TEST(decimal_minus_equals)
{
	Decimal d0("30.86");
	Decimal d1("20000.9");
	Decimal d2("-19970.04");
	d0 -= d1;
	CHECK_EQUAL(d0, d2);
	CHECK_EQUAL(Decimal("0.23420") - Decimal("0.03"), Decimal("0.2042"));
	CHECK_EQUAL(Decimal("100") - Decimal("99"), Decimal("1.00"));
	CHECK_EQUAL(Decimal("4231.123") - Decimal("-0"),
	  Decimal("4231.123"));
	CHECK_EQUAL(Decimal("-234") - Decimal(".0903"), Decimal("-234.0903"));
	
	// Check behaviour for unsafe operations

	// With precision loss
	Decimal d10("100000000");
	Decimal d11("-0.00000001");
	CHECK_THROW(d10 -= d11, UnsafeArithmeticException);
	Decimal d12("7927439");
	CHECK_THROW(d12 -= Decimal("-.001"), UnsafeArithmeticException);
	Decimal d13("-.900009");
	CHECK_THROW(d13 -= Decimal("-10000"), UnsafeArithmeticException);
	Decimal d14("09.009423");
	CHECK_THROW(d14 -= Decimal("-2924"), UnsafeArithmeticException);
	Decimal d15("-.98984790");
	CHECK_THROW(d15 -= Decimal("2342422"), UnsafeArithmeticException);
	// With straightforward overflow
	ostringstream oss;
	oss << numeric_limits<int_type>::max() / 11;
	Decimal large_num(oss.str());
	Decimal large_neg_num = -large_num;
	Decimal large_num_b = large_num;
	Decimal large_neg_num_b = large_neg_num;
	CHECK_THROW(for (int i = 0; i != 12; ++i) large_neg_num -= large_num,
	  UnsafeArithmeticException);
	CHECK_THROW(for (int i = 0; i != 12; ++i) large_num -= large_neg_num,
	  UnsafeArithmeticException);
	// But these shouldn't throw
	Decimal d17(".9324");
	d17 -= Decimal("-24.24");
	Decimal d18("1000000");
	d18 -= Decimal("9284792");
	Decimal d19(".0242234");
	d19 -= Decimal("-19.12");
	Decimal d20("100000");
	d20 -= Decimal(".001");
	large_num_b -= Decimal("10");
	large_neg_num_b -= large_neg_num_b;
	large_num_b -= large_num_b;
}

TEST(decimal_subtraction)
{
	Decimal d0("30.86");
	Decimal d1("20000.978");
	Decimal d2("-19970.1180");
	CHECK_EQUAL(d0 - d1, d2);
	CHECK_EQUAL(d2 - d1, Decimal("-39971.096"));
	CHECK_EQUAL(Decimal("0.009") - Decimal("0.003"), Decimal("0.006"));
	CHECK_EQUAL(Decimal("234") - Decimal("-0"), Decimal("234"));
	
	// Check behaviour for unsafe operations

	// With precision loss
	Decimal d10("100000000");
	Decimal d11("-0.00000001");
	CHECK_THROW(Decimal d11b = d10 - d11, UnsafeArithmeticException);
	Decimal d12("7927439");
	CHECK_THROW(Decimal d12b = d12 - Decimal("-.001"),
	  UnsafeArithmeticException);
	Decimal d13("-.900009");
	CHECK_THROW(Decimal d13b = d13 - Decimal("10000"),
	  UnsafeArithmeticException);
	Decimal d14("09.009423");
	CHECK_THROW(d14 = d14 - Decimal("2924"), UnsafeArithmeticException);
	Decimal d15("-.98984790");
	CHECK_THROW(Decimal d15b = d15 - Decimal("2342422"),
	  UnsafeArithmeticException);
	// With straightforward overflow
	ostringstream oss;
	oss << numeric_limits<int_type>::max() / 11;
	Decimal large_num(oss.str());
	Decimal large_neg_num = -large_num;
	Decimal large_num_b = large_num;
	Decimal large_neg_num_b = large_neg_num;
	CHECK_THROW(for (int i = 0; i != 12; ++i) large_neg_num =
	  large_neg_num - large_num, UnsafeArithmeticException);
	CHECK_THROW(for (int i = 0; i != 12; ++i) large_num =
	  large_num - large_neg_num, UnsafeArithmeticException);
	// But these shouldn't throw
	Decimal d17(".9324");
	d17 = d17 - Decimal("-24.24");
	Decimal d18("1000000");
	Decimal d18b("0");
	d18b = d18 - Decimal("9284792");
	Decimal d19(".0242234");
	Decimal d19b;
	d19b = d19 - Decimal("-19.12");
	Decimal d20("100000");
	d20 = d20 - Decimal(".001");
	large_num_b = large_num_b - Decimal("10");
	Decimal large_neg_num_d = large_neg_num_b + Decimal("-1");
	large_neg_num_b = large_neg_num_d - large_neg_num_b;
	Decimal large_num_e = large_num_b - Decimal("24");
	large_num_b = large_num_e - large_num_b;
}

TEST(decimal_multiply_equals)
{
	Decimal d0("56.9");
	Decimal d1("12");
	d0 *= d1;
	Decimal d2("682.8");
	CHECK_EQUAL(d0, d2);
	Decimal d3("90");
	Decimal d4("0");
	d3 *= d4;
	CHECK_EQUAL(d3, Decimal("0"));
	Decimal d50("90.27427");
	d50 *= Decimal("-118381.12");
	CHECK(d50 < Decimal("-10686769"));
	CHECK(d50 > Decimal("-10686770"));
	Decimal d100 = Decimal("900001") * Decimal("0.2234");
	CHECK(d100 > Decimal("201060.2"));
	CHECK(d100 < Decimal("201060.3"));

	// Test behaviour with unsafe operations
	
	// With straightforward overflow
	ostringstream oss;
	oss << numeric_limits<int_type>::max() / 11;
	Decimal large_num(oss.str());
	Decimal large_neg_num = -large_num;
	CHECK_THROW(large_num *= large_num, UnsafeArithmeticException);
	CHECK_THROW(large_num *= Decimal("-29"), UnsafeArithmeticException);
	CHECK_THROW(large_neg_num * large_num, UnsafeArithmeticException);
	
	// But these should shouldn't throw
	Decimal d101 = large_num * Decimal("0");
	Decimal d102 = -large_num * Decimal("1");
	Decimal d103 = large_num * Decimal("-1.000");
	
}

TEST(decimal_multiplication)
{
	Decimal d0("-100789.8");
	Decimal d1("-78.9");
	Decimal d2 = d0 * d1;
	Decimal d3 = Decimal("7952315.22");
	CHECK_EQUAL(d2, d3);
	Decimal d4("0.1008");
	Decimal d5("0.7000024");
	CHECK(d4 * d5 > Decimal("0.0705602"));
	CHECK(d4 * d5 < Decimal("0.0705603"));
	Decimal d6("0.00000001");
	Decimal d7("0.0000001");
	Decimal d8 = d6 * d7;
	CHECK_EQUAL(d8, Decimal("0"));

	// Test behaviour with unsafe operations
	
	// With straightforward overflow
	ostringstream oss;
	oss << numeric_limits<int_type>::max() / 11;
	Decimal large_num(oss.str());
	Decimal large_neg_num = -large_num;
	CHECK_THROW(Decimal large_num_b = large_num * large_num,
	  UnsafeArithmeticException);
	CHECK_THROW(Decimal large_num_c = Decimal("-29") * large_num,
	  UnsafeArithmeticException);
	CHECK_THROW(Decimal large_neg_num_b = large_neg_num * large_num,
	  UnsafeArithmeticException);
}

TEST(decimal_divide_equals)
{
	Decimal d0("1");
	Decimal d1("3");
	d0 /= d1;
	CHECK(d0 > Decimal("0.3333332"));
	CHECK(d0 < Decimal("0.3333334"));
	Decimal d3("91");
	Decimal d4("7.09010");
	d3 /= d4;
	CHECK(d3 > Decimal("12.834797"));
	CHECK(d3 < Decimal("12.834799"));
	Decimal d5("2");
	Decimal d6("3.00");
	d5 /= d6;
	CHECK(d5 > Decimal("0.6666666"));
	CHECK(d5 < Decimal("0.6666668"));
	Decimal d8("-56");
	d8 /= Decimal("7");
	CHECK_EQUAL(d8, Decimal("-8"));
	Decimal d9("-1");
	d9 /= Decimal("-11.00000");
	CHECK(d9 > Decimal("0.0909090"));
	CHECK(d9 < Decimal("0.0909091"));
	Decimal d10("1090");
	Decimal d11(".00001");
	CHECK_EQUAL(d10 / d11, Decimal("109000000"));

	// Check behaviour with unsafe operations
	
	// With straightforward overflow
	Decimal d101("-.03842");	
	CHECK_THROW(d101 /= Decimal("-23874"), UnsafeArithmeticException);
	Decimal d102("3.24");
	CHECK_THROW(d102 /= Decimal("23342342.4"), UnsafeArithmeticException);
	Decimal d103("-43.234234");
	Decimal d103b("-4345234");
	CHECK_THROW(d103 /= d103b, UnsafeArithmeticException);
	Decimal d104("-197787.987");
	Decimal d104b(".9879");
	CHECK_THROW(d104 /= d104b, UnsafeArithmeticException);
}

TEST(decimal_division)
{
	Decimal d0("0.07");
	Decimal d1("0.02");
	d0 = d0 / d1;
	CHECK_EQUAL(d0, Decimal("3.5"));
	CHECK(Decimal("4.863") / Decimal("95") > Decimal("0.05118"));
	CHECK(Decimal("4.863") / Decimal("95") < Decimal("0.05119"));
	
	// Check behaviour with unsafe operations
	
	// with straightforward overflow
	CHECK_THROW(Decimal d101 = Decimal("-.03842") / Decimal("-23874"),
	  UnsafeArithmeticException);
	CHECK_THROW(Decimal d102 = Decimal("3.24") / Decimal("23442300.98"),
	  UnsafeArithmeticException);
	CHECK_THROW(Decimal d103 = Decimal("-43.434234") / Decimal("-4234234"),
	  UnsafeArithmeticException);
	CHECK_THROW(Decimal d104 = Decimal("-197787.987") / Decimal(".9879"),
	  UnsafeArithmeticException);
}


/// @cond (to hide this from Doxygen-generated docs)

// Base class for increment-testing and decrement-testing
// test fixtures
struct DecimalTestFixtureForIncrDecr
{
	// setup
	void create(bool negs = false);

	// teardown
	~DecimalTestFixtureForIncrDecr()
	{
	}

	// the members we need in tests
	int_type starting_point;
	int_type maxi;
	int_type mini;
	ostringstream oss;
	Decimal multiplier;
	Decimal x;
};

// Generic set up for testing increment and decrement operators
void DecimalTestFixtureForIncrDecr::create(bool negs)
{
	maxi = numeric_limits<int_type>::max();
	mini = numeric_limits<int_type>::min();
	
	int_type const& limit = (negs? mini: maxi);  // A reference!

	// Start off here
	starting_point = (negs? -1: 1);
	
	// And increase to the highest number with whose string translation a
	// Decimal can be initialized.
	while (NumDigits::num_digits(starting_point) <
	  NumDigits::num_digits(limit) - 1)
	{
		starting_point *= 10;
	}
	assert (NumDigits::num_digits(starting_point) ==
	  NumDigits::num_digits(limit) - 1);

	// Initialize a Decimal with the help of the number we've reached.
	oss << starting_point;
	x = Decimal(oss.str());

	if (negs) assert (x < Decimal("0"));
	else assert (x > Decimal("0"));

	// Then get the Decimal a bit higher still
	multiplier = Decimal("1.001");
	while (true)
	{
		try
		{
			x *= multiplier;
		}
		catch (UnsafeArithmeticException&)
		{
			break;
		}
	}

	return;
}

// Fixture specifically for increment test
struct DecimalTestFixtureForIncr: public DecimalTestFixtureForIncrDecr
{
	DecimalTestFixtureForIncr()
	{
		create();
	}
};

// Fixture specifically for decrement test
struct DecimalTestFixtureForDecr: public DecimalTestFixtureForIncrDecr
{
	DecimalTestFixtureForDecr()
	{
		create(true);
	}
};


/// @endcond  // Ends "hiding from Doxygen"

TEST_FIXTURE(DecimalTestFixtureForIncr, decimal_increment)
{
	Decimal d0("0.007");
	++d0;
	CHECK_EQUAL(d0, Decimal("1.007"));
	d0++;
	CHECK_EQUAL(d0, Decimal("2.007"));
	d0++;
	++d0;
	Decimal d1 = d0++;
	CHECK_EQUAL(d1, Decimal("4.007"));
	d1 = ++d0;
	CHECK_EQUAL(d0, Decimal("6.007"));
	Decimal d2("-3");
	++d2;
	++d2;
	CHECK_EQUAL(d2, Decimal("-1"));

	// Test behaviour with unsafe operations
	// This is a bit complicated due to the inability to initialize
	// a Decimal with string that is \e very close to the maximum int_type.
	// (Arguably this is a bug, but anyway, that's not what we're testing
	// here.) We use the test fixture to set-up (above).
	
	// Create an int_type to "parallel" our progress as we increment
	// x towards its maximum safe value.
	ostringstream os2;
	os2 << x;
	int_type parallelx = lexical_cast<int_type>(os2.str());

	// Now confirm that if we continue to increment, we'll throw before
	// we exceed the equivalent of numeric_limits<int_type>::max()
	Decimal prefixee = x;
	int_type parallelpref = parallelx;
	CHECK_THROW(while (parallelpref <= maxi) { ++prefixee; ++parallelpref; },
	  UnsafeArithmeticException);
	CHECK_EQUAL(parallelpref, maxi);

	// Do the same with the suffix form
	Decimal suffixee = x;
	int_type parallelsuff = parallelx;
	CHECK_THROW(while (parallelsuff <= maxi) { suffixee++; parallelsuff++; },
	  UnsafeArithmeticException);
	CHECK_EQUAL(parallelsuff, maxi);
}

TEST_FIXTURE(DecimalTestFixtureForDecr, decimal_decrement)
{
	Decimal d0("3");
	--d0;
	CHECK_EQUAL(d0, Decimal("2"));
	Decimal d1("-78.23090");
	--d1;
	CHECK_EQUAL(d1, Decimal("-79.2309"));
	Decimal d2("7897");
	CHECK_EQUAL(--d2, Decimal("7896"));
	d2--;
	CHECK_EQUAL(d2, Decimal("7895"));

	// Now test behaviour with unsafe operations, using fixture as per
	// increment test

	assert (x < Decimal("0"));

	// Create an int_type to "parallel" our progress as we decrement
	// x towards its minimum safe value.
	ostringstream os2;
	os2 << x;
	int_type parallelx = lexical_cast<int_type>(os2.str());
	
	assert (parallelx < 0);

	// Now confirm that if we continue to decrement, we'll throw before
	// we exceed the equivalent of numeric_limits<int_type>::min()
	Decimal prefixee = x;
	int_type parallelpref = parallelx;
	CHECK_THROW(while (parallelpref >= mini) { --prefixee; --parallelpref; },
	  UnsafeArithmeticException);
	CHECK_EQUAL(parallelpref, mini);

	// Do the same with the suffix form
	Decimal suffixee = x;
	int_type parallelsuff = parallelx;
	CHECK_THROW(while (parallelsuff >= mini) { suffixee--; parallelsuff--; },
	  UnsafeArithmeticException);
	CHECK_EQUAL(parallelsuff, mini);
}

TEST(decimal_operator_less_than)
{
	Decimal d0("19.98");
	Decimal d1("19.981");
	CHECK(d0 < d1);
	Decimal d2("-1");
	Decimal d3("1.0000");
	CHECK(d2 < d3);
	CHECK(Decimal("0") < Decimal(".034"));
	CHECK(Decimal("-.34") < Decimal("-0.000"));
	CHECK(!(Decimal(".34") < Decimal("0.000")));
	CHECK(!(Decimal("-0.000") < Decimal("0")));
	CHECK(!(Decimal("0") < Decimal("-0.000")));
	CHECK(Decimal(".18791274") < Decimal("123412134"));
	CHECK(Decimal("-0.000000001") < Decimal("91341971"));
	CHECK(Decimal("-0.9000098") < Decimal("-0.9"));
	CHECK(Decimal("1.22390423") < Decimal("1.223904231"));
	CHECK(Decimal("201060.2234") < Decimal("201060.3"));
	CHECK(Decimal("201060.2") < Decimal("201060.2234"));
}

TEST(decimal_operator_greater_than)
{
	CHECK(Decimal("2.342809") > Decimal("2.34"));
	CHECK(Decimal("-1") > Decimal("-1.0099"));
	CHECK(Decimal("2.093") > Decimal("-0.12"));
	CHECK(Decimal("789") > Decimal("788.089234"));
	CHECK(Decimal("0") > Decimal("-0.3"));
	Decimal d0("100000000");
	Decimal d1(".00000001");
	CHECK(d0 > d1);
	CHECK(!(Decimal("0.000001") > Decimal("9000000")));
	CHECK(Decimal("-.000003") > Decimal("-112341878"));
	CHECK(Decimal("201060.2234") > Decimal("201060.2"));
	CHECK(Decimal("201060.3") > Decimal("201060.2234"));
	CHECK(Decimal("1") < Decimal("1.00000001"));
}

TEST(decimal_operator_equality)
{
	Decimal d0("-0.0000001");
	Decimal d1("-0.00000010");
	CHECK_EQUAL(d0, d1);
	CHECK(d0 == d1);
	CHECK(Decimal("1234.00") == Decimal("1234"));
	CHECK(Decimal(".988") == Decimal("000.988"));
	CHECK_EQUAL(Decimal("-.34"), Decimal("-0.34"));
	CHECK(Decimal("99") == Decimal("99"));
	CHECK(!(Decimal("9") == Decimal("90")));
	CHECK(!(Decimal("-38") == Decimal("-380")));
	CHECK(Decimal("0.000") == Decimal("-0"));
	CHECK(Decimal("234.123000") == Decimal("234.123"));
}

TEST(decimal_operator_inequality)
{
	Decimal d0("-.992");
	Decimal d1("-.34");
	CHECK(d0 != d1);
	Decimal d2("43.2");
	Decimal d3("432");
	CHECK(d2 != d3);
	CHECK(Decimal("99") != Decimal("9"));
	CHECK(Decimal("-38") != Decimal("-380"));
	CHECK(!(Decimal("0.001") != Decimal(".00100")));
	CHECK(Decimal("23.342234") != Decimal("23.34223"));
}

TEST(decimal_operator_output)
{
	ostringstream os0;
	ostringstream os1;
	os0 << "3.0001";
	os1 << Decimal("3.000100");
	CHECK_EQUAL(os0.str(), os1.str());
	ostringstream os2;
	os2 << "3";
	CHECK(os1.str() != os2.str());
	ostringstream os3;
	ostringstream os4;
	os3 << "501.001";
	Decimal d4("501");
	d4 += Decimal(".001");
	os4 << d4;
	CHECK_EQUAL(os4.str(), os3.str());
	ostringstream os5;
	os5 << "-2.001009";
	ostringstream os6;
	os6 << Decimal("-2.001009");
	CHECK_EQUAL(os5.str(), os6.str());
	ostringstream os7;
	ostringstream os8;
	os7 << "0.000000";
	os8 << Decimal("0");
	CHECK(os7.str() != os8.str());
	ostringstream os9;
	ostringstream os10;
	os9 << "-0.00002";
	os10 << Decimal("-.000020");
	CHECK_EQUAL(os9.str(), os10.str());
	ostringstream os11;
	ostringstream os12;
	os11 << "-3001.09";
	os12 << Decimal("-3001.09");
	CHECK_EQUAL(os11.str(), os12.str());
}

TEST(decimal_operator_input)
{
	istringstream is("90.789 234.2 -8");
	Decimal d0;
	Decimal d1;
	Decimal d2;
	is >> d0;
	is >> d1 >> d2;
	CHECK_EQUAL(d0, Decimal("90.789"));
	CHECK_EQUAL(d1, Decimal("234.2"));
	Decimal d3("-8.00000");
	CHECK_EQUAL(d2, d3);
	assert (is);  // The UnitTest++ CHECK macro won't compile here, for
	              // some obscure reason. So using assert.

	// What about with wonky reads?
	istringstream bis("90.00.0");
	Decimal d100;
	bis >> d100;
	assert (!bis);
	// Check the value has not changed
	CHECK_EQUAL(d100, Decimal("0"));
	Decimal d101("1234");
	istringstream bis2("8979ab");
	bis2 >> d101;
	assert (!bis2);
	// Check the value has not changed
	CHECK_EQUAL(d101, Decimal("1234"));
}


TEST(decimal_operator_unary_minus)
{
	Decimal d0("0");
	Decimal d1 = -d0;
	CHECK_EQUAL(d0, d1);
	Decimal d2("-1.03");
	Decimal d3 = -d2;
	CHECK_EQUAL(d3, Decimal("1.03"));
	CHECK_EQUAL(-Decimal("100"), Decimal("-100"));
	Decimal dmin = Decimal::minimum();
	CHECK_THROW(-dmin, UnsafeArithmeticException);
}

TEST(decimal_operator_unary_plus)
{
	CHECK_EQUAL(Decimal("89"), +Decimal("89"));
	CHECK_EQUAL(Decimal("-1.432"), +Decimal("-1.432"));
	Decimal d0("234");
	CHECK_EQUAL(d0, +d0);
}

TEST(decimal_value_preservation)
{
	double total = 0;
	for (int i = 0; i != 10; ++i)
	{
		total += double(1) / double(10);
	}
	assert (total != double(1));

	Decimal totaldecs("0");
	for (int i = 0; i != 10; ++i)
	{
		totaldecs += Decimal("1.0") / Decimal("10");
	}
	CHECK_EQUAL(totaldecs, Decimal("1"));
}

TEST(round_decimal)
{
	Decimal d0("7.890");
	Decimal d1("100");
	Decimal d2("-1234.987");
	Decimal d3("24.09999");
	Decimal d4("-42.030001");
	Decimal d0a = d0;
	CHECK_EQUAL(round(d0, 2), d0);
	CHECK_EQUAL(round(d0, 4), d0);
	CHECK_EQUAL(round(d0, 1), Decimal("7.9"));
	CHECK_EQUAL(round(d0, 0), Decimal("8"));
	CHECK_EQUAL(d0, d0a);
	CHECK_EQUAL(round(d1, 2), d1);
	CHECK_EQUAL(round(d2, 2), Decimal("-1234.99"));
	CHECK_EQUAL(round(d3, 1), Decimal("24.1"));
	CHECK_EQUAL(round(d3, 3), Decimal("24.1"));
	CHECK_EQUAL(round(d4, 2), Decimal("-42.03"));
}


TEST(decimal_operations_in_combination)
{
	Decimal d0("0");
	Decimal d23("23");
	Decimal d_p34("-.34");
	Decimal d_23p09("-23.09");
	CHECK_EQUAL(d0 * d23 + d_p34, d_p34);
	CHECK_EQUAL(d0 * (d23 + d_p34), d0);
	CHECK_EQUAL(-d23 - d_23p09 * d23, Decimal("508.07"));
}

TEST(decimal_maximum)
{
	Decimal m = Decimal::maximum();
	--m;
	++m;
	CHECK_THROW(++m, UnsafeArithmeticException);
}

TEST(decimal_minimum)
{
	Decimal m = Decimal::minimum();
	++m;
	--m;
	CHECK_THROW(--m, UnsafeArithmeticException);
}

TEST(decimal_maximum_precision)
{
	int m = Decimal::maximum_precision();
	string s = "1";
	for (int i = 1; i != m; ++i)
	{
		s += "0";
	}
	assert (s.size() == static_cast<string::size_type>(m));
	Decimal d(s);
	s += "0";
	CHECK_THROW(Decimal e(s), UnsafeArithmeticException);
}

