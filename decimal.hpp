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



#include "decimal_exceptions.hpp"
#include <boost/cstdint.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/operators.hpp>
#include <cassert>
#include <cctype>   // for isdigit
#include <cstdlib>  // for abs
#include <cmath>
#include <istream>
#include <ostream>
#include <string>
#include <vector>


namespace jewel
{

/**
 * @class Decimal
 * @brief A "constrained floating point" decimal number class.
 *
 * Each number is represented as an integer (of int_type), and a number of
 * decimal places (of places_type).
 *
 * The number of decimal places can be changed at runtime. As such this is a
 * floating point class. However the range of magnitudes is deliberately
 * restricted.
 *
 * @todo LOW PRIORITY
 * I think I should change places_type to a signed and then just do
 * away with arbitrary restrictions. This would free me up a bit w.r.t.
 * the implementation of multiplication and division. Although it would
 * complicate output and input. I would also need to change things
 * everywhere where it relies on m_places always being positive.
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
 * Decimal::maximum_precision() static function.
 * Regardless of any of the behaviour
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
 * @todo LOW PRIORITY
 * My use of boost::int64_t may compromise portability on some (probably
 * obscure) platforms. It may or may not be worth addressing this.
 *
 * @todo LOW PRIORITY
 * Multiplication and division throw exceptions in some cases where
 * they should be able to calculate an answer. I have documented these
 * behaviours in the API docs. I don't believe this is a \e very serious
 * problem, as the behaviour is well documented and exceptions are thrown
 * rather than silent failure occurring. However, it is limiting
 * for certain use cases, and it makes for a messy API.
 * I should be able to get around this, and also
 * simplify my implementation of both division and multiplication, by
 * using boost::int32_t for int_type, while overflowing into boost::int64_t
 * within the implementation of these functions where required. The functions
 * will then only throw if the final value of m_intval cannot be fit into
 * boost::int32_t. The only problem then is that int_type is limited to
 * 32 bits. I'm not sure whether this is acceptable for e.g. an accounting
 * application. I think it is probably better to retain the current
 * "messy" situation until there's a clear need to reimplement things
 * in light of actual use cases.
 *
 * @todo LOW PRIORITY
 * Make it work as expected with standard library stream precision
 * manipulators and formatting. Create a function that takes a string
 * representation of a number, and the set of all the formatting flags
 * of an ostream (or a basic_ostream<>?), and use
 * those flags to format the string in accordance with those flags.
 * I should probably define this in a separate file. It could include
 * currency formats and so forth, too.
 *
 * @todo LOW PRIORITY
 * Make Doxygen apidocs build and install as part of make install, and then
 * just install to the installation directory the apidocs folder, not the
 * Doxyfile.
 *
 * @todo LOW PRIORITY
 * Stop Doxygen from spewing all those warnings. They relate only to Doxygen
 * markup.
 *
  @todo LOW PRIORITY
 * Division now incorporates rounding but: (a) it is a bit inefficient;
 * and (b) it contains a "hard-wired" behaviour of rounding up at 5, while not
 * actually referring to the Decimal::s_rounding_threshold constant to achieve
 * this. This is a kind of code repetition and so is bad.
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

	/** The type of the underlying integer representation of the
	 * Decimal number.
	 */
	typedef boost::int64_t int_type;

	/** The type of the integer representation of the number of
	 * decimal places (scale).
	 */
	typedef unsigned short places_type;

	// Output
	template <typename charT, typename traits>
	friend
	std::basic_ostream<charT, traits>&
	operator<<(std::basic_ostream<charT, traits>&, Decimal const&);

	// Rounding
	friend Decimal round
	(	Decimal const& x,
		Decimal::places_type decimal_places
	);

	/** Unary minus
	 *
	 * See separate documentation for this function.
	 */
	friend Decimal operator-(Decimal const& d);

	/** 
	 * Initializes the Decimal to 0, with 0 decimal places.
	 *
	 * Exception safety: <em>nothrow guarantee</em>.
	 */
	Decimal();

