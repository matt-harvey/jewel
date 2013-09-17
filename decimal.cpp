// Copyright (c) 2013, Matthew Harvey. All rights reserved.

#include "decimal.hpp"
#include "assert.hpp"
#include "checked_arithmetic.hpp"
#include "decimal_exceptions.hpp"
#include "exception.hpp"
#include "num_digits.hpp"  // for num_digits

#include <algorithm>
#include <cmath>    // for pow
#include <cstdlib>  // for abs
#include <cctype>  // for isdigit(char) and iswdigit(wchar_t)
#include <limits>
#include <numeric>  // for accumulate
#include <vector>

#include <iostream>   // For logging only

/*
#ifndef NDEBUG
*/
	// TODO HIGH PRIORITY. I have made NUM_CAST ALWAYS
	// be boost::numeric_cast even on Release builds. This
	// is inefficient. However, if I used static_cast with
	// TDM-GCC 4.6.1 on Windows 7, there is a mysterious error
	// when casting from unsigned short to long long (even with
	// -std=c++0x flag). This error goes away if I use
	// numeric_cast. The task is to figure out what the
	// heck is going on there - it appears to be a compiler bug -
	// and fix it preferably by recompiling everything on my
	// Windows installation with a more recent version of GCC -
	// rather than by having to live with numeric_cast on release
	// builds (especially since I don't understand why it works
	// with numeric_cast and not static_cast). Note that, the error
	// ALSO goes away on Windows if I do cout << "Any string" << endl
	// just after the offending cast. It is utterly bizarre.
	#include <boost/numeric/conversion/cast.hpp>
	using boost::numeric_cast;
	#define NUM_CAST numeric_cast          // Safer
/*
#else
	#define NUM_CAST static_cast           // Faster
#endif
*/

/*
 * Don't rely on the exceptions thrown by boost::numeric_cast.
 * If boost::numeric_cast is throws here, there's something wrong
 * with the program!
 */

using std::accumulate;
using std::find;
using std::isdigit;
using std::numeric_limits;
using std::max_element;
using std::max;
using std::pow;
using std::vector;

using std::cerr;  // for logging only
using std::endl;  // for logging only

