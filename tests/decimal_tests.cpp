// Copyright (c) 2013, Matthew Harvey. All rights reserved.

// TODO HIGH PRIORITY The locale-specific tests in here are
// not portable.

#include "../decimal.hpp"
#include "../decimal_exceptions.hpp"
#include "../num_digits.hpp"
#include "decimal_tests_weird_punct.hpp"

#include <limits>
#include <ios>
#include <iostream>
#include <locale>
#include <sstream>
#include <stdexcept>
#include <string>
#include <boost/lexical_cast.hpp>
#include <boost/shared_ptr.hpp>
#include <UnitTest++/UnitTest++.h>

using boost::lexical_cast;
using boost::shared_ptr;
using jewel::Decimal;
using jewel::NumDigits;
using jewel::DecimalException;
using jewel::DecimalRangeException;
using jewel::DecimalAdditionException;
using jewel::DecimalSubtractionException;
using jewel::DecimalMultiplicationException;
using jewel::DecimalDivisionException;
using jewel::DecimalDivisionByZeroException;
using jewel::DecimalIncrementationException;
using jewel::DecimalDecrementationException;
using jewel::DecimalUnaryMinusException;
using jewel::DecimalFromStringException;
using jewel::DecimalStreamReadException;
using jewel::round;
using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::istringstream;
using std::locale;
using std::ostringstream;
using std::numeric_limits;
using std::numpunct;
using std::ostringstream;
using std::string;
using std::use_facet;
using std::wostringstream;
using std::wstring;
using weird_punct::WeirdPunct;



#ifndef JEWEL_DECIMAL_OUTPUT_FAILURE_TEST
// If the output failure test has been enabled, it is simpler just to
// exclude all tests for Decimal other than it (see below).


// @cond (to hide this from Doxygen-generated docs)

// We shouldn't rely on Decimal::int_type being a particular width.
// But we want to test the behaviour of Decimals at the edges
// of that width. To achieve this, let's create some strings, which we 
// know to have certain characteristics in relation to the size of
// Decimal::int_type, and then use these strings to construct Decimals
// to use in this testing.
// We'll do that in this test fixture.
struct DigitStringFixture
{
	// setup
	DigitStringFixture();

	// teardown
	~DigitStringFixture()
	{
	}

	// data members - strings to be passed to Decimal constructor in
	// various tests
	
	// String of "randomish" digits one less than the max safe size
	// for constructing a Decimal
	string s_max_digits_less_one;
	string s_neg_max_digits_less_one;  // with a negative sign

	// A string of the form "1000..." that's the maximum safe size
	// for constructing a Decimal
	string s_max_digits_one_and_zeroes;
	string s_neg_max_digits_one_and_zeroes;  // negative

	// A string of randomish digits that's one more than the maximum
	// safe size
	string s_max_digits_plus_one;
	string s_neg_max_digits_plus_one;

	wstring ws_max_digits_plus_one;

	// A string version of the maximum value of Decimal::int_type
	string s_max_int_type;
	string s_neg_max_int_type;  // Negative of same

	// A string version of the minimum value of Decimal::int_type
	string s_min_int_type;

	// The following are all versions of the above strings, but with
	// a decimal point two spaces from the end.
	string s_max_digits_less_one_places_2;
	string s_neg_max_digits_less_one_places_2;
	string s_max_digits_one_and_zeroes_places_2;
	string s_neg_max_digits_one_and_zeroes_places_2;
	string s_max_digits_plus_one_places_2;
	string s_neg_max_digits_plus_one_places_2;
	string s_max_int_type_places_2;
	string s_neg_max_int_type_places_2;
	string s_min_int_type_places_2;
};
// @endcond


// To add a "randomish" digit to a string of digits
void add_digit(string& s)
{
	string digit_source = "3422988137";
	JEWEL_ASSERT (digit_source.size() == 10);
	s += digit_source[s.size() % 10];
	return;
}

// To insert character c at n places from the end of a string
void insert_from_end(char c, string& s, string::size_type n)
{
	string::iterator it = s.end();
	it -= n;
	s.insert(it, c);
	return;
}

DigitStringFixture::DigitStringFixture()
{
	Decimal::int_type const maxi = numeric_limits<Decimal::int_type>::max();
	Decimal::int_type const mini = numeric_limits<Decimal::int_type>::min();
	Decimal::places_type const max_digits = NumDigits::num_digits(mini);
	for (Decimal::places_type i = 0; i != max_digits - 1; ++i)
	{
		add_digit(s_max_digits_less_one);
	}
	JEWEL_ASSERT (s_max_digits_less_one.size() ==
	  static_cast<string::size_type>(max_digits - 1));
	s_neg_max_digits_less_one = "-" + s_max_digits_less_one;
	JEWEL_ASSERT (s_neg_max_digits_less_one.size() == max_digits);
	JEWEL_ASSERT (s_neg_max_digits_less_one[0] == '-');
	s_max_digits_one_and_zeroes = "1";
	for (Decimal::places_type i = 1; i != max_digits; ++i)
	{
		s_max_digits_one_and_zeroes += '0';
	}
	JEWEL_ASSERT (s_max_digits_one_and_zeroes.size() == max_digits);
	JEWEL_ASSERT (s_max_digits_one_and_zeroes[0] == '1');
	JEWEL_ASSERT (s_max_digits_one_and_zeroes[max_digits - 1] == '0');
	s_neg_max_digits_one_and_zeroes = "-" + s_max_digits_one_and_zeroes;
	JEWEL_ASSERT (s_neg_max_digits_one_and_zeroes.size() ==
	  static_cast<string::size_type>(max_digits + 1));
	s_max_digits_plus_one = s_max_digits_less_one;
	add_digit(s_max_digits_plus_one);
	add_digit(s_max_digits_plus_one);
	JEWEL_ASSERT (s_max_digits_plus_one.size() ==
	  static_cast<string::size_type>(max_digits + 1));
	s_neg_max_digits_plus_one = "-" + s_max_digits_plus_one;
	ostringstream ossmax;
	ossmax << maxi;
	s_max_int_type = ossmax.str();
	s_neg_max_int_type = "-" + s_max_int_type;
	ostringstream ossmin;
	ossmin << mini;
	s_min_int_type = ossmin.str();
	JEWEL_ASSERT
	(	s_min_int_type.size() ==
		static_cast<string::size_type>(max_digits + 1)
	);
	JEWEL_ASSERT (s_min_int_type[0] == '-');

	s_max_digits_less_one_places_2 = s_max_digits_less_one;
	insert_from_end('.', s_max_digits_less_one_places_2, 2);
	JEWEL_ASSERT
	(	s_max_digits_less_one_places_2.size() ==
		s_max_digits_less_one.size() + 1
	);
	s_neg_max_digits_less_one_places_2 = "-" + s_max_digits_less_one_places_2;
	s_max_digits_one_and_zeroes_places_2 = s_max_digits_one_and_zeroes;
	insert_from_end('.', s_max_digits_one_and_zeroes_places_2, 2);
	s_neg_max_digits_one_and_zeroes_places_2 =
		"-" + s_max_digits_one_and_zeroes_places_2;
	s_max_digits_plus_one_places_2 = s_max_digits_plus_one;
	insert_from_end('.', s_max_digits_plus_one_places_2, 2);
	s_neg_max_digits_plus_one_places_2 = "-" + s_max_digits_plus_one_places_2;
	s_max_int_type_places_2 = s_max_int_type;
	insert_from_end('.', s_max_int_type_places_2, 2);
	s_neg_max_int_type_places_2 = "-" + s_max_int_type_places_2;
	s_min_int_type_places_2 = s_min_int_type;
	insert_from_end('.', s_min_int_type_places_2, 2);
}
		

TEST(decimal_parameterless_constructor)
{
	Decimal d0;
	CHECK_EQUAL(d0, Decimal("0"));
	CHECK_EQUAL(d0, Decimal("-0"));
	CHECK_EQUAL(d0 + d0, d0);
	CHECK_EQUAL(d0 - d0, d0);
}


TEST(decimal_direct_constructor)
{
	Decimal d0(0, 1);
	CHECK_EQUAL(d0.places(), 1);
	CHECK_EQUAL(d0.intval(), 0);
	Decimal d1(-123074, 3);
	CHECK_EQUAL(d1, Decimal("-123.074"));
	CHECK_THROW
	(	Decimal(1, Decimal::maximum_precision() + 1),
		DecimalRangeException
	);
	Decimal d2(9000089, Decimal::maximum_precision());
	CHECK_EQUAL(d2.intval(), 9000089);
	CHECK_EQUAL(d2.places(), Decimal::maximum_precision());
	Decimal d3(-329, Decimal::maximum_precision());
	CHECK(d3 != Decimal(0, 0));
	Decimal d4(1, Decimal::maximum_precision());
	CHECK(d4 != Decimal(0, 0));
	Decimal d5(109, 3);
	try
	{
		d5 = Decimal(3, Decimal::maximum_precision() + 100);
	}
	catch (DecimalRangeException&)
	{
		CHECK_EQUAL(d5, Decimal(109, 3));
	}
	CHECK_EQUAL(d5, Decimal(109, 3));
}