	/**
	 * Constructs a Decimal with an underlying integer of
	 * p_intval and with p_places decimal places to the right
	 * of the spot.
	 *
	 * @exception DecimalRangeException thrown if p_places
	 * exceeds the value returned by Decimal::maximum_precision().
	 *
	 * Exception safety: <em>strong guarantee</em>.
	 */
	Decimal(int_type m_intval, places_type p_places);

	/** Constructs a Decimal from a string.
	 *
	 * @param str is the string representation of a Decimal
	 * number.
	 *
	 * Currently str must be a possibly empty series of digits
	 * between 0 and 9 inclusive, possibly preceded by a minus
	 * sign, possibly followed by a spot ('.'), possibly followed
	 * by further digits between 0 and 9 inclusive. There must be
	 * at least one digit in the string.
	 * 
	 * Examples of accepted strings: "10", "0.0012", "-1.3", ".234".
	 *
	 * Examples of non-accepted strings: "10e2", "12312.23413434123424".
	 *
	 * @exception DecimalFromStringException is thrown if:\n
	 *   an empty string is passed to \c str; or\n
	 *   non-digit characters (other than '-' and '.', at the
	 *   appropriate point) are included in the string.
	 * 
	 * @exception DecimalRangeException is thrown if \c str is otherwise
	 *   valid, but
	 *   the position of the decimal point implies a number of decimal places
	 *   greater than the value returned by Decimal::maximum_precision().
	 *
	 * @exception DecimalRangeException also thrown if the implied Decimal
	 *   number would be required to exceed the maximum of the underlying
	 *   integral representation.
	 *
	 * @exception std::bad_alloc may be thrown - but this is unlikely - since
	 * the implementation of this function involves the construction of a
	 * std::string.
	 *
	 * Trailing zeroes to the right of the decimal point in the passed string
	 * influence the number of digits of fractional precision stored in the
	 * resulting Decimal. So \c Decimal("0.00") is stored with two digits of
	 * fractional precision. However it is still the case that
	 * <tt> Decimal("0.00") == Decimal("0") </tt>.
	 * Note leading negative signs in front of \c Decimal("0") or its
	 * equivalents are \e not stored.
	 *
	 * Exception safety: <em>strong guarantee</em>.
	 */
	explicit Decimal(std::string const& str);

	// use compiler-generated copy constructor and destructor

	/**
	 * Assigns by value.
	 *
	 * Exception safety: <em>nothrow guarantee</em>.
	 */
	Decimal& operator=(Decimal const&);

	/**
	 * @exception DecimalAdditionException thrown if addition
	 * would cause overflow. If this occurs, the left hand operand
	 * will be unchanged from
	 * its original value (and the right hand operand, as it is passed by
	 * value, will also be unchanged).
	 *
	 * @exception DecimalRangeException thrown if fractional precision
	 * cannot be maintained at the same level as that of the more precise
	 * of the two numbers being
	 * added. This ensures that - unlike in unchecked floating point
	 * arithmetic - adding two non-zero Decimals is always yield a Decimal
	 * that is equal to neither of the original Decimals (or else will throw
	 * an exception). If this exception is thrown, the left hand operand will
	 * be unchanged from its original value (as will the right hand operand,
	 * since it is passed by value).
	 * 
	 * Note trailing fractional zeroes are never "rationalized away" from the
	 * result. Stored fractional precision is always maintained at the level
	 * of the more precise of the operands, and if it cannot be, an exception
	 * is thrown.
	 *
	 * In virtue of operator+=(Decimal) being defined,
	 * \b operator+(Decimal, Decimal) is also defined (through the magic of
	 * Boost). It behaves as expected, and will throw exceptions under
	 * the same circumstances.
	 *
	 * Exception safety: <em>strong guarantee</em>.
	 */	
	Decimal& operator+=(Decimal);