namespace jewel
{

// initialize static data members

Decimal::int_type const
Decimal::s_base = 10;

Decimal::int_type const
Decimal::s_rounding_threshold = 5;

size_t const
Decimal::s_max_places = NumDigits::num_digits
(	numeric_limits<int_type>::min()
);

Decimal const
Decimal::s_maximum = Decimal(numeric_limits<int_type>::max(), 0);

Decimal const
Decimal::s_minimum = Decimal(numeric_limits<int_type>::min(), 0);


// static member functions


void Decimal::co_normalize(Decimal& x, Decimal& y)
{
	if (x.m_places == y.m_places)
	{
		// do nothing
	}
	else if (x.m_places < y.m_places)
	{
		if (x.rescale(y.m_places) != 0)
		{
			JEWEL_THROW
			(	DecimalRangeException,
				"Unsafe attempt to set fractional precision in course "
				"of co-normalization attempt."
			);
		}
	}
	else
	{
		JEWEL_ASSERT (y.m_places < x.m_places);
		if (y.rescale(x.m_places) != 0)
		{
			JEWEL_THROW
			(	DecimalRangeException,
				"Unsafe attempt to set fractional precision in course "
				"of co-normalization attempt."
			);
		}
	}
	return;
}


// some member functions


void
Decimal::rationalize(places_type min_places)
{
	JEWEL_ASSERT (s_base > 0);
	JEWEL_ASSERT (!remainder_is_unsafe(m_intval, s_base));
	while ((m_places > min_places) && (m_intval % s_base == 0))
	{
		JEWEL_ASSERT (!division_is_unsafe(m_intval, s_base));
		m_intval /= s_base;
		JEWEL_ASSERT (m_places > 0);
		--m_places;
	}
	return;
}


Decimal::Decimal(int_type p_intval, places_type p_places):
	m_places(p_places),
	m_intval(p_intval)
{
	JEWEL_ASSERT (s_max_places == maximum_precision());
	if (m_places > s_max_places)
	{
		// There is no point setting m_intval and m_places to 0 (or any other
		// other valid value) here, since the Decimal instance is not going
		// to be created anyway - nothing will be able to refer to it after this
		// exception is thrown.
		JEWEL_THROW
		(	DecimalRangeException,
			"Attempt to construct Decimal with precision greater"
			" than maximum precision."
		);
	}
}




int Decimal::rescale(places_type p_places)
{
	#ifndef NDEBUG
		places_type const DEBUGVARIABLE_orig_places = m_places;
		int_type const DEBUGVARIABLE_orig_intval = m_intval;
	#endif

	if (m_places == p_places)
	{
		return 0;
	}

	// remember sign
	bool const is_positive = (m_intval > 0);
	#ifndef NDEBUG
		bool const is_zero = (m_intval == 0);
		bool const is_negative = (m_intval < 0);
	#endif

	if (m_places < p_places)
	{	
		if (p_places > s_max_places)
		{
			JEWEL_ASSERT (m_places == DEBUGVARIABLE_orig_places);
			JEWEL_ASSERT (m_intval == DEBUGVARIABLE_orig_intval);
			return 1;
		}
		JEWEL_ASSERT (p_places <= s_max_places);
		// This should never cause overflow, as p_places is never greater
		// than s_max_places, and s_base raised to a number equal to or
		// greater than p_places will always be less than
		// numeric_limits<int_type>::max(), given that s_max_places is
		// equal to the number of digits in numeric_limits<int_type>::min().
		int_type multiplier = NUM_CAST<int_type>
		(	pow(s_base, p_places - m_places)
		);

		if (multiplication_is_unsafe(m_intval, multiplier))
		{
			JEWEL_ASSERT (m_places == DEBUGVARIABLE_orig_places);
			JEWEL_ASSERT (m_intval == DEBUGVARIABLE_orig_intval);
			return 1;
	 	}
		m_intval *= multiplier;
	}
	else
	{
		JEWEL_ASSERT (p_places < m_places);

		// truncate all but one of the required places
		for (unsigned int j = m_places - 1; j != p_places; --j)
		{
			JEWEL_ASSERT (!division_is_unsafe(m_intval, s_base));
			m_intval /= s_base;
		}

		// with one more place still to eliminate, we calculate
		// whether rounding is required
		JEWEL_ASSERT (!remainder_is_unsafe(m_intval, s_base));
		bool remainder =
		(	std::abs(m_intval % s_base) >=
			s_rounding_threshold
		);

		// now remove the remaining place
		JEWEL_ASSERT (!division_is_unsafe(m_intval, s_base));
		JEWEL_ASSERT (s_base > 1);
		m_intval /= s_base;
		JEWEL_ASSERT (m_intval < numeric_limits<int_type>::max());
		JEWEL_ASSERT (m_intval > numeric_limits<int_type>::min());

		// and add rounding if required
		if (remainder)
		{
			if (is_positive)
			{
				JEWEL_ASSERT
				(	!addition_is_unsafe(m_intval, static_cast<int_type>(1))
				);
				++m_intval;
			}
			else
			{
				JEWEL_ASSERT (is_negative);
				JEWEL_ASSERT (!is_zero);
				JEWEL_ASSERT
				(	!subtraction_is_unsafe(m_intval, static_cast<int_type>(1))
				);
				--m_intval;
			}
		}
	}
	m_places = p_places;
	return 0;
}


Decimal::int_type
Decimal::implicit_divisor() const
{	
	static bool calculated_already = false;
	static vector<int_type> divisor_lookup(1, 1);
	while (!calculated_already)
	{
		while (divisor_lookup.size() != s_max_places)
		{
			JEWEL_ASSERT (!divisor_lookup.empty());
			JEWEL_ASSERT
			(	!multiplication_is_unsafe(divisor_lookup.back(), s_base)
			);
			divisor_lookup.push_back(divisor_lookup.back() * s_base);
		}
		calculated_already = true;
	}
	JEWEL_ASSERT (m_places < divisor_lookup.size());
	return divisor_lookup[m_places];
}



// operators

Decimal const& Decimal::operator++()
{
	#ifndef NDEBUG
		places_type const benchmark_places = m_places;
		Decimal const orig = *this;
	#endif
	if (addition_is_unsafe(m_intval, implicit_divisor()))
	{
		JEWEL_ASSERT (*this == orig);
		JEWEL_THROW
		(	DecimalIncrementationException,
			"Incrementation may cause overflow."
		);
	}
	m_intval += implicit_divisor();
	JEWEL_ASSERT (m_places >= benchmark_places);
	return *this;
}




Decimal const& Decimal::operator--()
{
	#ifndef NDEBUG
		places_type const benchmark_places = m_places;
		Decimal const orig = *this;
	#endif
	if (subtraction_is_unsafe(m_intval, implicit_divisor()))
	{
		JEWEL_ASSERT (*this == orig);
		JEWEL_THROW
		(	DecimalDecrementationException,
			"Decrementation may cause "
			"overflow."
		);
	}
	m_intval -= implicit_divisor();
	JEWEL_ASSERT (m_places >= benchmark_places);
	return *this;
}




Decimal& Decimal::operator+=(Decimal rhs)
{
	#ifndef NDEBUG
		places_type const benchmark_places = max(m_places, rhs.m_places);
	#endif
	Decimal orig = *this;
	co_normalize(*this, rhs);
	if (addition_is_unsafe(m_intval, rhs.m_intval))
	{
		*this = orig;
		JEWEL_THROW(DecimalAdditionException, "Addition may cause overflow.");
	}
	m_intval += rhs.m_intval;
	JEWEL_ASSERT (m_places >= benchmark_places);
	return *this;
}



Decimal& Decimal::operator-=(Decimal rhs)
{
	#ifndef NDEBUG
		places_type const benchmark_places = max(m_places, rhs.m_places);
	#endif
	Decimal orig = *this;
	co_normalize(*this, rhs);
	if (subtraction_is_unsafe(m_intval, rhs.m_intval))
	{
		*this = orig;
		JEWEL_THROW
		(	DecimalSubtractionException,
			"Subtraction may cause overflow."
		);
	}
	m_intval -= rhs.m_intval;
	JEWEL_ASSERT (m_places >= benchmark_places);
	return *this;
}


Decimal& Decimal::operator*=(Decimal rhs)
{
	Decimal orig = *this;
	rationalize();
	rhs.rationalize();

	// Rule out problematic smallest Decimal
	if (*this == minimum() || rhs == minimum())
	{
		JEWEL_ASSERT (*this == orig);
		JEWEL_THROW
		(	DecimalMultiplicationException,
			"Cannot multiply smallest possible "
			"Decimal safely."
		);
	}

	// Make absolute and remember signs
	bool const signs_differ =
	(	(m_intval < 0 && rhs.m_intval > 0) ||
	    (m_intval > 0 && rhs.m_intval < 0)
	);
	if (m_intval < 0) m_intval *= -1;
	if (rhs.m_intval < 0) rhs.m_intval *= -1;

	// Do "unchecked multiply" if we can
	JEWEL_ASSERT (m_intval >= 0 && rhs.m_intval >= 0);	
	if (!multiplication_is_unsafe(m_intval, rhs.m_intval))
	{
		JEWEL_ASSERT (!addition_is_unsafe(m_places, rhs.m_places));
		m_intval *= rhs.m_intval;
		m_places += rhs.m_places;
		while (m_places > s_max_places)
		{
			JEWEL_ASSERT (m_places > 0);
			#ifndef NDEBUG
				int const check = rescale(m_places - 1);
				JEWEL_ASSERT (check == 0);
			#else
				rescale(m_places - 1);
			#endif
		}
		if (signs_differ) m_intval *= -1;
		rationalize();
		return *this;
	}

	*this = orig;
	JEWEL_THROW(DecimalMultiplicationException, "Unsafe multiplication.");
	JEWEL_ASSERT (false);  // Execution should never reach here.
	return *this;    // Silence compiler re. return from non-void function.

}


Decimal& Decimal::operator/=(Decimal rhs)
{
	// Record original dividend and divisor
	Decimal const orig = *this;

	rhs.rationalize();

	int_type rhs_intval = rhs.m_intval;

	// Capture division by zero
	if (rhs_intval == 0)
	{
		JEWEL_ASSERT (*this == orig);
		JEWEL_THROW(DecimalDivisionByZeroException, "Division by zero.");
	}
	
	// To prevent complications
	if ( m_intval == numeric_limits<int_type>::min() ||
	  rhs_intval == numeric_limits<int_type>::min() )
	{
		JEWEL_ASSERT (*this == orig);
		JEWEL_THROW
		(	DecimalDivisionException,
			"Smallest possible Decimal cannot "
			"feature in division operation."
		);
	}
	JEWEL_ASSERT (NumDigits::num_digits(rhs_intval) <= maximum_precision());
	if (NumDigits::num_digits(rhs_intval) == maximum_precision())
	{
		JEWEL_ASSERT (*this == orig);
		JEWEL_THROW
		(	DecimalDivisionException,
			"Dividend has a number of significant"
		 	"digits that is greater than or equal to the return value of "
			"Decimal::maximum_precision(); as a result, division cannot be "
			"performed safely."
		);
	}
	
	// Remember required sign of product
	bool const diff_signs =
	(	( m_intval > 0 && rhs_intval < 0) ||
		( m_intval < 0 && rhs_intval > 0)
	);

	// Make absolute
	JEWEL_ASSERT
	(	!multiplication_is_unsafe(m_intval, static_cast<int_type>(-1))
	);
	JEWEL_ASSERT
	(	!multiplication_is_unsafe(rhs_intval, static_cast<int_type>(-1))
	);
	if (m_intval < 0) m_intval *= -1;
	if (rhs_intval < 0) rhs_intval *= -1;

	// Rescale the dividend as high as we can
	while (m_places < rhs.m_places && rescale(m_places + 1) == 0)
	{
	}
	if (rhs.m_places > m_places)
	{
		// We can't rescale high enough to proceed, so reset and throw
		*this = orig;
		JEWEL_THROW(DecimalDivisionException, "Unsafe division.");
	}
	JEWEL_ASSERT (m_places >= rhs.m_places);

	// Proceed with basic division algorithm
	m_places -= rhs.m_places;
	JEWEL_ASSERT (rhs_intval != 0);
	JEWEL_ASSERT (m_intval != numeric_limits<int_type>::min());
	JEWEL_ASSERT (!remainder_is_unsafe(m_intval, rhs_intval));
	int_type remainder = m_intval % rhs_intval;
	JEWEL_ASSERT (!division_is_unsafe(m_intval, rhs_intval));
	m_intval /= rhs_intval;

	// Deal with any remainder using "long division"
	while (remainder != 0 && rescale(m_places + 1) == 0)
	{
		JEWEL_ASSERT (!multiplication_is_unsafe(remainder, s_base));

		/*
		 * Previously this commented-out section of code dealt
		 * with the case where it was unsafe to multiply remainder
		 * and s_base. However, this is now dealt with by the fact that
		 * an exception is thrown if the number of significant digits
		 * in the dividend is equal to Decimal::maximum_precision().
		 * This makes for a more straightforward API, since the
		 * below code caused loss of precision under difficult-to-explain
		 * circumstances.
		if (multiplication_is_unsafe(remainder, s_base))
		{
			// Then we can't proceed with ordinary "long division" safely,
			// and need to "scale down" first

			bool add_rounding_right = false;
			if (rhs_intval % s_base >= s_rounding_threshold)
			{
				add_rounding_right = true;
			}
			rhs_intval /= s_base;
			if (add_rounding_right)
			{
				JEWEL_ASSERT (!addition_is_unsafe(rhs_intval,
				  NUM_CAST<int_type>(1)));
				++(rhs_intval);
			}

			// Redo the Decimal division on a "safe scale"
			Decimal lhs = orig;
			if (lhs.m_intval < 0) lhs.m_intval *= -1;
			JEWEL_ASSERT (rhs_intval >= 0);
			lhs /= rhs;
			bool add_rounding_left = false;
			if (lhs.m_intval % s_base >= s_rounding_threshold)
			{
				add_rounding_left = true;	
			}
			lhs.m_intval /= s_base;
			if (add_rounding_left)
			{
				JEWEL_ASSERT (!addition_is_unsafe(lhs.m_intval,
				  NUM_CAST<int_type>(1)));
				++(lhs.m_intval);
			}
			if (diff_signs) lhs.m_intval *= -1;
			*this = lhs;

			return *this;
		}
		*/

		// It's safe to proceed with ordinary "long division"
		JEWEL_ASSERT (!multiplication_is_unsafe(remainder, s_base));
		remainder *= s_base;

		JEWEL_ASSERT (rhs_intval > 0);
		JEWEL_ASSERT (!remainder_is_unsafe(remainder, rhs_intval));
		int_type const temp_remainder = remainder % rhs_intval;
		JEWEL_ASSERT (!division_is_unsafe(remainder, rhs_intval));
		m_intval += remainder / rhs_intval;
		remainder = temp_remainder;
	}

	JEWEL_ASSERT (rhs_intval >= remainder);
	JEWEL_ASSERT (!subtraction_is_unsafe(rhs_intval, remainder));
	
	// Do rounding if required
	if (rhs_intval - remainder <= remainder)
	{
		// If the required rounding would be unsafe, we throw
		if (addition_is_unsafe(m_intval, NUM_CAST<int_type>(1)))
		{
			*this = orig;
			JEWEL_THROW(DecimalDivisionException, "Unsafe division.");
		}
		// Do the rounding, it's safe
		++m_intval;
	}

	// Put the correct sign
	// TODO HIGH PRIORITY Is this multiplication operation guaranteed
	// to be safe?
	if (diff_signs) m_intval *= -1;
	rationalize();
	return *this;
}


bool Decimal::operator<(Decimal rhs) const
{	
	Decimal lhs = *this;
	lhs.rationalize();
	rhs.rationalize();
	if (lhs.m_places == rhs.m_places)
	{
		return lhs.m_intval < rhs.m_intval;
	}
	bool const left_is_longer = (lhs.m_places > rhs.m_places);
	Decimal const *const shorter = (left_is_longer? &rhs: &lhs);
	Decimal const *const longer = (left_is_longer? &lhs: &rhs);
	places_type const target_places = shorter->m_places;
	int_type longers_revised_intval = longer->m_intval;
	int_type const shorters_intval = shorter->m_intval;
	bool const longer_is_negative = (longers_revised_intval < 0);
	for
	(	places_type longers_places = longer->m_places;
		longers_places != target_places;
		--longers_places
	)
	{
		JEWEL_ASSERT (s_base > 0);
		JEWEL_ASSERT (!division_is_unsafe(longers_revised_intval, s_base));
		longers_revised_intval /= s_base;
		JEWEL_ASSERT (longers_places > 0);
	}
	bool longer_is_smaller =
	(	longer_is_negative?
		(longers_revised_intval <= shorters_intval):
		(longers_revised_intval < shorters_intval)
	);
	return ( &lhs == (longer_is_smaller? longer: shorter) );
}


bool Decimal::operator==(Decimal rhs) const
{
	Decimal temp_lhs = *this;
	temp_lhs.rationalize();
	rhs.rationalize();
	return
	(	temp_lhs.m_intval == rhs.m_intval &&
		temp_lhs.m_places == rhs.m_places
	);
}


Decimal round(Decimal const& x, Decimal::places_type decimal_places)
{
	Decimal ret = x;
	if (ret.rescale(decimal_places) != 0)
	{	
		JEWEL_THROW
		(	DecimalRangeException,
			"Decimal number cannot safely be rounded to "
			"this number of places."
		);
	}
	return ret;
}


Decimal operator-(Decimal const& d)
{
	if (d.m_intval == numeric_limits<Decimal::int_type>::min())
	{
		JEWEL_THROW
		(	DecimalUnaryMinusException,
			"Unsafe arithmetic operation (unary minus)."
		);
	}
	JEWEL_ASSERT (d.m_intval != numeric_limits<Decimal::int_type>::min());
	Decimal ret = d;
	ret.m_intval *= -1;
	return ret;
}



// Specializations

template <>
char const Decimal::CharacterProvider<char>::null = '\0';

template <>
char const Decimal::CharacterProvider<char>::plus = '+';

template <>
char const Decimal::CharacterProvider<char>::minus = '-';

template <>
char const Decimal::CharacterProvider<char>::full_stop = '.';

template <>
bool Decimal::CharacterProvider<char>::is_digit(char c)
{
	return isdigit(c);
}

template <>
wchar_t const Decimal::CharacterProvider<wchar_t>::null = '\0';

template <>
wchar_t const Decimal::CharacterProvider<wchar_t>::plus = '+';

template <>
wchar_t const Decimal::CharacterProvider<wchar_t>::minus = '-';

template <>
wchar_t const Decimal::CharacterProvider<wchar_t>::full_stop = '.';

template <>
bool Decimal::CharacterProvider<wchar_t>::is_digit(wchar_t c)
{
	return iswdigit(c);
}


}  // namespace jewel