TEST_FIXTURE(DigitStringFixture, decimal_string_constructors)
{
	// Test basic functionality
	Decimal d0("0");
	Decimal d0b;
	Decimal d0c("-0.0");
	string s0("-0.0");
	Decimal d0c_s(s0);
	CHECK_EQUAL(d0, Decimal("0"));
	CHECK_EQUAL(d0b, d0);
	CHECK_EQUAL(d0c, d0);
	CHECK_EQUAL(d0c_s, d0c);
	Decimal d1("-908.234");
	CHECK(d0 != d1);
	string s1("-908.234");
	CHECK_EQUAL(d1, Decimal(s1));	
	CHECK_EQUAL(d1, Decimal("-908.23400"));
	Decimal d0p("+0");
	CHECK_EQUAL(d0p, d0);
	string s1p("+908.234");
	Decimal d1p(s1p.c_str());
	CHECK_EQUAL(d1p, Decimal("908.234"));
	CHECK_EQUAL(d1p, Decimal(908234, 3));
	Decimal d1p_s(s1p);
	CHECK_EQUAL(d1p_s, Decimal(908234, 3));

	// Test behaviour with empty string
	CHECK_THROW(Decimal d10(""), DecimalException);
	CHECK_THROW(Decimal d10(""), DecimalFromStringException);
	string s10("");
	CHECK_THROW(Decimal d10(s10), DecimalException);
	CHECK_THROW(Decimal d10(s10), DecimalFromStringException);
	string s10_s(0, 'c');

	// Test behaviour with very short digitless strings
	CHECK_THROW(Decimal d10b("."), DecimalFromStringException);
	CHECK_THROW(Decimal d10b("."), DecimalException);
	CHECK_THROW(Decimal d10b("-"), DecimalFromStringException);
	CHECK_THROW(Decimal d10b("-"), DecimalException);
	CHECK_THROW(Decimal d10b("+"), DecimalFromStringException);
	CHECK_THROW(Decimal d10b("+"), DecimalFromStringException);
	CHECK_THROW(Decimal d10b("-."), DecimalFromStringException);
	CHECK_THROW(Decimal d10b("-."), DecimalException);
	CHECK_THROW(Decimal d10b("+."), DecimalFromStringException);
	CHECK_THROW(Decimal d10b("+."), DecimalException);
	CHECK_THROW(Decimal d10b(".-"), DecimalFromStringException);
	CHECK_THROW(Decimal d10b(".-"), DecimalException);
	CHECK_THROW(Decimal d10b(".+"), DecimalFromStringException);
	CHECK_THROW(Decimal d10b(".+"), DecimalException);
	CHECK_THROW(Decimal d10b(".."), DecimalFromStringException);
	CHECK_THROW(Decimal d10b("--"), DecimalFromStringException);
	CHECK_THROW(Decimal d10b("++"), DecimalFromStringException);
	CHECK_THROW(Decimal d10b((string("."))), DecimalFromStringException);
	CHECK_THROW(Decimal d10b((string(","))), DecimalFromStringException);
	CHECK_THROW(Decimal d10b((string(","))), DecimalException);
	CHECK_THROW(Decimal d10b((string("-"))), DecimalFromStringException);
	CHECK_THROW(Decimal d10b((string("-"))), DecimalFromStringException);
	CHECK_THROW(Decimal d10b((string("+"))), DecimalFromStringException);
	CHECK_THROW(Decimal d10b((string("+"))), DecimalException);
	CHECK_THROW(Decimal d10b((string("-."))), DecimalFromStringException);
	CHECK_THROW(Decimal d10b((string("-."))), DecimalException);
	CHECK_THROW(Decimal d10b((string("+."))), DecimalFromStringException);
	CHECK_THROW(Decimal d10b((string("+."))), DecimalFromStringException);
	CHECK_THROW(Decimal d10b((string("++"))), DecimalFromStringException);
	CHECK_THROW(Decimal d10b((string("--"))), DecimalFromStringException);

	// Test behaviour with prohibited characters in string
	CHECK_THROW(Decimal d11("9.90b0"), DecimalException);
	CHECK_THROW(Decimal d11("9.90b0"), DecimalFromStringException);
	CHECK_THROW(Decimal d12((string("7,8"))), DecimalFromStringException);
	CHECK_THROW(Decimal d12("6,8"), DecimalException);
	CHECK_THROW(Decimal d13("Llama"), DecimalFromStringException);
	CHECK_THROW(Decimal d13b("9.0.3"), DecimalFromStringException);
	CHECK_THROW(Decimal d13c("-79-"), DecimalFromStringException);
	CHECK_THROW(Decimal d13c("-78-"), DecimalException);
	CHECK_THROW(Decimal d13d("0-0"), DecimalFromStringException);
	CHECK_THROW(Decimal d13e("0+0"), DecimalFromStringException);
	CHECK_THROW(Decimal d13f("+79+"), DecimalFromStringException);
	CHECK_THROW(Decimal d13g("++88"), DecimalFromStringException);
	CHECK_THROW(Decimal d13h((string("+-88"))), DecimalFromStringException);
	CHECK_THROW(Decimal d13i("-+88"), DecimalFromStringException);
	
	// Test behaviour with attempted Decimal having too large a would-be
	// underlying integer.
	CHECK_THROW
	(	Decimal d100(s_max_digits_plus_one),
		DecimalRangeException
	);
	CHECK_THROW
	(	Decimal d100(s_max_digits_plus_one),
		DecimalException
	);
	string even_longer = s_max_digits_plus_one + "00";
	CHECK_THROW(Decimal d101(even_longer), DecimalRangeException);
	CHECK_THROW
	(	Decimal d102(s_neg_max_digits_plus_one), DecimalRangeException
	);
	CHECK_THROW
	(	Decimal d102(s_neg_max_digits_plus_one), DecimalException
	);
	// These should be OK though
	Decimal d103(s_max_digits_one_and_zeroes);
	Decimal d104(s_neg_max_digits_one_and_zeroes);
	Decimal d105(s_max_digits_less_one);
	Decimal d106(s_neg_max_digits_less_one);
	Decimal d107(s_max_int_type);
	Decimal d108(s_min_int_type);
	// These should throw
	CHECK_THROW
	(	Decimal d109(s_max_digits_plus_one_places_2), DecimalRangeException
	);
	CHECK_THROW
	(	Decimal d110(s_neg_max_digits_plus_one_places_2), DecimalException
	);
	CHECK_THROW
	(	Decimal d110(s_neg_max_digits_plus_one_places_2), DecimalRangeException
	);
	// These should be OK
	Decimal d111(s_max_digits_one_and_zeroes_places_2);
	Decimal d112(s_neg_max_digits_one_and_zeroes_places_2);
	Decimal d113(s_max_int_type_places_2);
	Decimal d114(s_neg_max_int_type_places_2);
	Decimal d115(s_min_int_type_places_2);

	// These should't throw
	Decimal d18("1000030");
	Decimal d19("1.000030");
	Decimal d20("-1.000030");
	string s20b("0000001");
	Decimal d20b_s(s20b);
	Decimal d21(".0000430");
	Decimal d22("-.99489");
	Decimal d23(".00000");
	Decimal d23b("+.000000");
	string s23b("+.000000");
	Decimal d23b_s(s23b);

	// Test retention of fractional precision implied by string
	Decimal d24("90.100");
	ostringstream oss0;
	oss0 << d24;
	CHECK_EQUAL(oss0.str(), "90.100");
	Decimal d402(".0");
	ostringstream oss402;
	oss402 << d402;
	CHECK_EQUAL(oss402.str(), "0.0");

	// Test elimination of pointless negative
	string s25("-0.0");
	Decimal d25_s(s25);
	ostringstream oss1;
	oss1 << d25_s;
	CHECK_EQUAL(oss1.str(), "0.0");

	// Test elimination of pointless positive
	Decimal d26("+10987");
	ostringstream oss2;
	oss2 << d26;
	CHECK_EQUAL(oss2.str(), "10987");

}