	/**
	 * @exception DecimalSubtractionException is thrown if
	 * subtraction would cause overflow.
	 *
	 * @exception DecimalRangeException is thrown if fractional precision
	 * cannot be maintained
	 * at the same level as that of the more precise of the two numbers
	 * involved (the minuend and the subrahend).
	 * This ensures that - unlike in unchecked floating point
	 * arithmetic - subtracting one non-zero number from another will always
	 * yield a number that is equal to neither of the original numbers (or
	 * else will throw an exception).
	 *
	 * Note trailing fractional zeroes are never "rationalized away" from the
	 * result. Stored fractional precision is always maintained at the level
	 * of the more precise of the operands, and if it cannot be, an exception
	 * is thrown.

	 * In virtue of operator-=(Decimal) being defined,
	 * \b operator-(Decimal, Decimal) is also defined (through the magic
	 * of Boost). It behaves as expected, and will throw the same exceptions
	 * under the same circumstances.
	 *
	 * Exception safety: <em>strong guarantee</em>.
	 */
	Decimal& operator-=(Decimal);

	/**
	 * @exception DecimalMultiplicationException thrown if multiplication
	 * would cause overflow. If this occurs, the value of the left-hand
	 * operand will be unchanged from its original value, as will the value
	 * of the right-hand operand (since it is passed by value).
	 *
	 * Currently, for multiplication to be executed safely, it must be
	 * the case that the underlying integral representations of the Decimals
	 * being multiplied can themselves be multiplied without overflow. If
	 * this cannot occur, then the Decimal multiplication operation will
	 * throw an exception rather than proceeding with the operation. The
	 * simplest way to avoid an exception from multiplication is to ensure
	 * that the number of significant digits in the left multiplicand,
	 * plus the number of significant digits in the right multiplicand,
	 * is less than
	 * the value returned by Decimal::maximum_precision(). Note
	 * that all digits to the left of the decimal point (excluding the case
	 * where the only
	 * such digit is \c 0) are counted as significant digits.
	 * In Decimals with fractional parts (digits to the right of the 
	 * decimal point), the significant
	 * digits, going from left to right, start at the first non-zero digit,
	 * and continue consecutively until the last non-zero digit is reached
	 * (and the "boundary digits" just mentioned count as significant
	 * digits). The negative sign and the decimal point
	 * do not count as digits.
	 *
	 * Note also the smallest possible Decimal (the value returned by
	 * Decimal::minimum()) cannot be multiplied, and
	 * DecimalMultiplicationException is thrown if this is attempted.
	 *
	 * The fractional precision of the returned product is never more
	 * than a number of decimal places to the right
	 * of the decimal point equal to the value returned by 
	 * Decimal::maximum_precision(). The returned product is as precise as
	 * possible within this constraint; however, trailing fractional zeroes
	 * are always eliminated rather than being stored within the returned
	 * product.
	 *
	 * In virtue of operator*=(Decimal) being defined,
	 * \b operator*(Decimal, Decimal) is also defined (through the magic of
	 * Boost). It behaves as expected, and will throw the same exceptions
	 * under the same circumstances.
	 *
	 * Exception safety: <em>strong guarantee</em>.
	 */
	Decimal& operator*=(Decimal);

