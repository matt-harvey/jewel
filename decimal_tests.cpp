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
	places_type us0 = 4;
	Decimal d0(i0, us0);
	CHECK_EQUAL(d0, Decimal("0.0394"));
	Decimal::int_type i1 = -234234;
	places_type us1 = 1;
	Decimal d1(i1, us1);
	CHECK_EQUAL(d1, Decimal("-23423.4"));
	Decimal::int_type i2 = -23;
	places_type us2 = 10;
	CHECK_THROW(Decimal d2(i2, us2), UnsafeArithmeticException);
	Decimal::int_type i3(-23);
	places_type us3 = 9;
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
	Decimal d200("23042.12");
	Decimal d201("0.88");
	d200 += d201;
	CHECK_EQUAL(d200, Decimal("23043"));
	ostringstream oss200;
	oss200 << d200;
	CHECK_EQUAL(oss200.str(), "23043.00");

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
	Decimal d5 = Decimal("123.00900") + Decimal("0.001");
	CHECK_EQUAL(d5, Decimal("123.01"));
	ostringstream oss0;
	oss0 << d5;
	CHECK_EQUAL(oss0.str(), "123.01000");
	CHECK(oss0.str() != "123.01");
	
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
	Decimal d200("988.00");
	d200 -= Decimal("0.000");
	CHECK_EQUAL(d200, Decimal("988.00"));
	ostringstream oss0;
	oss0 << d200;
	CHECK_EQUAL(oss0.str(), "988.000");
	CHECK(oss0.str() != "988.00");
	Decimal d201("2.05");
	d201 -= Decimal("0.05");
	ostringstream oss1;
	oss1 << d201;
	CHECK_EQUAL(oss1.str(), "2.00");
	
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
	Decimal d200;
	d200 = Decimal("8123.120") - Decimal("23.0100");
	CHECK_EQUAL(d200, Decimal("8100.11"));
	ostringstream oss200;
	oss200 << d200;
	CHECK_EQUAL(oss200.str(), "8100.1100");
	CHECK(oss200.str() != "8100.11");
	
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

	// The smallest possible Decimal cannot be multiplied
	Decimal d200 = Decimal::minimum();
	CHECK_THROW(d200 *= Decimal("1"), UnsafeArithmeticException);

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
	CHECK_EQUAL(Decimal("1000001") * Decimal("0.00222"),
	  Decimal("2220.00222"));
	Decimal d100 = Decimal("-90.9087176") * Decimal("0.00020042");
	CHECK(d100 < Decimal("-0.01821992"));
	CHECK(d100 > Decimal("-0.01821993"));
	Decimal d101 = Decimal("192384.43") * Decimal("-1.9962");
	CHECK(d101 < Decimal("-384037.7"));
	CHECK(d101 > Decimal("-384037.8"));

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
	
	// The smallest possible Decimal cannot be multiplied
	Decimal d10 = Decimal::minimum();
	CHECK_THROW(Decimal d11 = d10 * Decimal("1"), UnsafeArithmeticException);
	CHECK_THROW(Decimal d12 = Decimal("1") * d10, UnsafeArithmeticException);
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

	// Check rounding behaviour
	Decimal d500("2");
	d500 /= Decimal("3");
	CHECK(d500 >= Decimal("0.666"));
	CHECK(d500 <= Decimal("0.667"));
	ostringstream oss;
	oss << d500;
	string str = oss.str();
	CHECK_EQUAL(str[str.size() - 1], '7');
	Decimal d501("-7");
	d501 /= Decimal("9");
	CHECK(d501 <= Decimal("-0.777"));
	CHECK(d501 >= Decimal("-0.778"));
	ostringstream oss1;
	oss1 << d501;
	string str1 = oss1.str();
	CHECK_EQUAL(str1[str1.size() - 1], '8');
	Decimal d502("-1");
	d502 /= Decimal("-3");
	CHECK(d502 >= Decimal("0.333"));
	CHECK(d502 <= Decimal("0.334"));
	ostringstream oss2;
	oss2 << d502;
	string str2 = oss2.str();
	CHECK_EQUAL(str2[str2.size() - 1], '3');
	Decimal d104("-197787.987");
	d104 /= Decimal(".9879");
	CHECK(d104 < Decimal("-200210"));
	CHECK(d104 > Decimal("-200211"));

	// Check behaviour with unsafe operations
	
	// With straightforward overflow
	Decimal d105("1000000");
	Decimal d106("0.000001");
	CHECK_THROW(d105 /= d106, UnsafeArithmeticException);
	Decimal d107("-1000000");
	Decimal d108("0.000001");
	CHECK_THROW(d107 /= d108, UnsafeArithmeticException);

	// With division by zero
	Decimal d300;
	CHECK_THROW(d300 /= Decimal("0"), UnsafeArithmeticException);
	CHECK_THROW(d300 /= Decimal(".0"), UnsafeArithmeticException);
	CHECK_THROW(d300 /= Decimal("-0.0"), UnsafeArithmeticException);
}