TEST_FIXTURE(DigitStringFixture, decimal_wstring_constructors)
{
	// Test basic functionality
	Decimal d0(L"0");
	Decimal d0b;
	Decimal d0c(L"-0.0");
	wstring s0(L"-0.0");
	Decimal d0c_s(s0);
	CHECK_EQUAL(d0, Decimal(L"0"));
	CHECK_EQUAL(d0b, d0);
	CHECK_EQUAL(d0c, d0);
	CHECK_EQUAL(d0c_s, d0c);
	Decimal d1(L"-908.234");
	CHECK(d0 != d1);
	wstring s1(L"-908.234");
	CHECK_EQUAL(d1, Decimal(s1));	
	CHECK_EQUAL(d1, Decimal(L"-908.23400"));
	Decimal d0p(L"+0");
	CHECK_EQUAL(d0p, d0);
	wstring s1p(L"+908.234");
	Decimal d1p(s1p.c_str());
	CHECK_EQUAL(d1p, Decimal(L"908.234"));
	CHECK_EQUAL(d1p, Decimal(908234, 3));
	Decimal d1p_s(s1p);
	CHECK_EQUAL(d1p_s, Decimal(908234, 3));

	// Test behaviour with empty wstring
	CHECK_THROW(Decimal d10(L""), DecimalException);
	CHECK_THROW(Decimal d10(L""), DecimalFromStringException);
	wstring s10(L"");
	CHECK_THROW(Decimal d10(s10), DecimalException);
	CHECK_THROW(Decimal d10(s10), DecimalFromStringException);
	wstring s10_s(0, L'c');

	// Test behaviour with very short digitless wstring
	CHECK_THROW(Decimal d10b(L"."), DecimalFromStringException);
	CHECK_THROW(Decimal d10b(L"."), DecimalException);
	CHECK_THROW(Decimal d10b(L"-"), DecimalFromStringException);
	CHECK_THROW(Decimal d10b(L"-"), DecimalException);
	CHECK_THROW(Decimal d10b(L"+"), DecimalFromStringException);
	CHECK_THROW(Decimal d10b(L"+"), DecimalFromStringException);
	CHECK_THROW(Decimal d10b(L"-."), DecimalFromStringException);
	CHECK_THROW(Decimal d10b(L"-."), DecimalException);
	CHECK_THROW(Decimal d10b(L"+."), DecimalFromStringException);
	CHECK_THROW(Decimal d10b(L"+."), DecimalException);
	CHECK_THROW(Decimal d10b(L".-"), DecimalFromStringException);
	CHECK_THROW(Decimal d10b(L".-"), DecimalException);
	CHECK_THROW(Decimal d10b(L".+"), DecimalFromStringException);
	CHECK_THROW(Decimal d10b(L".+"), DecimalException);
	CHECK_THROW(Decimal d10b(L".."), DecimalFromStringException);
	CHECK_THROW(Decimal d10b(L"--"), DecimalFromStringException);
	CHECK_THROW(Decimal d10b(L"++"), DecimalFromStringException);
	CHECK_THROW(Decimal d10b((wstring(L"."))), DecimalFromStringException);
	CHECK_THROW(Decimal d10b((wstring(L","))), DecimalFromStringException);
	CHECK_THROW(Decimal d10b((wstring(L","))), DecimalException);
	CHECK_THROW(Decimal d10b((wstring(L"-"))), DecimalFromStringException);
	CHECK_THROW(Decimal d10b((wstring(L"-"))), DecimalFromStringException);
	CHECK_THROW(Decimal d10b((wstring(L"+"))), DecimalFromStringException);
	CHECK_THROW(Decimal d10b((wstring(L"+"))), DecimalException);
	CHECK_THROW(Decimal d10b((wstring(L"-."))), DecimalFromStringException);
	CHECK_THROW(Decimal d10b((wstring(L"-."))), DecimalException);
	CHECK_THROW(Decimal d10b((wstring(L"+."))), DecimalFromStringException);
	CHECK_THROW(Decimal d10b((wstring(L"+."))), DecimalFromStringException);
	CHECK_THROW(Decimal d10b((wstring(L"++"))), DecimalFromStringException);
	CHECK_THROW(Decimal d10b((wstring(L"--"))), DecimalFromStringException);

	// Test behaviour with prohibited characters in wstring
	CHECK_THROW(Decimal d11(L"9.90b0"), DecimalException);
	CHECK_THROW(Decimal d11(L"9.90b0"), DecimalFromStringException);
	CHECK_THROW(Decimal d12((wstring(L"7,8"))), DecimalFromStringException);
	CHECK_THROW(Decimal d12(L"6,8"), DecimalException);
	CHECK_THROW(Decimal d13(L"Llama"), DecimalFromStringException);
	CHECK_THROW(Decimal d13b(L"9.0.3"), DecimalFromStringException);
	CHECK_THROW(Decimal d13c(L"-79-"), DecimalFromStringException);
	CHECK_THROW(Decimal d13c(L"-78-"), DecimalException);
	CHECK_THROW(Decimal d13d(L"0-0"), DecimalFromStringException);
	CHECK_THROW(Decimal d13e(L"0+0"), DecimalFromStringException);
	CHECK_THROW(Decimal d13f(L"+79+"), DecimalFromStringException);
	CHECK_THROW(Decimal d13g(L"++88"), DecimalFromStringException);
	CHECK_THROW(Decimal d13h((wstring(L"+-88"))), DecimalFromStringException);
	CHECK_THROW(Decimal d13i(L"-+88"), DecimalFromStringException);
	
	// These should't throw
	Decimal d18(L"1000030");
	Decimal d19(L"1.000030");
	Decimal d20(L"-1.000030");
	wstring s20b(L"0000001");
	Decimal d20b_s(s20b);
	Decimal d21(L".0000430");
	Decimal d22(L"-.99489");
	Decimal d23(L".00000");
	Decimal d23b(L"+.000000");
	wstring s23b(L"+.000000");
	Decimal d23b_s(s23b);

	// Note UnitTest++ macro "CHECK_EQUAL" fell apart in the below here. Hence
	// just using "==" here.

	// Test retention of fractional precision implied by wstring
	Decimal d24(L"90.100");
	wostringstream oss0;
	oss0 << d24;
	CHECK(oss0.str() == L"90.100");
	Decimal d402(L".0");
	wostringstream oss402;
	oss402 << d402;
	CHECK(oss402.str() == L"0.0");

	// Test elimination of pointless negative
	wstring s25(L"-0.0");
	Decimal d25_s(s25);
	wostringstream oss1;
	oss1 << d25_s;
	CHECK(oss1.str() == L"0.0");

	// Test elimination of pointless positive
	Decimal d26(L"+10987");
	wostringstream oss2;
	oss2 << d26;
	CHECK(oss2.str() == L"10987");


}


TEST(decimal_string_and_wstring_constructed_equivalence)
{
	Decimal d0(L"-609078.9870");
	Decimal d1("-609078.9870");
	string s2("-609078.9870");
	wstring s3(L"-609078.9870");
	Decimal d2(s2);
	Decimal d3(s3);
	CHECK_EQUAL(d0, d1);
	CHECK_EQUAL(d2, d3);
	CHECK_EQUAL(d3, d0);
}


TEST(decimal_assignment)
{
	Decimal d0("3074.2340");
	Decimal d1("34.234");
	d1 = d0;
	CHECK_EQUAL(d0, d1);
	CHECK_EQUAL(d1, Decimal("3074.2340"));
	CHECK(d0 != Decimal("34.234"));
	Decimal d2("-23.00");
	Decimal d3("0.1");
	d3 = d2;
	CHECK_EQUAL(d3, d2);
	CHECK_EQUAL(d3, Decimal("-23"));
	// Test retention of fractional precision
	ostringstream oss;
	oss << d3;
	CHECK_EQUAL(oss.str(), "-23.00");
}

