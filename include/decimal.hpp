/*
 * Copyright 2012, 2013 Matthew Harvey
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

#ifndef GUARD_decimal_hpp_0809249049429432
#define GUARD_decimal_hpp_0809249049429432

/** @file
 */

#include <boost/lexical_cast.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <algorithm>
#include <cctype>
#include <cstdlib>  // for abs
#include <cmath>
#include <istream>
#include <locale>
#include <memory>  // for allocator
#include <ostream>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>

namespace jewel
{

/**
 * @brief A floating point decimal number class, with a somewhat limited
 * range, suited to use in accounting and financial applications.
 *
 * Each number is represented as an integer (of jewel::Decimal::int_type), and a
 * number of decimal places (of jewel::Decimal::places_type).
 *
 * The number of decimal places can be changed at runtime. As such this is a
 * floating rather than fixed point arithmetic type. However the range of
 * magnitudes is quite restricted compared to e.g. \e double.
 *
 * There are two concepts of precision for this Decimal class. The
 * <em>total precision</em> of a Decimal is the total number of
 * decimal digits, to either the left or the right of the decimal point,
 * stored in the instance. The <em>fractional precision</em> is the number of
 * digits stored to the right of the decimal point, i.e. the number of
 * digits in the fractional part.
 *
 * The maximum total precision of any given Decimal is equal to the number
 * of decimal digits in the largest possible Decimal. This
 * number is implementation-dependent, and is the number returned by
 * Decimal::maximum_precision(). Regardless of any of the behaviour
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
 * I should probably define this in a separate file. Note this is different
 * from the internationalization task which is separately noted.
 *
 * @todo MEDIUM PRIORITY
 * Internationalized output is working using the std::numpunct facilities.
 * But it doesn't support Boost.Locale. If the client uses Boost.Locale
 * instead of the standard library facilities, Decimal will
 * not do any formatting. Considering that
 * Boost.Locale \e seems to be necessary to get things working on Windows
 * properly, do I want to provide support for Boost.Locale in
 * jewel::Decimal?
 *
 * @todo MEDIUM PRIORITY
 * Support internationalization of input.
 *
 * @todo LOW PRIORITY
 * Division now incorporates rounding but: (a) it is a bit inefficient;
 * and (b) it contains a "hard-wired" behaviour of rounding up at 5, while not
 * actually referring to the Decimal::s_rounding_threshold constant to achieve
 * this. This is a kind of code repetition and so is bad.
 */
class Decimal
{
public:

    /** The type of the underlying integer representation of the
     * Decimal number.
     */
    typedef long long int_type;

    /** The type of the integer representation of the number of
     * decimal places (scale).
     */
    typedef unsigned char places_type;

    // Output
    template <typename charT, typename traits>
    friend
    std::basic_ostream<charT, traits>&
    operator<<(std::basic_ostream<charT, traits>&, Decimal const&);

