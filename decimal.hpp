#ifndef GUARD_decimal_hpp
#define GUARD_decimal_hpp

/** @file decimal.hpp
 *
 * @brief Decimal number class 
 *
 * @author Matthew Harvey
 * @date 13 May 2012
 *
 * Copyright (c) 2012, Matthew Harvey. All rights reserved.
 */


#include <checked_arithmetic.hpp>
#include <boost/exception/all.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/operators.hpp>
#include <cassert>
#include <cctype>   // for isdigit
#include <cmath>
#include <istream>
#include <ostream>
#include <string>

#include "arithmetic_exceptions.hpp"

namespace jewel
{

/**
 * @class Decimal
 * @brief A "constrained floating point" decimal number class.
 *
 * Each number is represented as an integer (of int_type), and a number of
 * decimal places (an unsigned short).
 *
 * The number of decimal places can be changed at runtime. As such this is a
 * floating point class. However the range of magnitudes is deliberately
 * restricted.
 *
 * Addition and subtraction operations throw exceptions if they
 * would cause either overflow, or precision loss relative to the level of
 * precision (conceived as digits to the right of the decimal point) of the
 * more precise of the two Decimals being operated on. At this point,
 * multiplication and division do not guarantee any particular level
 * of precision, but do guarantee that they will throw an exception
 * rather than overflow.
 *
 * @todo Multiplication and division should offer specific guarantees about
 * precison, and the class documentation (above para.) should reflect this.
 *
 * @todo Test the below statements (up to the next "to-do") regarding
 * the behaviour of Decimals w.r.t. precision.
 *
 * There are two concepts of precision in regards to this Decimal class. The
 * \e total \e precision of an instance of this class is the total number of
 * decimal digits, to either the left or the right of the decimal point,
 * stored in the instance. The \e fractional \e precision is the number of
 * digits stored to the right of the decimal point, i.e. the number of
 * digits in the fractional part.
 *
 * The maximum total precision of any given Decimal is equal to the number
 * of decimal digits in the largest possible Decimal. This
 * number is implementation dependent, and is the number returned by the
 * Decimal::maximum() static function. Regardless of any of the behaviour
 * outlined below, the total precision of a Decimal will never exceed this
 * level. Note we cannot end up with a number that has more than this many
 * digits in its whole part, even if all but one of these digits are zero.
 * This significantly limits the range of this Decimal class compared to
 * typical floating point implementations.
 *
 * A single leading zero to the left of the decimal point
 * does not count towards the total precision of the Decimal.
 *
 * When a Decimal is constructed from a string, all digits of
 * precision that are implied by that string are retained in the Decimal
 * that is thereby constructed - regardless of whether some of these digits
 * are trailing fractional zeroes. For example, \c Decimal("0.2400")
 * will create a Decimal with 4 digits of total precision, and 4 digits of
 * fractional precision. If a Decimal cannot be created that would hold the
 * number of digits of precision implied by string, then an exception is
 * thrown.
 *
 * When addition or subtraction is performed on two Decimals,
 * the result will be such that:\n
 * (a) For each of the operands \e x, the fractional precision of the result
 * is greater than or equal to the fractional precision of \e x; and\n
 * (b) Trailing fractional zeroes are not stored, but are culled, unless doing
 * so would violate (a).
 * Thus, the result of "3.01 + 4.090" is "7.100" (stored with three digits
 * of fractional precision). But the result of "3.01 + 4.09" is "7.10" (stored
 * with two digits of fractional precision).
 * Note that \c Decimal("7.100") compares as equal with
 * \c Decimal("7.10") just as one would expect. However they don't always
 * behave identically. In particular, it may be possible to add some
 * other Decimals to \c Decimal("7.10") that it is not possible to add to
 * \c Decimal("7.100"), since the latter requires that a fractional precision
 * of 3 be preserved in the result, whereas the former requires only a
 * fractional precision of 2 to be preserved.
 * 
 * Multiplication and division behave slightly differently to addition and
 * subtraction. Trailing fractional zeroes in the result of these operations
 * are always "culled", even if this
 * would reduce the fractional precision of the result below that of either of
 * the operands. However, as with addition and subtraction, only fractional
 * zeroes are culled, and never whole digits.
 *
 * Apart from the above requirements, Decimals will store as many digits of
 * precision as possible. So "1/3" will result in "0.333...." with as many
 * trailing '3's as are permitted by the implementation.
 *
 * @todo Make it work as expected with standard library stream precision
 * manipulators and formatting. To do this properly, I need to understand
 * about how streams work. Create a function that takes a string
 * representation of a number, and the set of all the formatting flags
 * of an ostream (or a basic_ostream<>?), and use
 * those flags to format the string in accordance with those flags.
 * I should probably define this in a separate file. It could include
 * currency formats and so forth, too.
 * 
 * @todo In the unit tests for operator++ and operator--, the elaborate
 * setup for these tests is no longer necessary, now that the string
 * constructor allows us to initialize anything up to
 * numeric_limits<int_type>::max();
 *
 * @todo For eventual release, tidy the naughty NUM_CAST macro in decimal.cpp.
 * (Replace it everywhere with plain static_cast, providing of course that
 * the tests pass when compiled in release mode.)
 *
 * @todo Properly document how operations and constructors handle "trailing
 * zeroes". Currently they cull trailing fractional zeroes in the result, but
 * only up to point. That point is the maximm of the fractional precisions of
 * the most precise of the two operands.
 *
 * @todo Fully test the expected behaviour w.r.t. the number of trailing
 * fractional zeroes left behind by operations and constructors.
 *
 * @todo Division and multiplication do not incorporate rounding of the last
 * available digit of precision. Should they?
 */

class Decimal:
	// use boost.operators to automatically define certain operators
	// on the basis of others
	boost::less_than_comparable < Decimal, 
	boost::equality_comparable < Decimal,
	boost::addable < Decimal,
	boost::subtractable < Decimal,
	boost::multipliable < Decimal,
	boost::dividable < Decimal, 
	boost::incrementable < Decimal,
	boost::decrementable < Decimal
	> > > > > > > >		
{
public:

	// Output
	template <typename charT, typename traits>
	friend
	std::basic_ostream<charT, traits>&
	operator<<(std::basic_ostream<charT, traits>&, Decimal const&);

	// Rounding
	friend Decimal round(Decimal const& x, unsigned int decimal_places);

	// Unary minus
	friend Decimal operator-(Decimal const& d);

	/** The type of the underlying integer representation of the
	 * Decimal number.
	 */
	typedef long int_type;

	/** 
	 * Initializes the Decimal to 0, with 0 decimal places.
	 */
	Decimal();

	/** Constructs a Decimal from a string.
	 *
	 * @param str is the string representation of a Decimal
	 * number.
	 * 
	 * Examples of accepted strings: "10", "0.0012", "-1.3", ".234".
	 *
	 * Examples of non-accepted strings: "10e2", "12312.23413434123424".
	 *
	 * @exception jewel::UnsafeArithmeticException is thrown if:\n
	 *   an empty string is passed to str;\n
	 *   non-digit characters (other than '-' and '.', at the
	 *   appropriate point) are included in the string; or
	 *   the position of the decimal point implies a number of decimal places
	 *   greater than the value returned by Decimal::maximum_precision(); or
	 *   the implied Decimal number would be required to exceed the maximum of
	 *   the underlying integeral representation.
	 */
	explicit Decimal(std::string const& str);

	// use compiler-generated copy constructor and destructor

	/**
	 * Assigns by value.
	 * Does not throw.
	 */
	Decimal& operator=(Decimal const&);

	/**
	 * @exception jewel::UnsafeArithmeticException thrown if addition
	 * would cause overflow, or if precision cannot be maintained at the
	 * same level as that of the more precise of the two numbers being
	 * added. This ensures that - unlike in unchecked floating point
	 * arithmetic - the sum of two non-zero numbers is always equal to
	 * neither of the original numbers (or else throw an exception).
	 *
	 * In virtue of operator+=(Decimal) being defined,
	 * \b operator+(Decimal, Decimal) is also defined (through the magic of
	 * Boost). It behaves as expected, and will throw exceptions under
	 * the same circumstances.
	 */	
	Decimal& operator+=(Decimal);

	/**
	 * @exception jewel::UnsafeArithmeticException thrown if
	 * subtraction would cause overflow, or if precision cannot be maintained
	 * at the same level as that of the more precise of the two numbers
	 * involved (the minuend and the subrahend).
	 * This ensures that - unlike in unchecked floating point
	 * arithmetic - subtracting one non-zero number from another will always
	 * yield a number that is equal to neither of the original numbers (Or
	 * else throw an exception).
	 *
	 * In virtue of operator-=(Decimal) being defined,
	 * \b operator-(Decimal, Decimal) is also defined (through the magic
	 * of Boost). It behaves as expected, and will throw the same exceptions
	 * under the same circumstances.
	 */
	Decimal& operator-=(Decimal);

	/**
	 * @exception jewel::UnsafeArithmeticException thrown if multiplication
	 * would cause overflow.
	 *
	 * Precision is never more than a number of decimal places to the right
	 * of the decimal point, equal to the value returned by 
	 * Decimal::maximum_precision(). Sometimes it can be less: there are no
	 * guarantees at this point in regard to precision from the
	 * multiplication operation.
	 *
	 * @todo Multiplication should be offer a more well-defined guarantee in
	 * regards to precision.
	 *
	 * In virtue of operator*=(Decimal) being defined,
	 * \b operator*(Decimal, Decimal) is also defined (through the magic of
	 * Boost). It behaves as expected, and will throw the same exceptions
	 * under the same circumstances.
	 */
	Decimal& operator*=(Decimal);

	/**
	 * @exception jewel::UnsafeArithmeticException is thrown if
	 * division would cause overflow.
	 *
	 * Precision is never more than a number of decimal places to the right
	 * of the decimal point, equal to the value returned by 
	 * of the decimal point. Sometimes it can be less: there are no 
	 * guarantees at this point in regards to precision from the division
	 * operation.
	 *
	 * @todo Division should be able to offer a more well-defined
	 * guarantee in regards to precision.
	 *
	 * In virtue of operator/=(Decimal) being defined,
	 * \b operator/(Decimal, Decimal) is also defined (through the magic
	 * of Boost). It behaves as expected, and will through the same exceptions
	 * under the same circumstances.
	 */
	Decimal& operator/=(Decimal);

	/**
	 * @exception jewel::UnsafeArithmeticException is thrown if incrementing
	 * would cause overflow.
	 */
	Decimal const& operator++();  // prefix version

	/** @exception jewel::UnsafeArithmeticException is throw if decrementing
	 * would cause overflow.
	 */
	Decimal const& operator--();  // prefix version

	/**
	 * Less-than operator. Compares Decimals by value.
	 *
	 * In virtue of operator<(Decimal) and operator==(Decimal) being defined,
	 * the following operators are also defined (through the magic of
	 * Boost): \n
	 * \b operator>(Decimal) \b const \n
	 * \b operator<=(Decimal) \b const \n
	 * \b operator>=(Decimal) \b const \n
	 *
	 * These all behave as you would expect.
	 *
	 * @todo operator<(Decimal) could probably be made more efficient.
	 */
	bool operator<(Decimal) const;

	/**
	* Equality operator. Compares Decimals by value.
	*/
	bool operator==(Decimal) const;

	/**
	 * Returns the largest possible Decimal number
	 */
	static Decimal maximum();

	/**
	 * Returns the smallest possible Decimal number
	 */
	static Decimal minimum();

	/**
	 * Returns the maximum number of digits of precision
	 */
	static int maximum_precision();

private:

	/**
	 * Sets the number of decimal places of precision to 
	 * the right of the decimal point, while also rescaling the
	 * underlying integer (which may involve rounding) to preserve
	 * the same order of magnitude.
	 *
	 * For example:
	 * @code
	 * Decimal d0("90.23457");
	 * d0.set_fractional_precision(3);
	 * assert(d0 == Decimal("90.235"));
	 * d0.set_fractional_precision(5);
	 * assert(d0 == Decimal("90.23500"));
	 * assert(d0 == Decimal("90.235"));
	 * assert(d0.precision() == 3);
	 * @endcode
	 *
	 * @param p_places the new number of decimal places (i.e. number
	 * of digits of precision to the right of the decimal point)
	 *
	 * If p_places exceeds MAX_PLACES, or if the function cannot otherwise
	 * execute safely, an exception is \e not thrown, but
	 * rather a non-zero value is returned to indicate error. If this occurs
	 * the unsafe operation is not actually executed, but rather the Decimal
	 * is retained in its original state.
	 *
	 * The function is non-throwing primarily for reasons of efficiency.
	 *
	 * @returns an integer indicating whether the operation was successful,
	 * viz. 0 if successful, otherwise a non-zero value. 
	 */
	int
	set_fractional_precision(unsigned short p_places);

	/**
	 * Where the final digit(s) of the Decimal number are '0', this
	 * method "chops off the zeroes" by reducing the number of
	 * decimal places, while re-scaling the underlying integer to
	 * preserve the same order of magnitude.
	 * 
	 * For example: "0.3234000" rationalizes to "0.3234".
	 *
	 * If there are no trailing zeroes, then the function does nothing.
	 * 
	 * A minimum number of places can be set, so that the number of digits
	 * of fractional precision is not reduced below this number of places.
	 *
	 * For example, with a minimum number places of 5, "0.2324000"
	 * rationalizes to "0.23240".
	 *
	 * @parameter min_places is the minimum number of decimal places to retain
	 * to the right of the decimal point.
	 */
	void rationalize(unsigned short min_places = 0);

	/**
	 * Base of arithmetic. I can't imagine this ever being equal to
	 */
	static int_type const BASE;

	/**
	 * Threshold at which rounding goes up rather than down.
	 */
	static int_type const ROUNDING_THRESHOLD;

	/**
	 * Maximum number of decimal places of precision to the right of
	 * the decimal point.
	 */
	static size_t const MAX_PLACES;

	/**
	 * The character used for the decimal point.
	 */
	static char const SPOT;

	/**
	 * Underlying integer representation of Decimal number.
	 */
	int_type m_intval;

	/**
	 * Number of digits of precision to the right of the decimal point.
	 */
	unsigned short m_places;

	/** 
	 * Convert two Decimal objects to the same number of places by converting
	 * the one with the lesser number of places to the same number of places
	 * as the one with the greater number of places (while rescaling to
	 * maintain the same order of magnitude).
	 */
	static void co_normalize(Decimal&, Decimal&);

	/**
	 * Power of 10 by which the underlying integer is implicitly divided.
	 */
	int_type implicit_divisor() const;

	// Return an int_type being the value of the whole part of the Decimal,
	// i.e. the part "to the left of the decimal point".
	// This is like rounding down to nil decimal places, but returning
	// an int_type rather than another Decimal.
	int_type whole_part() const;

	/** This constructor deliberately unimplemented. Ensures if an int or
	 * a convertible-to-int is passed to constructor, compilation will fail.
	 */
	explicit Decimal(int);


	/* I have commented this constructor out unless and until I really
	 * need it.
	 *
	 * @param p_intval initializes the underlying integer.
	 * @param p_places initializes the number of decimal places (nil
	 * places by default).
	 *
	 * @exception jewel::UnsafeArithmeticException thrown if p_places
	 * exceeds the value returned by Decimal::maximum_precision().
	 *
	 * The underlying integer and the number of places to are set to nil
	 * before throwing.
	 */
	// explicit Decimal(int_type p_intval, unsigned short p_places = 0);

	/* I have commented this out unless and until I really need it.
	 * @param p_intval initializes the underlying integer.
	 * @param p_places initializes the number of decimal places (nil
	 * places by default).
	 *
	 * @exception jewel::UnsafeArithmeticException thrown if p_places
	 * exceeds the value returned by Decimal::maximum_precision().
	 *
	 * The underlying integer and the number of places are set to 0 before
	 * throwing.
	 */
	// explicit Decimal(int p_intval, unsigned short p_places = 0);

	/*
	 * I have commented this constructor out unless and until I really need
	 * it. Constructor to intercept and reject attempted initialization from
	 * a double.
	 */
	 // explicit Decimal(double p_intval, unsigned short p_places = 0);
	 


	/* I have commented this out unless and until I really need it.
	 * Read into a Decimal from a std::istream in two parts: first an int_type
	 * representing the underlying integer; then an unsigned short being the
	 * number of decimal places.
	 * 
	 * This function does not check, but presumes, that the correct types will
	 * be read from the stream.
	 *
	 * @exception jewel::UnsafeArithmeticException thrown when the number of
	 * decimal places exceeds the value returned by
	 * Decimal::maximum_precision().
	 */
	// This function was a quick way of reading from a stream - MUCH quicker
	// than the operator>> function, or initialization from a stream. However
	// I want to hide it and comment it out unless and until it's really
	// required.
	// std::istream& read_parts_from_stream(std::istream&);
	
	// write underlying integer and number of places to stream
	// I've commented this out unless and until I need it. The advantage of
	// this is it's much faster than operator<< method, but there's no need to
	// implement it or expose it at this point. (Note the implementation is
	// also commented out.)
	// std::ostream& write_parts_to_stream(std::ostream&) const;	

}; // class Decimal


// non-member functions - declarations

/** Write to an output stream.
 */
template <typename charT, typename traits>
std::basic_ostream<charT, traits>&
operator<<(std::basic_ostream<charT, traits>&, Decimal const&);

/** Read from a std::istream
 *
 * @relates jewel::Decimal
 *
 * @throws jewel::UnsafeArithmeticException thrown if:\n
 *   Invalid characters are read;\n
 *   There are too many digits; or\n
 *   There are too many places to the right of the decimal point.\n
 *   These mirror the exceptions thrown by Decimal(std::string const&)
 *   constructor - see documentation of constructor for more detail.
 */
template <typename charT, typename traits>
std::basic_istream<charT, traits>&
operator>>(std::basic_istream<charT, traits>&, Decimal&);

/** Unary minus
 *
 * @throws jewel::UnsafeArithmeticException if you try to apply unary
 * minus to a value for which this is unsafe; for example, where the
 * underlying integral representation is \c long, it will be unsafe
 * to take the negative value of the Decimal corresponding to LONG_MIN
 * (which would then be the value returned by Decimal::minimum()).
 */
Decimal operator-(Decimal const& d);

/** Unary plus
 */
Decimal operator+(Decimal const& d);

/** Rounding function
 * 
 * @param x The Decimal number to be rounded.
 * @param decimal_places The number of decimal digits after the
 * zero to which you wish to round. Should be an \e unsigned int.
 * @returns A decimal number by value (distinct from x, which is not changed).
 * @exception UnsafeArithmeticException thrown if achieving the requested
 * degree of precision would cause overflow.
 */
Decimal round(Decimal const& x, unsigned int decimal_places);



// ****************//

// IMPLEMENTATIONS


// Contructor with no parameters
inline
Decimal::Decimal(): m_intval(0), m_places(0)
{
}


// various member functions

/* This is a quick way of storing to a stream but for I've commented it
 * out unless and until I'm required to use or expose it.
inline
std::ostream& Decimal::write_parts_to_stream(std::ostream& os) const
{
	os << m_intval << ' ' << m_places;
	return os;
}
*/

inline
Decimal& Decimal::operator=(Decimal const& rhs)
{
	m_intval = rhs.m_intval;
	m_places = rhs.m_places;
	return *this;
}

// inline non-member functions

inline
Decimal operator+(Decimal const& d)
{
	return d;
}


// Output

template <typename charT, typename traits>
std::basic_ostream<charT, traits>&
operator<<(std::basic_ostream<charT, traits>& os, Decimal const& d)
{	
	typedef std::string::size_type str_sz;

	// We will write to a basic_ostringstream initially. Only at the last
	// minute will we write to os itself.
	std::basic_ostringstream<charT, traits> ss;
	unsigned short const plcs = d.m_places;
	Decimal::int_type dintval = d.m_intval;	

	// special case of zero
	if (dintval == 0)
	{
		ss << '0';
		if (plcs > 0) ss << Decimal::SPOT << std::string(plcs, '0');
		os << ss.str();
		return os;
	}

	// Our starting point is the string of digits representing
	// the absolute value of the underlying integer
	std::string s = boost::lexical_cast<std::string>(abs(dintval));
	assert(s != "0");
	str_sz slen = s.size();
	
	// negative sign
	if (dintval < 0) ss << '-';
	
	// case where the whole part is zero
	if (slen <= plcs)
	{
		ss << '0' << Decimal::SPOT;
		str_sz stop_here = plcs - slen;
		for (str_sz i = 0; i != stop_here; ++i) ss << '0';
		for (str_sz j = 0; j != slen; ++j) ss << s[j];
		os << ss.str();
		return os;
	}

	// case where the whole part is non-zero
	str_sz whole_digits = slen - plcs;
	str_sz k = 0;
	for ( ; k != whole_digits; ++k) ss << s[k];
	if (plcs > 0)
	{
		ss << Decimal::SPOT;
		for ( ; k != slen; ++k) ss << s[k];
	}
	os << ss.str();
	return os;
}


// Input

template <typename charT, typename traits>
std::basic_istream<charT, traits>&
operator>>(std::basic_istream<charT, traits>& is, Decimal& d)
{
	std::string str;
	is >> str;
	if (is)
	{
		try
		{	
			d = Decimal(str);
		}
		catch (UnsafeArithmeticException&)
		{
			// Record error in stream
			is.setstate(std::ios::failbit);
			return is;
		}
	}
	return is;
}



inline
int Decimal::maximum_precision()
{
	return MAX_PLACES;
}


} // namespace jewel

#endif  // GUARD_decimal_hpp