TEST_FIXTURE(DigitStringFixture, decimal_plus_equals)
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
	Decimal d200("+23042.12");
	Decimal d201("0.88");
	d200 += d201;
	CHECK_EQUAL(d200, Decimal("23043"));
	ostringstream oss200;
	oss200 << d200;
	CHECK_EQUAL(oss200.str(), "23043.00");

	// Check behaviour for unsafe operations

	// With precision loss
	string s10(Decimal::maximum_precision() * 2 / 3, '0');
	string s11 = s10;
	s10[0] = '1';
	s11 = "0." + s11;
	s11[s11.size() - 1] = '1';
	Decimal d10(s10);
	Decimal d11(s11);
	CHECK_THROW(d10 += d11, DecimalRangeException);
	CHECK_THROW(d10 += d11, DecimalException);
	Decimal d12(s_max_digits_less_one);
	CHECK_THROW(d12 += Decimal("0.001"), DecimalRangeException);
	Decimal d13("-.900009");
	Decimal d13b(s_max_digits_less_one_places_2);
	CHECK_THROW(d13 += d13b, DecimalRangeException);
	// Check value unchanged after exception thrown
	CHECK_EQUAL(d13, Decimal("-.900009"));
	// Check again using standard try/catch
	try
	{
		d13 += d13b;
	}
	catch (DecimalRangeException&)
	{
		CHECK_EQUAL(d13, Decimal("-.900009"));
	}
	CHECK_EQUAL(d13, Decimal("-.900009"));
	Decimal d14("-0." + s_max_digits_less_one);
	CHECK_THROW(d14 += Decimal("29"), DecimalException);
	
	// With straightforward overflow
	Decimal d15 = Decimal::maximum() - Decimal("100");
	Decimal const d15_orig = d15;
	Decimal d15b("101");
	CHECK_THROW(d15 += d15b, DecimalAdditionException);
	// Check original value unchanged after exception thrown
	CHECK_EQUAL(d15, d15_orig);
	try
	{
		d15 += d15b;
	}
	catch (DecimalAdditionException&)
	{
		CHECK_EQUAL(d15, d15_orig);
	}
	CHECK_EQUAL(d15, d15_orig);
	ostringstream oss;
	oss << numeric_limits<Decimal::int_type>::max() / 11;
	Decimal large_num(oss.str());
	Decimal large_neg_num = -large_num;
	Decimal large_num_b = large_num;
	Decimal large_neg_num_b = large_neg_num;
	CHECK_THROW
	(	for (int i = 0; i != 12; ++i) large_num += large_num,
		DecimalAdditionException
	);
	CHECK_THROW
	(	for (int i = 0; i != 12; ++i) large_neg_num += large_neg_num,
		DecimalAdditionException
	);
	// But these shouldn't throw
	Decimal d17(".9324");
	d17 += Decimal("24.24");
	Decimal d18("1000000");
	d18 += Decimal("9284792");
	Decimal d19("+.0242234");
	d19 += Decimal("19.12");
	Decimal d20("100000");
	d20 += Decimal(".001");
	large_num_b += Decimal("10");
	large_neg_num_b += large_num_b;
	large_num_b += (-large_num_b);

	// Test retention of fractional precision
	Decimal d21("0.99");
	Decimal d22("+0.01");
	ostringstream oss22;
	d21 += d22;
	oss22 << d21;
	CHECK_EQUAL(oss22.str(), "1.00");
	CHECK(oss22.str() != "1");
	Decimal d23("-2340.1230");
	Decimal d24("2340.1230");
	d23 += d24;
	ostringstream oss24;
	oss24 << d23;
	CHECK_EQUAL(oss24.str(), "0.0000");
}

TEST_FIXTURE(DigitStringFixture, decimal_addition)
{
	Decimal d0("-.30010");
	Decimal d1("-400.678");
	Decimal d2 = d0 + d1;
	CHECK_EQUAL(d2, Decimal("-400.97810"));
	CHECK_EQUAL(d2, Decimal("-400.9781"));
	Decimal d5 = Decimal("123.00900") + Decimal("0.001");
	CHECK_EQUAL(d5, Decimal("123.01"));
	ostringstream oss0;
	oss0 << d5;
	CHECK_EQUAL(oss0.str(), "123.01000");
	CHECK(oss0.str() != "123.01");
	CHECK_EQUAL(Decimal("23.91001") + Decimal("-.29781"), Decimal("23.6122"));
	CHECK_EQUAL
	(	Decimal("10604.9") + Decimal("917.1909"),
		Decimal("11522.0909")
	);
	
	// Check behaviour for unsafe operations

	// With precision loss
	string s10(Decimal::maximum_precision() * 2 / 3, '0');
	string s11 = s10;
	s10[0] = '1';
	s11 = "0." + s11;
	s11[s11.size() - 1] = '1';
	Decimal d10(s10);
	Decimal d11(s11);
	Decimal d10p11;
	CHECK_THROW(d10p11 = d10 + d11, DecimalRangeException);
	Decimal d12(s_max_digits_less_one);
	Decimal d12p("12.3");
	CHECK_THROW(d12p = d12 + Decimal("0.001"), DecimalRangeException);
	Decimal d13("-.129774");
	Decimal d13b(s_max_digits_less_one_places_2);
	Decimal d13p;
	CHECK_THROW(d13p = d13 + d13b, DecimalRangeException);
	Decimal d14("-0." + s_max_digits_less_one);
	Decimal d14p;
	CHECK_THROW(d14p = d14 + Decimal("29"), DecimalException);
	CHECK_THROW(d14p = d14 + Decimal("29"), DecimalRangeException);
	
	// With straightforward overflow
	ostringstream oss;
	oss << numeric_limits<Decimal::int_type>::max() / 11;
	Decimal large_num(oss.str());
	Decimal large_neg_num = -large_num;
	Decimal large_num_b = large_num;
	Decimal large_neg_num_b = large_neg_num;
	CHECK_THROW
	(	for (int i = 0; i != 12; ++i) large_num = large_num + large_num,
		DecimalAdditionException
	);
	CHECK_THROW
	(	for (int i = 0; i != 12; ++i) large_neg_num =
			large_neg_num + large_neg_num,
		DecimalAdditionException
	);
	// But these shouldn't throw
	Decimal d17(".9324");
	d17 += Decimal("24.24");
	Decimal d18("1000000");
	Decimal d18b;
	d18b = d18 + Decimal("9284792");
	Decimal d19(".0242234");
	Decimal d19b;
	d19b = d19 + Decimal("19.12");
	Decimal d20("100000");
	d20 = d20 + Decimal(".001");
	large_num_b = large_num_b + Decimal("10");
	Decimal large_neg_num_c;
	large_neg_num_c = large_neg_num_b + large_num_b;
	large_num_b = large_num_b + (-large_num_b);

	// Test retention of fractional precision
	ostringstream oss21;
	oss21 << Decimal("-3.5") + Decimal("-4.5");
	CHECK_EQUAL(oss21.str(), "-8.0");
}

TEST_FIXTURE(DigitStringFixture, decimal_minus_equals)
{
	Decimal d0("30.86");
	Decimal d1("20000.9");
	Decimal d2("-19970.04");
	d0 -= d1;
	CHECK_EQUAL(d0, d2);
	CHECK_EQUAL(Decimal("0.23420") - Decimal("0.03"), Decimal("0.2042"));
	CHECK_EQUAL(Decimal("100") - Decimal("99"), Decimal("1.00"));
	CHECK_EQUAL
	(	Decimal("4231.123") - Decimal("-0"),
		Decimal("4231.123")
	);
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
	string s10 = s_max_digits_one_and_zeroes;
	s10.resize(s10.size() - 3);
	string s11 = "0.00001";
	Decimal d10(s10);
	Decimal d11(s11);
	CHECK_THROW(d10 -= d11, DecimalException);
	CHECK_THROW(d10 -= d11, DecimalRangeException);
	// Check value unchanged after exception
	CHECK_EQUAL(d10, Decimal(s10));
	// Check again using standard try/catch
	try
	{
		d10 -= d11;
	}
	catch (DecimalRangeException&)
	{
		CHECK_EQUAL(d10, Decimal(s10));
	}
	CHECK_EQUAL(Decimal(s10), d10);
	Decimal d12("7927439");
	Decimal d12b("-." + string(Decimal::maximum_precision() - 4, '0') + "1");
	CHECK_THROW(d12 -= d12b, DecimalRangeException);
	Decimal d13("1.90009");
	Decimal s13b(s_max_digits_one_and_zeroes_places_2);
	CHECK_THROW(d13 -= s13b, DecimalRangeException);
	// With straightforward overflow
	ostringstream oss;
	oss << numeric_limits<Decimal::int_type>::max() / 11;
	Decimal large_num(oss.str());
	Decimal large_neg_num = -large_num;
	Decimal large_num_b = large_num;
	Decimal large_neg_num_b = large_neg_num;
	// Check value unchanged after exception
	Decimal d14 = Decimal::minimum() + Decimal("999");
	Decimal const d14_orig = d14;
	CHECK_THROW(d14 -= Decimal("1000"), DecimalSubtractionException);
	CHECK_EQUAL(d14, d14_orig);
	try
	{
		d14 -= Decimal("1000");
	}
	catch (DecimalSubtractionException&)
	{
		CHECK_EQUAL(d14, d14_orig);
	}
	CHECK_EQUAL(d14, d14_orig);
	CHECK_THROW
	(	for (int i = 0; i != 12; ++i) large_neg_num -= large_num,
		DecimalSubtractionException
	);
	CHECK_THROW
	(	for (int i = 0; i != 12; ++i) large_neg_num -= large_num,
		DecimalException
	);
	CHECK_THROW
	(	for (int i = 0; i != 12; ++i) large_num -= large_neg_num,
		DecimalSubtractionException
	);
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

	// Test retention of fractional precision
	Decimal d21("100");
	Decimal d22("0.00000");
	d21 -= d22;
	ostringstream oss22;
	oss22 << d21;
	CHECK_EQUAL(oss22.str(), "100.00000");
}