TEST(decimal_division)
{
	Decimal d0("0.07");
	Decimal d1("0.02");
	d0 = d0 / d1;
	CHECK_EQUAL(d0, Decimal("3.5"));
	CHECK(Decimal("4.863") / Decimal("95") > Decimal("0.05118"));
	CHECK(Decimal("4.863") / Decimal("95") < Decimal("0.05119"));
	CHECK(Decimal("3.24") / Decimal("11442300.98") > Decimal("0.000000283"));
	CHECK(Decimal("3.24") / Decimal("11442300.98") < Decimal("0.000000284"));
	CHECK(Decimal("-43.434234") / Decimal("-4234234") > Decimal("0.00001"));
	CHECK(Decimal("-43.434234") / Decimal("-4234234") < Decimal("0.00002"));
	CHECK(Decimal("-197787.987") / Decimal(".9879") < Decimal("-200210.5"));
	CHECK(Decimal("-197787.987") / Decimal(".9879") > Decimal("-200210.6"));
	Decimal d900 = Decimal("-2.098987") / Decimal("-10.56983205");
	CHECK(d900 < Decimal("0.198583"));
	CHECK(d900 > Decimal("0.198582"));
	Decimal d901 = Decimal("1.098986778") / Decimal("-1.156983333");
	CHECK(d901 > Decimal("-0.9498727"));
	CHECK(d901 < Decimal("-0.9498726"));
	Decimal d902 = Decimal("3879989") / Decimal("1000000023");
	CHECK(d902 < Decimal("0.00387999"));
	CHECK(d902 > Decimal("0.00387998"));

	// Check value preservation
	Decimal d300("1");
	Decimal const d300a = d300;
	d300 = d300 / Decimal("1000");
	d300 *= Decimal("1000");
	CHECK_EQUAL(d300, d300a);
	Decimal d301("-10000");
	Decimal d301b = Decimal("1") / d301;
	d301b *= d301;
	CHECK_EQUAL(d301b, Decimal("1"));

	// Check behaviour with unsafe operations
	CHECK_THROW(Decimal d2000 = Decimal("10079297") / Decimal("0.000023"),
	  UnsafeArithmeticException);
	CHECK_THROW(Decimal d2001 = Decimal("-1901") / Decimal("0"),
	  UnsafeArithmeticException);
}

TEST(decimal_increment)
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
	Decimal d10 = Decimal::maximum();
	CHECK_THROW(++d10, UnsafeArithmeticException);
	CHECK_EQUAL(d10, Decimal::maximum());
	CHECK_THROW(d10++, UnsafeArithmeticException);
	CHECK_EQUAL(d10, Decimal::maximum());
	// This should be safe
	d10--;
	Decimal d11 = ++d10;
	// Then this should throw
	CHECK_THROW(d11++, UnsafeArithmeticException);
}

TEST(decimal_decrement)
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
	Decimal d3 = --d2;
	CHECK_EQUAL(d3, Decimal("7894"));
	CHECK_EQUAL(d2, Decimal("7894"));
	Decimal d4 = d3--;
	CHECK_EQUAL(d3, Decimal("7893"));
	CHECK_EQUAL(d4, Decimal("7894"));

	// Now test behaviour with unsafe operations.
	Decimal d10 = -Decimal::maximum();
	// This should be safe
	--d10;
	// This should throw
	CHECK_THROW(--d10, UnsafeArithmeticException);
	CHECK_EQUAL(d10, Decimal::minimum());
	// This should be safe
	++d10;
	CHECK_EQUAL(d10, Decimal::minimum() + Decimal("1"));
	d10--;
	// This should throw
	CHECK_THROW(d10--, UnsafeArithmeticException);
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
	ostringstream os1b;
	os0 << Decimal("3.0001") + Decimal("0.000000");
	os1 << Decimal("3.000100");
	os1b << "3.000100";
	CHECK_EQUAL(os0.str(), os1.str());
	CHECK_EQUAL(os1b.str(), os1.str());
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
	os9 << "-3001.09";
	os10 << Decimal("-3001.09");
	CHECK_EQUAL(os9.str(), os10.str());
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
	CHECK_EQUAL(round(Decimal("234.2"), 0), Decimal("234"));
	CHECK_EQUAL(round(Decimal("23.0199"), 2), Decimal("23.02"));
	CHECK_EQUAL(round(Decimal("-2.25"), 1), Decimal("-2.3"));
	CHECK_EQUAL(round(Decimal("2.25"), 1), Decimal("2.3"));
	CHECK_EQUAL(round(Decimal("89.9999"), 0), Decimal("90.000"));
	Decimal d5("13.49");
	Decimal d6 = round(d5, 5);
	ostringstream oss;
	oss << d6;
	CHECK_EQUAL(oss.str(), "13.49000");
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