	/**
	 * @exception DecimalDivisionByZeroException is thrown if division by
	 * zero is attempted.
	 *
	 * @exception DecimalDivisionException is thrown if division would cause
	 * overflow.
	 *
	 * If an exception is thrown, the left-hand operand will be unchanged
	 * from its original value, as will the right-hand operand (since it
	 * is passed by value).
	 *
	 * The precision of the returned quotient is never more than
	 * a number of decimal places to the right
	 * of the decimal point, equal to the value
	 * returned by Decimal::maximum_precision().
	 * Also, the returned quotient never has
	 * more significant (non-zero) digits than this. Division returns a
	 * quotient that is as precise as possible within these constraints.
	 * However, pointless trailing zeroes to the right of the
	 * decimal point are eliminated rather than stored within the result.
	 *
	 * Currently, to be able to divide one Decimal by another,
	 * it must be possible for the implementation of the division
	 * function to increase the
	 * fractional precision (number of places to the right of the decimal
	 * point) of the dividend until it equals the fractional precision
	 * of the divisor. In some cases it is not possible to do this safely,
	 * in which case an exception
	 * is thrown. You can avoid this
	 * possibility by ensuring that you only divide Decimals where the
	 * fractional precision of the dividend is at least as great as that
	 * of the divisor; or, failing that, where the sum of the number of
	 * significant digits in the dividend, and the number of extra digits
	 * of fractional precision that would need to be added to match the
	 * fractional precision of the divisor, is less than the value returned
	 * by Decimal::maximum_precision().
	 *
	 * In addition, an exception will be thrown in cases where the number
	 * of significant digits in the dividend is equal to the return
	 * value of Decimal::maximum_precision(). This is due to limitations
	 * in the implementation. For division to succeed, the number of
	 * significant digits in the dividend must be less than the value
	 * returned by Decimal::maximum_precision(). Note
	 * that all digits to the left of the decimal point (excluding the case
	 * where the only
	 * such digit is \c 0) are counted as significant digits.
	 * In Decimals with fractional parts (digits to the right of the 
	 * decimal point), the significant
	 * digits, going from left to right, start at the first non-zero digit,
	 * and continue consecutively until the last non-zero digit is reached
	 * (and the "boundary digits" just mentioned count as significant
	 * digits). The negative sign and the decimal point
	 * do not count as digits.
	 *
	 * In virtue of operator/=(Decimal) being defined,
	 * \b operator/(Decimal, Decimal) is also defined (through the magic
	 * of Boost). It behaves as expected, and will through the same exceptions
	 * under the same circumstances.
	 *
	 * Exception safety: <em>strong guarantee</em>.
	 */
	Decimal& operator/=(Decimal);

	/**
	 * @exception DecimalIncrementationException is thrown if incrementing
	 * would cause overflow. If this happens, the Decimal will be unchanged
	 * from its original value.
	 *
	 * Incrementing a Decimal never changes its fractional precision.
	 *
	 * Exception safety: <em>strong guarantee</em>.
	 */
	Decimal const& operator++();  // prefix version

	/** @exception DecimalDecrementationException is throw if decrementing
	 * would cause overflow.
	 *
	 * Decrementing a Decimal never changes its fractional precision.
	 *
	 * Exception safety: <em>strong guarantee</em>.
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
	 * Exception safety: <em>nothrow guarantee</em>.
	 */
	bool operator<(Decimal) const;

	/**
	* Equality operator. Compares Decimals by value.
	* Note the following evaluates to \c true: \n
	* Decimal("-0.000") == Decimal("0");
	*
	* Exception safety: <em>nothrow guarantee</em>.
	*/
	bool operator==(Decimal) const;

	/**
	 * Return the underlying integer representing the Decimal.
	 *
	 * Exception safety: <em>nothrow guarantee</em>.
	 */
	int_type intval() const;	

	/**
	 * Return the number of digits of fractional precision in the
	 * Decimal, i.e. the number of digits to the right of the decimal
	 * point.
	 *
	 * Exception safety: <em>nothrow guarantee</em>.
	 */
	places_type places() const;

	/**
	 * Returns the largest possible Decimal number
	 *
	 * Exception safety: <em>nothrow guarantee</em>.
	 */
	static Decimal maximum();

	/**
	 * Returns the smallest possible Decimal number
	 *
	 * Exception safety: <em>nothrow guarantee</em>.
	 */
	static Decimal minimum();

	/**
	 * Returns the maximum number of digits of precision
	 *
	 * Exception safety: <em>nothrow guarantee</em>.
	 */
	static places_type maximum_precision();


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
	 * d0.rescale(3);
	 * assert(d0 == Decimal("90.235"));
	 * d0.rescale(5);
	 * assert(d0 == Decimal("90.23500"));
	 * assert(d0 == Decimal("90.235"));
	 * assert(d0.precision() == 3);
	 * @endcode
	 *
	 * @param p_places the new number of decimal places (i.e. number
	 * of digits of precision to the right of the decimal point)
	 *
	 * If p_places exceeds s_max_places, or if the function cannot otherwise
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
	rescale(places_type p_places);

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
	void rationalize(places_type min_places = 0);