TEST_FIXTURE(DigitStringFixture, decimal_subtraction)
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
	string s10 = s_max_digits_one_and_zeroes;
	s10.resize(s10.size() - 3);
	string s11 = "0.00001";
	Decimal d10(s10);
	Decimal d11(s11);
	Decimal d11b;
	CHECK_THROW(d11b = d10 - d11, DecimalRangeException);
	Decimal d12("7927439");
	Decimal d12b("-." + string(Decimal::maximum_precision() - 4, '0') + "1");
	Decimal d12c;
	CHECK_THROW(d12c = d12 - d12b, DecimalRangeException);
	Decimal d13("1.900009");
	Decimal d13b(s_max_digits_one_and_zeroes_places_2);
	Decimal d13c;
	CHECK_THROW(d13c = d13 - d13b, DecimalException);
	Decimal d14("09.009423");
	CHECK_THROW
	(	d14 = d14 - Decimal(s_max_digits_less_one),
		DecimalRangeException
	);
	// Check value unchanged after exception thrown
	CHECK_EQUAL(d14, Decimal("09.009423"));
	// Check again using normal try/catch
	try
	{
		d14 = d14 - Decimal(s_max_digits_less_one);
	}
	catch (DecimalRangeException&)
	{
		CHECK_EQUAL(d14, Decimal("09.009423"));
	}

	// With straightforward overflow
	ostringstream oss;
	oss << numeric_limits<Decimal::int_type>::max() / 11;
	Decimal large_num(oss.str());
	Decimal large_neg_num = -large_num;
	Decimal large_num_b = large_num;
	Decimal large_neg_num_b = large_neg_num;
	CHECK_THROW
	(	for (int i = 0; i != 12; ++i) large_neg_num =
	  		large_neg_num - large_num,
		DecimalSubtractionException
	);
	CHECK_THROW
	(	for (int i = 0; i != 12; ++i) large_num =
			large_num - large_neg_num,
		DecimalSubtractionException
	);
	CHECK_THROW
	(	Decimal("0") - Decimal::minimum(),
		DecimalSubtractionException
	);
	// But these shouldn't throw
	Decimal d16b;
	d16b = Decimal("0") - Decimal::maximum();
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

	// Test retention of fractional precision
	Decimal d21("329876.0877");
	Decimal d22("0.0877");
	Decimal d23 = d21 - d22;
	ostringstream oss23;
	oss23 << d23;
	CHECK_EQUAL(oss23.str(), "329876.0000");
}

TEST_FIXTURE(DigitStringFixture, decimal_multiply_equals)
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
	Decimal d100 = Decimal("900001") * Decimal("0.2234");
	CHECK(d100 > Decimal("201060.2"));
	CHECK(d100 < Decimal("201060.3"));

	// Test behaviour with unsafe operations
	
	// With straightforward overflow
	ostringstream oss;
	oss << numeric_limits<Decimal::int_type>::max() / 11;
	Decimal large_num(oss.str());
	Decimal large_neg_num = -large_num;
	CHECK_THROW(large_num *= large_num, DecimalMultiplicationException);
	CHECK_THROW(large_num *= large_num, DecimalException);
	CHECK_THROW(large_num *= Decimal("-29"), DecimalMultiplicationException);
	// Check value unchanged after exception
	CHECK_EQUAL(large_num, Decimal(oss.str()));
	// Check again using standard try/catch
	try
	{
		large_num *= Decimal("-29");
	}
	catch (DecimalMultiplicationException&)
	{
		CHECK_EQUAL(large_num, Decimal(oss.str()));
	}
	CHECK_THROW
	(	large_neg_num * large_neg_num,
		DecimalMultiplicationException
	);

	CHECK_THROW
	(	Decimal(numeric_limits<Decimal::int_type>::min(), 0) *=
			Decimal(-1, 0),
		DecimalMultiplicationException
	);

	// With overflow in execution
	Decimal d50("1.1111111");
	string s51 = s_max_digits_one_and_zeroes;
	s51.resize(s51.size() - 3);
	Decimal d51(s51);
	CHECK_THROW(d50 *= d51, DecimalMultiplicationException);
	// Check value unchanged after exception
	CHECK_EQUAL(d50, Decimal("1.1111111"));
	// Check again using standard try/catch
	try
	{
		d50 *= d51;
	}
	catch (DecimalException&)
	{
		CHECK_EQUAL(d50, Decimal("1.1111111"));
	}

	// The smallest possible Decimal cannot be multiplied
	Decimal d200 = Decimal::minimum();
	CHECK_THROW(d200 *= Decimal("1"), DecimalMultiplicationException);
	// Check value unchanged after exception
	CHECK_EQUAL(d200, Decimal::minimum());
	// Check again using standard try/catch, and a different variable
	Decimal d200b = Decimal::minimum();
	try
	{
		d200b *= Decimal("1");
	}
	catch (DecimalException&)
	{
		CHECK_EQUAL(d200b, Decimal::minimum());
	}

	// But these should shouldn't throw
	Decimal d101, d102, d103;
	d101 = large_num * Decimal("0");
	d102 = -large_num * Decimal("1");
	d103 = large_num * Decimal("-1.000");

	// Test elimination of trailing fractional zeroes
	Decimal d104("2.00");
	d104 *= Decimal("1.00");
	ostringstream oss104;
	oss104 << d104;
	CHECK_EQUAL(oss104.str(), "2");
	CHECK(oss104.str() != "2.00");
	Decimal d105("-0.20");
	Decimal d106("-5.0");
	d105 *= d106;
	ostringstream oss105;
	oss105 << d105;
	CHECK_EQUAL(oss105.str(), "1");
}


TEST_FIXTURE(DigitStringFixture, decimal_multiplication)
{
	Decimal d0("-100789.8");
	Decimal d1("-78.9");
	Decimal d2 = d0 * d1;
	Decimal d3 = Decimal("7952315.22");
	CHECK_EQUAL(d2, d3);
	Decimal d6("0.0000001");
	Decimal d7("0.00001");
	Decimal d8 = d6 * d7;
	CHECK(d8 < Decimal("0.0000000001"));
	CHECK_EQUAL
	(	Decimal("1000001") * Decimal("0.00222"),
		Decimal("2220.00222")
	);
	string s100 = "0." + string(Decimal::maximum_precision() - 2, '0') + "1";
	string s101 = "-0." + string(Decimal::maximum_precision() - 3, '0') + "1";
	Decimal d100(s100);
	Decimal d101(s101);
	CHECK_EQUAL(d100 * d101, Decimal("0"));
	// Check rounding occurs
	string s102 = "0." + string(Decimal::maximum_precision() - 1, '0') + "5";
	string s102r = "0." + string(Decimal::maximum_precision() - 1, '0') + "1";
	CHECK_EQUAL(Decimal(s102) * Decimal("0.1"), Decimal(s102r));
	string s103 = "-0." + string(Decimal::maximum_precision() - 1, '0') + "7";
	string s103r =
	  "-0." + string(Decimal::maximum_precision() - 1, '0') + "1";
	CHECK_EQUAL(Decimal(s102) * Decimal("0.1"), Decimal(s102r));
	// or doesn't occur, as appropriate
	string s104 = "0." + string(Decimal::maximum_precision() - 1, '0') + "4";
	string s104r = "0." + string(Decimal::maximum_precision() - 1, '0') + "0";
	CHECK_EQUAL(Decimal(s104) * Decimal("0.1"), Decimal(s104r));

	// Test behaviour with unsafe operations
	
	// With straightforward overflow
	ostringstream oss;
	oss << numeric_limits<Decimal::int_type>::max() / 11;
	Decimal large_num(oss.str());
	Decimal large_neg_num = -large_num;
	Decimal large_num_b;
	CHECK_THROW
	(	large_num_b = large_num * large_num,
		DecimalMultiplicationException
	);
	// Check value unchanged after exception using standard try/catch
	Decimal large_num_b2(oss.str());
	bool exception_has_been_caught = false;
	try
	{
		large_num_b2 = large_num * large_num;
	}
	catch (DecimalException&)
	{
		exception_has_been_caught = true;
	}
	CHECK(exception_has_been_caught);
	CHECK_EQUAL(large_num_b2, Decimal(oss.str()));
	CHECK_EQUAL(large_num, Decimal(oss.str()));
	// End check of value preservation
	Decimal large_num_c;
	CHECK_THROW
	(	large_num_c = Decimal("-29") * large_num,
		DecimalMultiplicationException
	);
	Decimal large_neg_num_b;
	CHECK_THROW
	(	large_neg_num_b = large_neg_num * large_num,
		DecimalMultiplicationException
	);

	// With overflow within execution
	Decimal d50("2.0877879");
	string s51 = s_max_digits_one_and_zeroes;
	s51.resize(s51.size() - 3);
	s51 = "-" + s51;
	Decimal d51(s51);
	Decimal d52 = Decimal("1");
	CHECK_THROW(d52 = d51 * d50, DecimalException);
	CHECK_THROW(d52 = d51 * d50, DecimalMultiplicationException);
	// Check value unchanged after exception
	CHECK_EQUAL(d52, Decimal("1"));
	// Check again using standard try/catch
	try
	{
		d52 = d51 * d50;
	}
	catch (DecimalMultiplicationException&)
	{
	}
	CHECK_EQUAL(d52, Decimal("1"));
	CHECK_EQUAL(d51, Decimal(s51));
	CHECK_EQUAL(d50, Decimal("2.0877879"));

	// The smallest possible Decimal cannot be multiplied
	Decimal d10 = Decimal::minimum();
	Decimal d11;
	CHECK_THROW(d11 = d10 * Decimal("1"), DecimalException);
	Decimal d12;
	CHECK_THROW
	(	d12 = Decimal("1") * d10,
		DecimalMultiplicationException
	);

	// Test elimination of trailing fractional zeroes
	Decimal d700("107.0700");
	d700 /= Decimal("10");
	ostringstream oss700;
	oss700 << d700;
	CHECK_EQUAL(oss700.str(), "10.707");
}