    // Rounding
    friend Decimal round
    (   Decimal const& x,
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
     * @param str is the string representation of a Decimal number. Must
     * be either a <em>std::string const&</em> or a
     * <em>std::wstring const&</em>, or else compilation will fail.
     *
     * Currently str must be a non-empty series of digits
     * between 0 and 9 inclusive, possibly preceded by a minus
     * sign, possibly followed by a decimal point character, possibly
     * followed by further digits between 0 and 9 inclusive. There must be
     * at least one digit in the string. The spot character is
     * determined by the current global \e std::locale (whatever \e std::locale
     * is created by the default constructor for \e std::locale). It could be
     * be ".", "," or some other character - whatever represents a
     * decimal point in that locale.
     *
     * Assuming "." for the global locale's decimal point, the following are
     * examples of accepted strings: "10", "0.0012", "-1.3", ".234".
     *
     * Assuming a "." for the global locale's decimal point,
     * the following are examples of non-accepted strings: "10e2",
     * "12312.234134341.23424", "1,000".
     *
     * @exception DecimalFromStringException is thrown if:\n
     *   an empty string is passed to \c str; or\n
     *   non-digit characters (other than '-' and the decimal point, at the
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
    template <typename charT, typename traits, typename Alloc>
    explicit Decimal(std::basic_string<charT, traits, Alloc> const& str);

    /**
     * Precondition: the string must be null-terminated.
     *
     * Behaviour re. exceptions is the same as for the constructor
     * which takes a std::string.
     */
    explicit Decimal(char const* str);

    /**
     * Precondition: the string must be null-terminated.
     * 
     * Behaviour re. exceptions is the same as for the constructor which
     * takes a std::wstring.
     */
    explicit Decimal(wchar_t const* str);

    Decimal(Decimal const&) = default;
    Decimal(Decimal&&) = default;
    Decimal& operator=(Decimal const&) = default;
    Decimal& operator=(Decimal&&) = default;
    ~Decimal() = default;

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
     * point) of the dividend until it is greater than or equal to the
     * fractional precision of the divisor. In some cases it is not possible to
     * do this safely in which case an exception
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
     * Postfix and prefix version are both provided, with conventional
     * semantics.
     *
     * Exception safety: <em>strong guarantee</em>.
     */
    Decimal const& operator++();
    Decimal operator++(int);

    /** @exception DecimalDecrementationException is throw if decrementing
     * would cause overflow.
     *
     * Decrementing a Decimal never changes its fractional precision.
     *
     * Postfix and prefix version are both provided, with conventional
     * semantics.
     *
     * Exception safety: <em>strong guarantee</em>.
     */
    Decimal const& operator--();
    Decimal operator--(int);

    /**
     * Less-than operator. Compares Decimals by value.
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
    int rescale(places_type p_places);

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
    static int_type constexpr s_base = 10;

    /**
     * Threshold at which rounding goes up rather than down.
     */
    static int_type constexpr s_rounding_threshold = 5;

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
     * Number of digits of precision to the right of the decimal point.
     */
    places_type m_places;

    /**
     * Underlying integer representation of Decimal number.
     */
    int_type m_intval;

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
     * Writes a Decimal number backwards on a stream. (Backwards is
     * easier.)
     */
    template <typename charT, typename traits>
    void output_aux(std::basic_ostream<charT, traits>& oss) const;

    // Auxiliary class to help with char and wchar_t literals.
    template <typename charT>
    struct CharacterProvider
    {
        static charT constexpr null = '\0';
        static charT constexpr plus = '+';
        static charT constexpr minus = '-';
    };

}; // class Decimal


// Helper function

namespace detail
{

template <typename charT>
static bool is_digit(charT c);

}  // namespace detail


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
 *
 * Output is sensitive to the std::numpunct facet of the locale of the
 * stream being written to. "Thousands" separators, digit groupings and
 * the decimal point sign will adjust according to the facet. However,
 * jewel::Decimal does NOT currently support locales set by Boost.Locale.
 * This is a significant shortcoming, since Boost.Locale offers superior
 * localization facilities to those of the standard library.
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
 * In this case, std::ios::badbit, and
 * an exception, being an instance of std::ios_base::failure, will be thrown
 * if and only if exceptions have been enabled for badbit for the stream.
 *
 * Exception safety: <em>nothrow guarantee</em>, unless exceptions have
 * been enabled for std::ios_base::failbit or std::ios_base::badbit for the
 * stream (see above).
 */
template <typename charT, typename traits>
std::basic_istream<charT, traits>&
operator>>(std::basic_istream<charT, traits>&, Decimal&);

/**
 * @relates Decimal
 *
 * Behaves as would be expected given the behaviour of operator+=, and
 * throws under the same circumstances.
 */
Decimal const operator+(Decimal lhs, Decimal const& rhs);

/**
 * @relates Decimal
 *
 * Behaves as would be expected given the behaviour of operator-=, and
 * throws under the same circumstances.
 */
Decimal const operator-(Decimal lhs, Decimal const& rhs);

/**
 * @relates Decimal
 *
 * Behaves as would be expected given the behaviour of operator*=, and
 * throws under the same circumstances.
 */
Decimal const operator*(Decimal lhs, Decimal const& rhs);

/**
 * @relates Decimal
 *
 * Behaves as would be expected given the behaviour of operator/=, and
 * throws under the same circumstances.
 */
Decimal const operator/(Decimal lhs, Decimal const& rhs);

/**
 * @relates Decimal
 *
 * Behaves as would be expected given the behaviour of operator==.
 */
bool operator!=(Decimal const& lhs, Decimal const& rhs);

/**
 * @relates Decimal
 *
 * Behaves as would be expected given the behaviour of operator< and
 * of operator==.
 */
bool operator<=(Decimal const& lhs, Decimal const& rhs);

/**
 * @relates Decimal
 *
 * Behaves as would be expected given the behaviour of operator<.
 */
bool operator>(Decimal const& lhs, Decimal const& rhs);

/**
 * @relates Decimal
 *
 * Behaves as would be expected given the behaviour of operator> and
 * of operator==.
 */
bool operator>=(Decimal const& lhs, Decimal const& rhs);

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


}  // namespace jewel


// ****************//

#include "assert.hpp"
#include "decimal_exceptions.hpp"
#include "exception.hpp"

namespace jewel
{


// SPECIALIZATIONS - must come first

namespace detail
{

template <>
bool is_digit<char>(char c)
{
    return std::isdigit(c);
}

template <>
bool is_digit<wchar_t>(wchar_t c)
{
    return std::iswdigit(c);
}

}  // namespace detail


// IMPLEMENTATIONS

template <typename charT, typename traits, typename Alloc>
Decimal::Decimal(std::basic_string<charT, traits, Alloc> const& str):
    m_places(0),
    m_intval(0)
{
    // NOTE In regards to the restriction enforced by this static_assert,
    // the ONLY reason this restriction is in place is that types other
    // than std::string and std::wstring are UNTESTED. It may well be
    // that the ONLY action required to make this work perfectly well with other
    // types is to get rid of the static_assert.
    static_assert
    (   std::is_same<decltype(str), std::string const&>::value ||
        std::is_same<decltype(str), std::wstring const&>::value,
        "Decimal constructor expected either std::string const& or "
        "std::wstring const&, but received some other type."
    );

    typedef typename std::basic_string<charT> stringT;
    typedef typename stringT::size_type sz_t;

    std::locale const loc;  // global locale
    charT const spot_char =
        std::use_facet<std::numpunct<charT> >(loc).decimal_point();
    charT const null_char = CharacterProvider<charT>::null;
    charT const plus_char = CharacterProvider<charT>::plus;
    charT const minus_char = CharacterProvider<charT>::minus;

    // Writing through iterators here to make it as fast as I reasonably
    // can.
    //
    // This seems to be about as fast as using indexing, but roughly 25%
    // faster than using push_back.
    //
    // I've got plentiful asserts to ensure I don't read or write off the
    // end.
    //
    // Note the lexical cast near the end accounts for a huge chunk of the
    // execution time.
    //
    // Of course, if I want extremely fast construction of a Decimal,
    // the constructor-from-string is not the best constructor to achieve
    // that.
    
    if (str.empty())
    {
        JEWEL_THROW
        (   DecimalFromStringException,
            "Cannot construct Decimal from an empty string"
        );
    }
    sz_t const str_size = str.size();
    
    // To hold string representation of underlying integer...
    // We will decrease this size later if there's a spot (decimal point)
    // as we won't hold the spot in str_rep.
    stringT str_rep(str_size, null_char);
    typename stringT::const_iterator si = str.begin();  // Read through this
    typename stringT::iterator ri = str_rep.begin();    // Write through this

    if (*si == minus_char || *si == plus_char)
    {
        JEWEL_ASSERT (ri < str_rep.end());
        *ri++ = *si++;
    }

    typename stringT::const_iterator const str_end = str.end();
    for ( ; *si != spot_char && si != str_end; ++si, ++ri)
    {
        JEWEL_ASSERT (si < str_end);
        if (!detail::is_digit(*si))
        {
            JEWEL_THROW
            (   DecimalFromStringException,
                "Invalid string passed to Decimal constructor."
            );
        }
        JEWEL_ASSERT (ri < str_rep.end());
        *ri = *si;
    }
    sz_t spot_position = 0;   // for the position of decimal point    
    if (*si == spot_char)
    {
        // We have a spot.
        // We have a str_rep that's one too big
        sz_t reduced_size = str_size;
        JEWEL_ASSERT (reduced_size > 0);
        str_rep.resize(--reduced_size);
        JEWEL_ASSERT (reduced_size == str_rep.size());    
        JEWEL_ASSERT (reduced_size < str_size);
        JEWEL_ASSERT (str_size >= 1);

        // Jump over the spot in str
        ++si;

        // Now let's get the remaining the digits
        JEWEL_ASSERT (str_end == str.end());
        for ( ; si != str_end; ++si, ++ri)
        {
            ++spot_position;        // To count no. of fractional places
            JEWEL_ASSERT (si < str_end);
            if (!detail::is_digit(*si))
            {
                JEWEL_ASSERT (m_places == 0);
                JEWEL_ASSERT (m_intval == 0);
                JEWEL_THROW
                (   DecimalFromStringException,
                    "Invalid string passed to Decimal constructor."
                );
            }
            JEWEL_ASSERT (reduced_size == str_rep.size());
            JEWEL_ASSERT (ri < str_rep.end());
            *ri = *si;
        }
    }
    if (spot_position > s_max_places)
    {
        JEWEL_THROW
        (   DecimalRangeException,
            "Attempt to set m_places to a value exceeding that returned by "
            "Decimal::maximum_precision()."
        );
    }
    if (str_rep.size() <= 1)
    {
        if
        (   str_rep.empty() ||
            str_rep == stringT(1, minus_char) ||
            str_rep == stringT(1, plus_char)
        )
        {
            JEWEL_THROW
            (   DecimalFromStringException,
                "Attempt to create a Decimal without any digits."
            );
        }
    }
    try
    {   
        // This lexical cast accounted for over half of the execution
        // time in this function last time I checked.
        m_intval = boost::lexical_cast<int_type>(str_rep);
    }
    catch (boost::bad_lexical_cast&)
    {
        JEWEL_THROW
        (   DecimalRangeException,
            "Attempt to create Decimal that is either too large, too small "
            "or too precise than is supported by the Decimal implementation."
        );
    }
    m_places = boost::numeric_cast<places_type>(spot_position);
}

inline
Decimal::Decimal(char const* str)
{
    *this = Decimal(std::string(str));
}

inline
Decimal::Decimal(wchar_t const* str)
{
    *this = Decimal(std::wstring(str));
}

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


// Inline non-member functions

inline
Decimal const
operator+(Decimal lhs, Decimal const& rhs)
{
    lhs += rhs;
    return lhs;
}

inline
Decimal const
operator-(Decimal lhs, Decimal const& rhs)
{
    lhs -= rhs;
    return lhs;
}

inline
Decimal const
operator*(Decimal lhs, Decimal const& rhs)
{
    lhs *= rhs;
    return lhs;
}

inline
Decimal const
operator/(Decimal lhs, Decimal const& rhs)
{
    lhs /= rhs;
    return lhs;
}

inline
Decimal
operator+(Decimal const& d)
{
    return d;
}

inline
bool
operator!=(Decimal const& lhs, Decimal const& rhs)
{
    return !(lhs == rhs);
}

inline
bool
operator>(Decimal const& lhs, Decimal const& rhs)
{
    return rhs < lhs;
}

inline
bool
operator<=(Decimal const& lhs, Decimal const& rhs)
{
    return (lhs == rhs) || (lhs < rhs);
}

inline
bool
operator>=(Decimal const& lhs, Decimal const& rhs)
{
    return (lhs == rhs) || (rhs < lhs);
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

        ss.imbue(os.getloc());
        d.output_aux(ss);
        if (!ss)
        {
            // If any error flags have been set in ss, we now
            // mirror them here in os, and return rather
            // than potentially corrupt data to os.
            os.setstate(ss.rdstate());
            return os;
        }
        JEWEL_ASSERT (ss);
        std::basic_string<charT> const s = ss.str();
        std::reverse_copy
        (   s.begin(),
            s.end(),
            std::ostream_iterator<charT, charT, traits>(os)
        );
    }
    catch (std::exception&)
    {
        // Exception could be std::bad_alloc from failed allocation of
        // ostringstream or string (though both are
        // extremely unlikely).
        // Possibly others? Catch std::exception to be sure.
        os.setstate(std::ios_base::badbit);
    }
    return os;
}

template <typename charT, typename traits>
void
Decimal::output_aux(std::basic_ostream<charT, traits>& oss) const
{
    using std::locale;
    using std::numpunct;
    using std::reverse_copy;
    using std::use_facet;
    typedef typename std::basic_string<charT> stringT;
    typedef typename stringT::size_type str_sz;
    typedef typename std::basic_ostringstream<charT> ostringstreamT;
    typedef typename std::ostream_iterator<charT, charT, traits> ostream_itT;

    // Record target locale
    locale const loc(oss.getloc());
    charT const decimal_point =
        use_facet<numpunct<charT> >(loc).decimal_point();

    // We will be "manually" reflecting the locale's numpunct facet
    // in what we write to oss. If there is already a non-C
    // locale on oss, we get rid of it now. We don't want
    // some other locale's numpunct facet interfering with
    // our manual labours.
    oss.imbue(locale::classic());

    // Now we write the number BACKWARDS onto oss. It's easier
    // this way, especially when it comes to processing
    // "thousands separators".

    // special case of zero
    if (m_intval == 0)
    {
        if (m_places > 0)
        {
            oss << stringT(m_places, oss.widen('0')) << decimal_point;
        }
        oss << oss.widen('0');
    }

    // special case of smallest possible m_intval - as we
    // cannot take the absolute value below
    else if (m_intval == std::numeric_limits<Decimal::int_type>::min())
    {
        JEWEL_ASSERT (m_places == 0);
        ostringstreamT tempstream;
        tempstream.imbue(loc);
        tempstream << m_intval;
        stringT const tempstring = tempstream.str();
        reverse_copy
        (   tempstring.begin(),
            tempstring.end(),
            ostream_itT(oss)
        );
    }

    else
    {
        // Our starting point is the stringT of digits representing
        // the absolute value of the underlying integer.
        ostringstreamT tempstream;
        tempstream.imbue(locale::classic());
        tempstream << std::abs(m_intval);
        stringT s = tempstream.str();
    
        // Write the fractional part
        typename stringT::const_reverse_iterator const rend = s.rend();
        typename stringT::const_reverse_iterator rit = s.rbegin();
        str_sz digits_written  = 0;
        for
        (   ;
            (digits_written != m_places) && (rit != rend);
            ++rit, ++digits_written
        )
        {
            oss << *rit;
        }

        // Deal with any "filler zeroes" required in the
        // fractional part
        while (digits_written != m_places)
        {
            oss << oss.widen('0');
            ++digits_written;
        }

        // Write the decimal point if required
        if (m_places != 0) oss << decimal_point;
            
        // Write the whole part
        
        // Get format specifier for digit grouping
        std::string const grouping =
            use_facet< numpunct<charT> >(loc).grouping();
        if (grouping.empty())
        {
            // We don't have to deal with digit grouping
            for
            (   ;
                rit != rend;
                ++rit, ++digits_written
            )
            {
                oss << *rit;
            }
        }
        else
        {
            // We have to deal with digit grouping
            charT const separator =
                use_facet<numpunct<charT> >(loc).thousands_sep();
            std::string::const_iterator grouping_it = grouping.begin();
            JEWEL_ASSERT (!grouping.empty());
            std::string::const_iterator const last_group_datum
                = grouping.end() - 1;
            str_sz digits_written_this_group = 0;
            for
            (   ;
                rit != rend;
                ++rit, ++digits_written, ++digits_written_this_group
            )
            {
                if
                (   digits_written_this_group ==
                    static_cast<str_sz>(*grouping_it)
                )
                {
                    oss << separator;
                    digits_written_this_group = 0;
                    if (grouping_it != last_group_datum) ++grouping_it;
                }
                oss << *rit;
            }
        }

        // Write a leading zero if required
        if (digits_written == m_places)
        {
            oss << oss.widen('0');
        }
        
        // Write negative sign if required
        if (m_intval < 0) oss << '-';
    }
    #ifdef JEWEL_PERFORM_DECIMAL_OUTPUT_FAILURE_TEST
        // We cause bad memory allocation here to provoke
        // failure. This is to test how Decimal output
        // operator<< (which calls this function) handles
        // failure.
        std::string grow_me("a");
        while (true)
        {
            grow_me += grow_me;
        }
    #endif
    return;
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
        is.setstate(std::ios_base::badbit);
        return is;
    }
    return is;
}



} // namespace jewel

#endif  // GUARD_decimal_hpp_0809249049429432