	/**
	 * Base of arithmetic. I can't imagine this ever being equal to anything
	 * other than 10.
	 */
	static int_type const s_base;

	/**
	 * Threshold at which rounding goes up rather than down.
	 */
	static int_type const s_rounding_threshold;

	/**
	 * Maximum number of decimal places of precision to the right of
	 * the decimal point.
	 */
	static size_t const s_max_places;

	/**
	 * Largest possible Decimal.
	 */
	static Decimal const s_maximum;

	/**
	 * Smallest possible Decimal.
	 */
	static Decimal const s_minimum;

	/**
	 * The character used for the decimal point.
	 */
	static char const s_spot;

	/**
	 * Vector for looking up the implicit divisor of a Decimal instance
	 * based on the value of m_places.
	 */
	static std::vector<int_type> s_divisor_lookup;

	/**
	 * Underlying integer representation of Decimal number.
	 */
	int_type m_intval;

	/**
	 * Number of digits of precision to the right of the decimal point.
	 */
	places_type m_places;

	/** 
	 * Convert two Decimal objects to the same number of places by converting
	 * the one with the lesser number of places to the same number of places
	 * as the one with the greater number of places (while rescaling to
	 * maintain the same order of magnitude).
	 *
	 * @throws DecimalRangeException if the operation would cause
	 * overflow.
	 */
	static void co_normalize(Decimal&, Decimal&);

	/**
	 * Power of 10 by which the underlying integer is implicitly divided.
	 */
	int_type implicit_divisor() const;

	/** This constructor is deliberately unimplemented. Ensures if an int or
	 * a convertible-to-int is passed to constructor, compilation will fail.
	 */
	explicit Decimal(int);

	/**
	 * Called by output operator. This is not designed to be called by
	 * other functions. May throw boost::bad_lexical_cast (would be rare)
	 * or std::bad_alloc (even if exceptions not enabled on oss).
	 */
	void output_aux(std::ostringstream& oss) const;
	
}; // class Decimal


// non-member functions - declarations

/** Write to an output stream.
 * 
 * Exception safety: provides <em>nothrow guarantee</em>, \e unless exceptions
 * have been enabled for the output stream. In the
 * event of output failure, std::ios_base::badbit will be set on the
 * stream; but the setting of this flag will trigger an exception only if
 * the client has enabled exceptions for std::ios_base::badbit for this
 * stream. (At the time of writing this is not common practice.) The thrown
 * exception will be an instance of std::ios_base::failure. This might be
 * caused by internal memory allocation failure in the body of this function
 * (extremely unlikely); or it might be caused by an "external" error, e.g.
 * a disk being removed during the course of writing to the disk.
 */
template <typename charT, typename traits>
std::basic_ostream<charT, traits>&
operator<<(std::basic_ostream<charT, traits>&, Decimal const&);

/** Read from a std::istream
 *
 * @relates Decimal
 *
 * If the sequence of characters read from the stream is such that
 * it cannot be validly converted to a \c Decimal (see the \c Decimal
 * constructor that takes a \c std::string \c const& parameter, for
 * the circumstances in which this can occur), an exception is \c not
 * generally thrown, but rather the input stream will have
 * std::ios_base::failbit set on the stream, per the common convention.
 * In this case, the Decimal argument then retains
 * the value it had prior to commencement of the read operation. However,
 * an exception viz. std::ios_base::failure \e will be thrown if this
 * occurs where the input stream has had exceptions enabled for
 * std::ios_base::failbit. This is in
 * accordance with standard library convention. There is also an (extremely
 * small) chance of memory allocation failure during the read operation.
 * Again, std::ios::failbit will be set in this case, and
 * an exception, again an instance of std::ios_base::failbit, will be thrown
 * if and only if exceptions have been enabled for failbit for the stream.
 *
 * Exception safety: <em>nothrow guarantee</em>, unless exceptions have
 * been enabled for std::ios_base::failbit for the stream (see above).
 */