TEST_FIXTURE(DigitStringFixture, decimal_divide_equals)
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
	Decimal d800("1000001");
	d800 /= Decimal("0.001");
	CHECK_EQUAL(d800, Decimal("1000001000"));
	/* Note this next check crashes if Decimal::int_type is only 32 bit
	 * instead of 64 bit.
	 */
	Decimal d801("90.27423");
	d801 /= Decimal("0.00008447");
	CHECK(d801 > Decimal("1068713.50"));
	CHECK(d801 < Decimal("1068713.51"));

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
	string s105 = s_max_digits_one_and_zeroes;
	s105.resize(s105.size() - 5);
	Decimal d105(s105);
	Decimal d106("0.000001");
	CHECK_THROW(d105 /= d106, DecimalException);
	CHECK_THROW(d105 /= d106, DecimalDivisionException);
	// Check value unchanged after exception
	CHECK_EQUAL(d105, Decimal(s105));
	// Check again using standard try/catch and a different variable
	Decimal d105b(s105);
	Decimal const d105c = d105b;
	bool exception_was_caught = false;
	try
	{
		d105b /= d106;
	}
	catch (DecimalDivisionException&)
	{
		exception_was_caught = true;
	}
	CHECK_EQUAL(d105b, d105c);
	CHECK_EQUAL(exception_was_caught, true);
	Decimal d107("-" + s105);
	Decimal d108("0.000001");
	CHECK_THROW(d107 /= d108, DecimalDivisionException);
	// Check value unchanged after exception
	CHECK_EQUAL(d107, Decimal("-" + s105));
	// Check again using standard try/catch
	Decimal const d107c = d107;
	Decimal const d108c = d108;
	try
	{
		d107 /= d108;
	}
	catch (DecimalDivisionException&)
	{
	}
	CHECK_EQUAL(d107, d107c);
	CHECK_EQUAL(d108, d108c);

	// With division by zero
	Decimal d300("24.3");
	CHECK_THROW(d300 /= Decimal("0"), DecimalDivisionByZeroException);
	CHECK_THROW(d300 /= Decimal("0"), DecimalDivisionException);
	CHECK_THROW(d300 /= Decimal("0"), DecimalException);
	CHECK_THROW(d300 /= Decimal(".0"), DecimalDivisionByZeroException);
	CHECK_THROW(d300 /= Decimal("-0.0"), DecimalDivisionByZeroException);
	// Check value unchanged after exception
	CHECK_EQUAL(Decimal("24.3"), d300);

	// Test elimination of trailing fractional zeroes
	ostringstream oss301;
	oss301 << Decimal("234.001100") / Decimal("10");
	CHECK_EQUAL(oss301.str(), "23.40011");

	// Test that exception is thrown when dividend has too many
	// significant digits
	Decimal d400("1");
	Decimal d401("-31.908");
	Decimal d402("-1");
	CHECK_THROW
	(	d400 /= Decimal(s_max_digits_one_and_zeroes),
		DecimalDivisionException
	);
	d400 = Decimal("1");
	CHECK_THROW
	(	d400 /= Decimal::maximum(),
		DecimalException
	);
	d400 = Decimal("1");
	CHECK_THROW
	(	d400 /=
			Decimal(s_max_digits_one_and_zeroes_places_2) +
			Decimal("0.01"),
		DecimalDivisionException
	);
	d400 = Decimal("1");
	CHECK_THROW(d400 /= Decimal(s_neg_max_int_type_places_2),
	  DecimalDivisionException);
	d400 = Decimal("1");
	CHECK_THROW(d401 /= Decimal(s_max_digits_one_and_zeroes),
	  DecimalException);
	d401 = Decimal("-31.908");
	CHECK_THROW(d401 /= Decimal::maximum(), DecimalDivisionException);
	d401 = Decimal("-31.908");
	CHECK_THROW(d401 /= Decimal(s_max_digits_one_and_zeroes_places_2) +
	  Decimal("0.01"), DecimalDivisionException);
	d401 = Decimal("-31.908");
	CHECK_THROW(d401 /= Decimal(s_neg_max_digits_one_and_zeroes),
	  DecimalDivisionException);
	d401 = Decimal("-31.908");
	CHECK_THROW(d401 /= Decimal(s_neg_max_int_type_places_2),
	  DecimalDivisionException);
	d401 = Decimal("-31.908");
	CHECK_THROW(d402 /= Decimal(s_max_digits_one_and_zeroes),
	  DecimalDivisionException);
	d402 = Decimal("-1");
	CHECK_THROW(d402 /= Decimal::maximum(), DecimalException);
	d402 = Decimal("-1");
	CHECK_THROW(d402 /= Decimal(s_max_digits_one_and_zeroes_places_2) +
	  Decimal("0.01"), DecimalDivisionException);
	d402 = Decimal("-1");
	CHECK_THROW(d402 /= Decimal(s_max_int_type_places_2),
	  DecimalDivisionException);
	d402 = Decimal("-1");
	CHECK_THROW(d402 /= Decimal(s_neg_max_int_type_places_2),
	  DecimalDivisionException);
}

TEST_FIXTURE(DigitStringFixture, decimal_division)
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
	string s2000 = s_max_digits_less_one;
	s2000.resize(s2000.size() - 3);
	Decimal d2000(s2000);
	string s2001 = "0.000023";
	Decimal d2001(s2001);
	Decimal d2002;
	CHECK_THROW(d2002 = d2000 / d2001, DecimalDivisionException);
	Decimal d2003("20");
	CHECK_THROW(d2003 = Decimal("-1901") / Decimal("0"),
	  DecimalDivisionException);
	// Check value unchanged after exception
	Decimal d2003b("20");
	CHECK_EQUAL(d2003, d2003b);
	// Check again using standard try/catch, and different variable
	Decimal d2004("20");
	Decimal d2005("-1901");
	Decimal d2006("0");
	try
	{
		d2004 = d2005 / d2006;
	}
	catch (DecimalDivisionException&)
	{
	}
	CHECK_EQUAL(d2004, Decimal("20"));
	CHECK_EQUAL(d2005, Decimal("-1901"));
	CHECK_EQUAL(Decimal("0"), d2006);

	// Test elimination of trailing fractional zeroes
	ostringstream oss3000;
	oss3000 << Decimal("1.333333340") / Decimal("-1");
	CHECK_EQUAL(oss3000.str(), "-1.33333334");

	// Test that exception is thrown when dividend has too many
	// significant digits
	Decimal d400("1");
	Decimal d401("-31.908");
	Decimal d402("-1");
	CHECK_THROW
	(	d400 / Decimal(s_max_digits_one_and_zeroes),
		DecimalDivisionException
	);
	CHECK_THROW(d400 / Decimal::maximum(), DecimalDivisionException);
	CHECK_THROW
	(	d400 / (Decimal(s_max_digits_one_and_zeroes_places_2) +
		Decimal("0.01")), DecimalException
	);
	CHECK_THROW
	(	d400 / Decimal(s_neg_max_digits_one_and_zeroes),
		DecimalDivisionException
	);
	CHECK_THROW
	(	d400 / Decimal(s_neg_max_int_type_places_2),
		DecimalDivisionException
	);
	CHECK_THROW
	(	d401 / Decimal(s_max_digits_one_and_zeroes),
		DecimalDivisionException
	);
	CHECK_THROW
	(	d401 / Decimal::maximum(),
		DecimalException
	);
	CHECK_THROW
	(	d401 / (Decimal(s_max_digits_one_and_zeroes_places_2) +
	 	Decimal("0.01")), DecimalDivisionException
	);
	CHECK_THROW
	(	d401 / Decimal(s_neg_max_digits_one_and_zeroes),
		DecimalDivisionException
	);
	CHECK_THROW
	(	d401 / Decimal(s_neg_max_int_type_places_2),
		DecimalDivisionException
	);
	CHECK_THROW
	(	d402 / Decimal(s_max_digits_one_and_zeroes),
		DecimalDivisionException
	);
	CHECK_THROW(d402 / Decimal::maximum(), DecimalDivisionException);
	CHECK_THROW
	(	d402 / (Decimal(s_max_digits_one_and_zeroes_places_2) +
			Decimal("0.01")),
		DecimalDivisionException
	);
	CHECK_THROW
	(	d402 / Decimal(s_max_int_type_places_2),
		DecimalException
	);
	CHECK_THROW
	(	d402 / Decimal(s_neg_max_int_type_places_2),
		DecimalDivisionException
	);
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
	CHECK_THROW(++d10, DecimalIncrementationException);
	// Check value unchanged after exception
	CHECK_EQUAL(d10, Decimal::maximum());
	// Check again using standard try/catch and DecimalException
	try
	{
		++d10;
	}
	catch (DecimalException&)
	{
		CHECK(Decimal::maximum() == d10);
	}
	CHECK_EQUAL(d10, Decimal::maximum());
	CHECK_THROW(d10++, DecimalException);
	CHECK_THROW(d10++, DecimalIncrementationException);
	CHECK_THROW(d10++, jewel::Exception);
	CHECK_THROW(d10++, std::exception);
	CHECK_EQUAL(d10, Decimal::maximum());
	// This should be safe
	d10--;
	Decimal d11 = ++d10;
	// Then this should throw
	CHECK_THROW(d11++, DecimalIncrementationException);

	// Test preservation of fractional precision
	Decimal d12("1.900");
	++d12;
	ostringstream oss12;
	oss12 << d12;
	CHECK_EQUAL(oss12.str(), "2.900");
	d12++;
	ostringstream oss12b;
	oss12b << d12;
	CHECK_EQUAL(oss12b.str(), "3.900");
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
	CHECK_THROW(--d10, DecimalDecrementationException);
	// Check value unchanged after exception
	CHECK_EQUAL(d10, Decimal::minimum());
	// Check again using standard try/catch
	try
	{
		--d10;
	}
	catch (DecimalDecrementationException&)
	{
		CHECK_EQUAL(d10, Decimal::minimum());
	}
	// This should be safe
	++d10;
	CHECK_EQUAL(d10, Decimal::minimum() + Decimal("1"));
	d10--;
	CHECK_EQUAL(d10, Decimal::minimum());
	// This should throw
	CHECK_THROW(d10--, DecimalDecrementationException);
	// Check value unchanged after exception
	CHECK_EQUAL(d10, Decimal::minimum());
	// Check again using standard try/catch
	try
	{
		d10--;
	}
	catch (DecimalDecrementationException&)
	{
		CHECK_EQUAL(d10, Decimal::minimum());
	}
	CHECK(Decimal::minimum() == d10);

	// Check preservation of fractional precision
	Decimal d11("1.099000");
	d11--;
	ostringstream oss11;
	oss11 << d11;
	CHECK_EQUAL(oss11.str(), "0.099000");
	--d11;
	ostringstream oss11b;
	oss11b << d11;
	CHECK_EQUAL(oss11b.str(), "-0.901000");
}