template <typename charT, typename traits>
std::basic_istream<charT, traits>&
operator>>(std::basic_istream<charT, traits>&, Decimal&);

/** Unary minus
 *
 * @relates Decimal
 *
 * @throws DecimalUnaryMinusException if you try to apply unary
 * minus to a value for which this is unsafe; for example, where the
 * underlying integral representation is \c long, it will be unsafe
 * to take the negative value of the Decimal corresponding to LONG_MIN
 * (which would then be the value returned by Decimal::minimum()).
 *
 * Exception safety: <em>strong guarantee</em>.
 */
Decimal operator-(Decimal const& d);

/** Unary plus
 *
 * Does what you would expect.
 *
 * @relates Decimal
 *
 * Exception safety: <em>nothrow guarantee</em>.
 */
Decimal operator+(Decimal const& d);

/** Rounding function
 *
 * @relates Decimal
 * 
 * @param x The Decimal number to be rounded.
 * @param decimal_places The number of decimal digits after the
 * zero to which you wish to round. Should be a \e Decimal::places_type.
 *
 * Note if you round to a number of decimal places greater than the current
 * fractional precision, it will return a Decimal with the requested
 * fractional precision (filling in the extra places effectively with zeroes).
 * If this cannot be safely done an exception will be thrown.
 *
 * @returns A decimal number by value (distinct from x, which is not changed).
 * @exception DecimalRangeException thrown if achieving the requested
 * degree of precision would cause overflow.
 *
 * Exception safety: <em>strong guarantee</em>.
 */
Decimal round(Decimal const& x, Decimal::places_type decimal_places);



// ****************//

// IMPLEMENTATIONS


inline
Decimal::Decimal(): m_intval(0), m_places(0)
{
}


inline
Decimal& Decimal::operator=(Decimal const& rhs)
{
	m_intval = rhs.m_intval;
	m_places = rhs.m_places;
	return *this;
}

// Inline member functions

inline
Decimal::int_type
Decimal::intval() const
{
	return m_intval;
}

inline 
Decimal::places_type
Decimal::places() const
{
	return m_places;
}

// Inline static class functions

inline
Decimal::places_type Decimal::maximum_precision()
{
	return s_max_places;
}

inline
Decimal
Decimal::minimum()
{
	return s_minimum;
}

inline
Decimal
Decimal::maximum()
{
	return s_maximum;
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
	if (!os)
	{
		return os;
	}
	try
	{
		// We will write to a basic_ostringstream initially. Only at the last
		// minute will we write to os itself.
		std::basic_ostringstream<charT, traits> ss;
		
		// Whatever exception-throwing behaviour the client has set for os
		// should also be mirrored in ss.
		ss.exceptions(os.exceptions());

		d.output_aux(ss);
		if (!ss)
		{
			// If any error flags have been set in ss, we now
			// mirror them here in os, and return rather
			// than potentially corrupt data to os.
			os.setstate(ss.rdstate());
			return os;
		}
		assert (ss);
		os << ss.str();
	}
	catch (std::bad_alloc&)
	{
		// Exception could be from failed allocation of
		// ostringstream or string (though both are
		// extremely unlikely).
		os.setstate(std::ios_base::badbit);
	}
	catch (boost::bad_lexical_cast&)
	{
		os.setstate(std::ios_base::badbit);
	}
	return os;
}


// Input

template <typename charT, typename traits>
std::basic_istream<charT, traits>&
operator>>(std::basic_istream<charT, traits>& is, Decimal& d)
{
	if (!is)
	{
		return is;
	}
	Decimal temp = d;
	try
	{
		std::string str;
		is >> str;
		if (!is)
		{
			return is;
		}
		try
		{	
			temp = Decimal(str);
		}
		catch (DecimalException&)
		{
			is.setstate(std::ios_base::failbit);
			return is;
		}
		d = temp;
	}
	catch (std::bad_alloc&)
	{
		is.setstate(std::ios_base::failbit);
		return is;
	}
	return is;
}



} // namespace jewel

#endif  // GUARD_decimal_hpp