TEST(decimal_operator_less_than)
{
	Decimal d0("19.98");
	Decimal d1("19.981");
	CHECK(d0 < d1);
	Decimal d2("-1");
	Decimal d3("1.0000");
	CHECK(d2 < d3);
	// Check operands unchanged
	CHECK_EQUAL(d2, Decimal("-1"));
	CHECK_EQUAL(d3, Decimal("1.0000"));
	CHECK_EQUAL(d3, Decimal("1"));
	ostringstream oss3;
	oss3 << d3;
	CHECK_EQUAL(oss3.str(), "1.0000");
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
	CHECK(Decimal("-102349187") < Decimal("-0.000012"));
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
	// Check operands unchanged after comparison
	Decimal d2("19237.02");
	Decimal d3("1234.2");
	bool b = (d2 > d3);
	CHECK_EQUAL(b, true);
	CHECK_EQUAL(d2, Decimal("19237.02"));
	CHECK_EQUAL(Decimal("1234.2"), d3);
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
	Decimal const d0c = d0;
	Decimal const d1c = d1;
	CHECK_EQUAL(d0, d1);
	CHECK_EQUAL(Decimal("2837.9900"), Decimal("2837.99"));
	// Check values unchanged after comparison
	CHECK_EQUAL(d0, d0c);
	CHECK_EQUAL(d1, d1c);
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

#endif  // JEWEL DECIMAL_OUTPUT_FAILURE_TEST



TEST(decimal_operator_output)
{
	#ifndef JEWEL_DECIMAL_OUTPUT_FAILURE_TEST
		// Failure test not enabled. Output should work
		// as normal
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
		ostringstream os11;
		os11 << numeric_limits<Decimal::int_type>::max();
		ostringstream os12;
		os12 << Decimal::maximum();
		CHECK_EQUAL(os11.str(), os12.str());
		ostringstream os13;
		os13 << numeric_limits<Decimal::int_type>::max() * -1;
		ostringstream os14;
		os14 << Decimal::maximum() * Decimal("-1");
		CHECK_EQUAL(os13.str(), os14.str());
		ostringstream os15;
		os15 << numeric_limits<Decimal::int_type>::min();
		ostringstream os15b;
		os15b << Decimal("0.01");
		CHECK_EQUAL(os15b.str(), "0.01");
		ostringstream os15c;
		os15c << Decimal("0.001");
		CHECK_EQUAL(os15c.str(), "0.001");
		ostringstream os15d;
		os15d << Decimal("-.00030");
		CHECK_EQUAL(os15d.str(), "-0.00030");
		ostringstream os16;
		os16 << Decimal::minimum();
		CHECK_EQUAL(os15.str(), os16.str());

		// Test internationalization support

		// C locale
		ostringstream os16b;
		os16b << Decimal("9300700.95838");
		CHECK_EQUAL(os16b.str(), "9300700.95838");

	      #ifndef __WIN32__ 

		// WARNING This test is implementation-dependant, because
		// locale names are implementation-dependant.
		// We don't compile here if on WIN32
		// WARNNG This sucks.
		// (However
		// the code these tests are testing is NOT implementation-dependant.)

		locale const german = locale("german");
		locale const french = locale("fr_FR");
		locale const nepali = locale("ne_NP"); 

		// German locale
		locale::global(german);
		JEWEL_ASSERT (use_facet< numpunct<char> >(german).grouping()[0] == 3);
		JEWEL_ASSERT (use_facet< numpunct<char> >(german).grouping().size() == 2);
		JEWEL_ASSERT (use_facet< numpunct<char> >(german).grouping()[0] == 3);
		JEWEL_ASSERT (use_facet< numpunct<char> >(german).grouping()[1] == 3);
		JEWEL_ASSERT (use_facet< numpunct<char> >(german).thousands_sep() == '.');
		JEWEL_ASSERT (use_facet< numpunct<char> >(german).decimal_point() == ',');
		ostringstream os17;
		os17 << Decimal("9300700.958");
		CHECK_EQUAL(os17.str(), "9.300.700,958");
		ostringstream os17b;
		os17b << Decimal("1000");
		CHECK_EQUAL(os17b.str(), "1.000");
		ostringstream os18;
		os18 << Decimal("900");
		CHECK_EQUAL(os18.str(), "900");
		ostringstream os19;
		os19 << Decimal("0");
		CHECK_EQUAL(os19.str(), "0");
		ostringstream os20;
		os20 << Decimal("-50800.5");
		CHECK_EQUAL(os20.str(), "-50.800,5");
		ostringstream os20b;
		os20b << Decimal("1000000000000");
		ostringstream os20c;
		os20c << 1000000000000LL;
		CHECK_EQUAL(os20b.str(), os20c.str());
		locale::global(locale::classic());
		
		// French locale
		locale::global(french);
		JEWEL_ASSERT (use_facet< numpunct<char> >(french).grouping()[0] == 3);
		JEWEL_ASSERT (use_facet< numpunct<char> >(french).grouping().size() == 1);
		JEWEL_ASSERT (use_facet< numpunct<char> >(french).grouping()[0] == 3);
		JEWEL_ASSERT (use_facet< numpunct<char> >(french).thousands_sep() == ' ');
		JEWEL_ASSERT (use_facet< numpunct<char> >(french).decimal_point() == ',');
		ostringstream os24;
		os24 << Decimal("898234.2983");
		CHECK_EQUAL(os24.str(), "898 234,2983");
		ostringstream os25;
		os25 << Decimal("-50000000.000");
		ostringstream os25b;
		CHECK_EQUAL(os25.str(), "-50 000 000,000");
		locale::global(locale::classic());
		ostringstream os26;
		os26.imbue(french);
		os26 << Decimal("7909");
		ostringstream os27;
		os27.imbue(os26.getloc());
		os27 << 7909;
		CHECK_EQUAL(os26.str(), os27.str());
		CHECK_EQUAL(os26.str(), "7 909");

		// Nepali locale
		locale::global(nepali);
		JEWEL_ASSERT (use_facet< numpunct<char> >(nepali).grouping().size() == 1);
		JEWEL_ASSERT (use_facet< numpunct<char> >(nepali).grouping() == "\3");
		JEWEL_ASSERT (use_facet< numpunct<char> >(nepali).decimal_point() == '.');
		JEWEL_ASSERT (use_facet< numpunct<char> >(nepali).thousands_sep() == ',');
		string const nepali_grouping =
			use_facet< numpunct<char> >(nepali).grouping();
		ostringstream os28;
		os28 << 2003678376;
		ostringstream os29;
		os29 << Decimal("2003678376");
		cout.imbue(locale::classic());
		CHECK_EQUAL(os28.str(), os29.str());
		ostringstream os30;
		os30 << Decimal("-2000000.5555");
		CHECK_EQUAL(os30.str(), "-2,000,000.5555");
		locale::global(locale::classic());

		// WeirdPunct locale
		// Apparently WeirdPunct needs to be newed here. And
		// if we delete it later it actually crashes.
		locale const weird_locale(locale(""), new WeirdPunct(""));
		locale::global(weird_locale);
		ostringstream os31;
		os31 << Decimal("-453709876.090");
		CHECK_EQUAL(os31.str(), "-453w709w87w6^090");
		locale::global(locale::classic());

		// With wide char
		wostringstream os32;
		os32 << Decimal(".897");
		wstring const ws32(os32.str());
		wstring const ws32b(L"0.897");
		// Note UnitTest++ macro CHECK_EQUAL breaks here
		bool const ok32 = (ws32 == ws32b);
		CHECK(ok32);
		wostringstream os33;
		os33 << Decimal("-8908.550");
		wstring const ws33(os33.str());
		wstring const ws33b(L"-8908.550");
		bool const ok33 = (ws33 == ws33b);
		CHECK(ok33);
		wostringstream os34;
		os34.imbue(french);
		os34 << Decimal("-88750.550");
		wstring const ws34(os34.str());
		wstring const ws34b(L"-88 750,550");
		bool const ok34 = (ws34 == ws34b);
		CHECK(ok34);

	    #endif /* __WIN32__ */

	#else	
		// JEWEL_DECIMAL_OUTPUT_FAILURE_TEST is defined.
		// Decimal output should fail. Here we test how the
		// failure is handled.
		ostringstream os_e;
		os_e.exceptions(std::ios::badbit);
		Decimal d_e(982, 1);
		bool const os_e_ok = os_e;
		// CHECK macro has trouble with conversion of stream to bool,
		// so we do it like this.
		CHECK(os_e_ok);
		CHECK_THROW(os_e << d_e, std::ios_base::failure);
		bool const os_e_ok_now = os_e;
		CHECK(!os_e_ok_now);
		CHECK_EQUAL(os_e.str(), string(""));
		CHECK(os_e.str().empty());
		ostringstream os2_e;
		bool const os2_e_ok = os2_e;
		CHECK(os2_e_ok);
		os2_e << d_e;
		bool const os2_e_ok_now = os2_e;
		CHECK(!os2_e_ok_now);
		CHECK_EQUAL(os2_e.str(), string(""));
		CHECK(os2_e.str().empty());
	#endif
}




#ifndef JEWEL_DECIMAL_OUTPUT_FAILURE_TEST
// If the output failure test has been enabled, it is simpler just to
// exclude all tests for Decimal other than it.

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
	// The UnitTest++ CHECK macro need explicit conversion here for
	// some reason
	CHECK(static_cast<bool>(is));
	// What about with wonky reads?
	istringstream bis0("90.00.0");
	Decimal d100;
	CHECK_EQUAL(d100, Decimal("0"));
	bis0 >> d100;
	CHECK(!static_cast<bool>(bis0));
	CHECK(bis0.rdstate() != std::ios::goodbit);
	// Check value hasn't changed.
	CHECK_EQUAL(d100, Decimal("0"));
	// Now with exceptions enabled on stream for failbit
	Decimal d100b("1");
	istringstream bis1("90.00.0");
	CHECK(static_cast<bool>(bis1));
	CHECK_EQUAL(bis1.rdstate(), std::ios::goodbit);
	bis1.exceptions(std::ios::failbit);
	CHECK(static_cast<bool>(bis1));
	CHECK_THROW(bis1 >> d100b, std::ios_base::failure);
	CHECK(!static_cast<bool>(bis1));
	// Check value hasn't changed
	CHECK_EQUAL(d100b, Decimal("1"));
		
	// Check the value has not changed
	CHECK_EQUAL(d100, Decimal("0"));
	Decimal d101("1234");
	istringstream bis2("8979ab");
	bis2 >> d101;
	JEWEL_ASSERT (!bis2);
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
	CHECK_THROW(-dmin, DecimalUnaryMinusException);
	CHECK_THROW(-dmin, DecimalException);
	CHECK_THROW(-dmin, jewel::Exception);
	CHECK_THROW(-dmin, std::exception);
	Decimal d4("34.712000");
	ostringstream oss4;
	oss4 << -d4;
	CHECK_EQUAL(oss4.str(), "-34.712000");
	CHECK_EQUAL(-Decimal("98774.09"), Decimal("0") - Decimal("98774.09"));
	CHECK_EQUAL(-Decimal("900"), Decimal("0.000") - Decimal("900.0"));
}

TEST(decimal_operator_unary_plus)
{
	CHECK_EQUAL(Decimal("89"), +Decimal("89"));
	CHECK_EQUAL(Decimal("-1.432"), +Decimal("-1.432"));
	Decimal d0("234");
	CHECK_EQUAL(d0, +d0);
	Decimal d1("1.0");
	ostringstream oss1;
	oss1 << +d1;
	CHECK_EQUAL(oss1.str(), "1.0");
}

TEST(decimal_value_preservation)
{
	double total = 0;
	for (int i = 0; i != 10; ++i)
	{
		total += double(1) / double(10);
	}
	JEWEL_ASSERT (total != double(1));

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
	Decimal d7(3, 2);
	CHECK_THROW(round(d7, 120), DecimalRangeException);
	CHECK_EQUAL(d7, Decimal(3, 2));
	CHECK_THROW(round(d7, -1), DecimalRangeException);
	CHECK_EQUAL(d7, Decimal(3, 2));
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

TEST(decimal_intval)
{
	Decimal d0("982.970");
	CHECK_EQUAL(d0.intval(), 982970);
	Decimal d1("-1849");
	CHECK_EQUAL(d1.intval(), -1849);
	Decimal d2("-0.0000");
	CHECK_EQUAL(d2.intval(), 0);
}

TEST(decimal_places)
{
	Decimal d0("2037.109000");
	CHECK_EQUAL(d0.places(), 6);
	Decimal d1("-90091");
	CHECK_EQUAL(d1.places(), 0);
	Decimal d2("-0.000");
	CHECK_EQUAL(d2.places(), 3);
}

TEST(decimal_maximum)
{
	Decimal m = Decimal::maximum();
	--m;
	++m;
	CHECK_THROW(++m, DecimalIncrementationException);
	ostringstream oss;
	oss << numeric_limits<Decimal::int_type>::max();
	CHECK_EQUAL(Decimal(oss.str()), Decimal::maximum());
}

TEST(decimal_minimum)
{
	Decimal m = Decimal::minimum();
	++m;
	--m;
	CHECK_THROW(--m, DecimalDecrementationException);
	ostringstream oss;
	oss << numeric_limits<Decimal::int_type>::min();
	CHECK_EQUAL(Decimal(oss.str()), Decimal::minimum());
}

TEST(decimal_maximum_precision)
{
	int m = Decimal::maximum_precision();
	string s = "1";
	for (int i = 1; i != m; ++i)
	{
		s += "0";
	}
	JEWEL_ASSERT (s.size() == static_cast<string::size_type>(m));
	Decimal d(s);
	s += "0";
	CHECK_THROW(Decimal e(s), DecimalRangeException);
}

#endif  // JEWEL_DECIMAL_OUTPUT_FAILURE_TEST
