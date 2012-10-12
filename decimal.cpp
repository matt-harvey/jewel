#include "decimal.hpp"
#include "decimal_exceptions.hpp"
#include "checked_arithmetic.hpp"
#include "num_digits.hpp"  // for num_digits

#include <boost/lexical_cast.hpp>
#include <algorithm>
#include <cassert>
#include <cmath>    // for pow
#include <cstdlib>  // for abs
#include <istream>
#include <limits>
#include <numeric>  // for accumulate
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

#include <iostream>   // For logging only

#ifndef NDEBUG
	#include <boost/numeric/conversion/cast.hpp>
	using boost::numeric_cast;
	#define NUM_CAST numeric_cast          // Safer
#else
	#define NUM_CAST static_cast           // Faster
#endif

/*
 * Don't rely on the exceptions thrown by boost::numeric_cast.
 * If boost::numeric_cast is throws here, there's something wrong
 * with the program!
 */

using boost::lexical_cast;
using boost::bad_lexical_cast;
using std::accumulate;
using std::find;
using std::isdigit;
using std::istream;
using std::numeric_limits;
using std::basic_ostream;
using std::basic_ostringstream;
using std::max_element;
using std::ostream;
using std::ostringstream;
using std::max;
using std::pow;
using std::string;
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

char const
Decimal::s_spot = '.';

std::vector<Decimal::int_type>
Decimal::s_divisor_lookup(Decimal::s_max_places, 0);


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
			throw DecimalRangeException
			(	"Unsafe attempt to set fractional precision in course "
				"of co-normalization attempt."
			);
		}
	}
	else
	{
		assert (y.m_places < x.m_places);
		if (y.rescale(x.m_places) != 0)
		{
			throw DecimalRangeException
			(	"Unsafe attempt to set fractional precision in course "
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
	while ((m_places > min_places) && (m_intval % s_base == 0))
	{
		assert (m_places > 0);
		m_intval /= s_base;
		--m_places;
	}
	return;
}


Decimal::Decimal(int_type p_intval, places_type p_places):
	m_intval(p_intval),
	m_places(p_places)
{
	assert (s_max_places == maximum_precision());
	if (m_places > s_max_places)
	{
		// There is no point setting m_intval and m_places to 0 (or any other
		// other valid value) here, since the Decimal instance is not going
		// to be created anyway - nothing will be able to refer to it after this
		// exception is thrown.
		throw DecimalRangeException
		(	"Attempt to construct Decimal with precision greater"
			" than maximum precision."
		);
	}
}



Decimal::Decimal(string const& str): m_intval(0), m_places(0)
{
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
	
	typedef string::size_type sz_t;
	if (str.empty())
	{
		throw DecimalFromStringException
		(	"Cannot construct Decimal from an empty string"
		);
	}
	sz_t const str_size = str.size();
	
	// To hold string representation of underlying integer...
	// We will decrease this size later if there's a spot (decimal point)
	// as we won't hold the spot in str_rep.
	string str_rep(str_size, '\0');

	string::const_iterator si = str.begin();  // We'll read through from this
	string::iterator ri = str_rep.begin();    // And write through this

	if (*si == '-')
	{
		assert (ri < str_rep.end());
		*ri = '-';
		++si;
		++ri;
	}
	string::const_iterator const str_end = str.end();
	for ( ; *si != s_spot && si != str_end; ++si, ++ri)
	{
		assert (si < str_end);
		if (!isdigit(*si))  // Note: this is fairly cheap.
		{
			throw DecimalFromStringException
			(	"Invalid string passed "
				"to Decimal constructor."
			);
		}
		assert (ri < str_rep.end());
		*ri = *si;
	}
	sz_t spot_position = 0;   // for the position of decimal point	
	if (*si == s_spot)
	{
		// We have a spot.
		// We have a str_rep that's one too big
		sz_t reduced_size = str_size;
		str_rep.resize(--reduced_size);
		assert (reduced_size == str_rep.size());	
		assert (reduced_size < str_size);
		assert (str_size >= 1);

		// Jump over the spot in str
		++si;

		// Now let's get the remaining the digits
		assert (str_end == str.end());
		for ( ; si != str_end; ++si, ++ri)
		{
			++spot_position;        // To count no. of fractional places
			assert (si < str_end);
			if (!isdigit(*si))  // Note: this is fairly cheap.
			{
				assert (m_intval == 0);
				assert (m_places == 0);
				throw DecimalFromStringException
				(	"Invalid string passed to"
					" Decimal constructor."
				);
			}
			assert (reduced_size == str_rep.size());
			assert (ri < str_rep.end());
			*ri = *si;
		}
	}
	if (spot_position > s_max_places)
	{
		throw DecimalRangeException
		(	"Attempt to set m_places "
			"to a value exceeding that returned by "
			"Decimal::maximum_precision()."
		);
	}
	if (str_rep.empty() || str_rep == "-")
	{
		throw DecimalFromStringException
		(	"Attempt to create a Decimal without any digits."
		);
	}
	try
	{	
		// This lexical cast accounted for over half of the execution
		// time in this function last time I checked.
		m_intval = lexical_cast<int_type>(str_rep);
	}
	catch (bad_lexical_cast&)
	{
		throw DecimalRangeException
		(	"Attempt to create Decimal that is either too large, too small "
			"or too precise than is supported by the Decimal implementation."
		);
	}
	m_places = NUM_CAST<places_type>(spot_position);
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
			assert (m_places == DEBUGVARIABLE_orig_places);
			assert (m_intval == DEBUGVARIABLE_orig_intval);
			return 1;
		}
		// WARNING Can we guarantee this won't overflow?
		int_type multiplier = NUM_CAST<int_type>
		(	pow(s_base, p_places - m_places)
		);

		if (multiplication_is_unsafe(m_intval, multiplier))
		{
			assert (m_places == DEBUGVARIABLE_orig_places);
			assert (m_intval == DEBUGVARIABLE_orig_intval);
			return 1;
	 	}
		m_intval *= multiplier;
	}
	else
	{
		assert(p_places < m_places);

		// truncate all but one of the required places
		for (unsigned int j = m_places - 1; j != p_places; --j)
		{
			m_intval /= s_base;
		}

		// with one more place still to eliminate, we calculate
		// whether rounding is required
		bool remainder =
		(	std::abs(m_intval % s_base) >=
			s_rounding_threshold
		);

		// now remove the remaining place
		m_intval /= s_base;

		// and add rounding if required
		if (remainder)
		{
			if (is_positive) ++m_intval;
			else
			{
				assert (is_negative);
				assert (!is_zero);
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
	if (calculated_already)
	{
		return s_divisor_lookup[m_places];
	}
	assert (!calculated_already);
	int_type next_power = 1;
	for (size_t j = 0; j != s_max_places; ++j)
	{
		assert (j < s_divisor_lookup.size());
		s_divisor_lookup[j] = next_power;
		next_power *= s_base;
	}
	assert (s_divisor_lookup.size() == s_max_places);
	calculated_already = true;
	return s_divisor_lookup[m_places];
}


void
Decimal::output_aux(ostringstream& oss) const
{
	typedef std::string::size_type str_sz;

	// special case of zero
	if (m_intval == 0)
	{
		oss << '0';
		if (m_places > 0)
		{
			oss << Decimal::s_spot << std::string(m_places, '0');
		}
	}

	// special case of smallest possible m_intval - as we
	// cannot take the absolute value below
	else if (m_intval == std::numeric_limits<Decimal::int_type>::min())
	{
		assert (m_places == 0);
		oss << m_intval;
	}

	else
	{
		// Our starting point is the string of digits representing
		// the absolute value of the underlying integer
		std::string const s =
			boost::lexical_cast<std::string>(std::abs(m_intval));
		assert(s != "0");
		str_sz slen = s.size();
		
		// negative sign
		if (m_intval < 0) oss << '-';
		
		// case where the whole part is zero
		if (slen <= m_places)
		{
			oss << '0' << Decimal::s_spot;
			str_sz stop_here = m_places - slen;
			for (str_sz i = 0; i != stop_here; ++i) oss << '0';
			for (str_sz j = 0; j != slen; ++j) oss << s[j];
		}
		
		// case where the whole part is non-zero
		else
		{
			str_sz whole_digits = slen - m_places;
			str_sz k = 0;
			for ( ; k != whole_digits; ++k) oss << s[k];
			if (m_places > 0)
			{
				oss << Decimal::s_spot;
				for ( ; k != slen; ++k) oss << s[k];
			}
		}
	}
	return;
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
		assert (*this == orig);
		throw DecimalIncrementationException
		(	"Incrementation may cause overflow."
		);
	}
	m_intval += implicit_divisor();
	assert (m_places >= benchmark_places);
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
		assert (*this == orig);
		throw DecimalDecrementationException
		(	"Decrementation may cause "
			"overflow."
		);
	}
	m_intval -= implicit_divisor();
	assert (m_places >= benchmark_places);
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
		throw DecimalAdditionException("Addition may cause overflow.");
	}
	m_intval += rhs.m_intval;
	assert (m_places >= benchmark_places);
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
		throw DecimalSubtractionException("Subtraction may cause overflow.");
	}
	m_intval -= rhs.m_intval;
	assert (m_places >= benchmark_places);
	return *this;
}

Decimal& Decimal::operator*=(Decimal rhs)
{
	Decimal orig = *this;
	Decimal orig_rhs = rhs;
	rationalize();
	rhs.rationalize();

	// Rule out problematic smallest Decimal
	if (*this == minimum() || rhs == minimum())
	{
		assert (*this == orig);
		throw DecimalMultiplicationException
		(	"Cannot multiply smallest possible "
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
	assert (m_intval >= 0 && rhs.m_intval >= 0);	
	if (!multiplication_is_unsafe(m_intval, rhs.m_intval))
	{
		assert (!addition_is_unsafe(m_places, rhs.m_places));
		m_intval *= rhs.m_intval;
		m_places += rhs.m_places;
		while (m_places > s_max_places)
		{
			assert (m_places > 0);
			#ifndef NDEBUG
				int const check = rescale(m_places - 1);
				assert (check == 0);
			#else
				rescale(m_places - 1);
			#endif
		}
		if (signs_differ) m_intval *= -1;
		rationalize();
		return *this;
	}

	*this = orig;
	throw DecimalMultiplicationException("Unsafe multiplication.");
	assert (false);  // Execution should never reach here.
	return *this;    // Silence compiler re. return from non-void function.

}


Decimal& Decimal::operator/=(Decimal rhs)
{
	// Record original dividend and divisor
	Decimal const orig = *this;
	Decimal const orig_rhs = rhs;

	rhs.rationalize();

	// Capture division by zero
	if (rhs.m_intval == 0)
	{
		assert (*this == orig);
		throw (DecimalDivisionByZeroException("Division by zero."));
	}
	
	// To prevent complications
	if ( m_intval == numeric_limits<int_type>::min() ||
	  rhs.m_intval == numeric_limits<int_type>::min() )
	{
		assert (*this == orig);
		throw DecimalDivisionException
		(	"Smallest possible Decimal cannot "
			"feature in division operation."
		);
	}
	assert (NumDigits::num_digits(rhs.m_intval) <= maximum_precision());
	if (NumDigits::num_digits(rhs.m_intval) == maximum_precision())
	{
		assert (*this == orig);
		throw DecimalDivisionException
		(	"Dividend has a number of significant"
		 	"digits that is greater than or equal to the return value of "
			"Decimal::maximum_precision(); as a result, division cannot be "
			"performed safely."
		);
	}
	
	// Remember required sign of product
	bool const diff_signs =
	(	( m_intval > 0 && rhs.m_intval < 0) ||
		( m_intval < 0 && rhs.m_intval > 0)
	);

	// Make absolute
	if (m_intval < 0) m_intval *= -1;
	if (rhs.m_intval < 0) rhs.m_intval *= -1;

	// Rescale the dividend as high as we can
	while (m_places < rhs.m_places && rescale(m_places + 1) == 0)
	{
	}
	if (rhs.m_places > m_places)
	{
		// We can't rescale high enough to proceed, so reset and throw
		*this = orig;
		throw (DecimalDivisionException("Unsafe division."));
	}
	assert (m_places >= rhs.m_places);

	// Proceed with basic division algorithm
	m_places -= rhs.m_places;
	int_type remainder = m_intval % rhs.m_intval;
	m_intval /= rhs.m_intval;

	// Deal with any remainder using "long division"
	while (remainder != 0 && rescale(m_places + 1) == 0)
	{
		assert (!multiplication_is_unsafe(remainder, s_base));

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
			if (rhs.m_intval % s_base >= s_rounding_threshold)
			{
				add_rounding_right = true;
			}
			rhs.m_intval /= s_base;
			if (add_rounding_right)
			{
				assert (!addition_is_unsafe(rhs.m_intval,
				  NUM_CAST<int_type>(1)));
				++(rhs.m_intval);
			}

			// Redo the Decimal division on a "safe scale"
			Decimal lhs = orig;
			if (lhs.m_intval < 0) lhs.m_intval *= -1;
			assert (rhs.m_intval >= 0);
			lhs /= rhs;
			bool add_rounding_left = false;
			if (lhs.m_intval % s_base >= s_rounding_threshold)
			{
				add_rounding_left = true;	
			}
			lhs.m_intval /= s_base;
			if (add_rounding_left)
			{
				assert (!addition_is_unsafe(lhs.m_intval,
				  NUM_CAST<int_type>(1)));
				++(lhs.m_intval);
			}
			if (diff_signs) lhs.m_intval *= -1;
			*this = lhs;

			return *this;
		}
		*/

		// It's safe to proceed with ordinary "long division"
		assert(!multiplication_is_unsafe(remainder, s_base));
		remainder *= s_base;
		int_type temp_remainder = remainder % rhs.m_intval;
		m_intval += remainder / rhs.m_intval;
		remainder = temp_remainder;
	}

	assert (rhs.m_intval >= remainder);
	assert (!subtraction_is_unsafe(rhs.m_intval, remainder));
	
	// Do rounding if required
	if (rhs.m_intval - remainder <= remainder)
	{
		// If the required rounding would be unsafe, we throw
		if (addition_is_unsafe(m_intval, NUM_CAST<int_type>(1)))
		{
			*this = orig;
			throw DecimalDivisionException("Unsafe division.");
		}
		// Do the rounding, it's safe
		++m_intval;
	}

	// Put the correct sign
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
		longers_revised_intval /= s_base;
		assert (longers_places > 0);
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
		throw DecimalRangeException
		(	"Decimal number cannot "
			"safely be rounded to this number of places."
		);
	}
	return ret;
}


Decimal operator-(Decimal const& d)
{
	if (d.m_intval == numeric_limits<Decimal::int_type>::min())
	{
		throw DecimalUnaryMinusException
		(	"Unsafe arithmetic "
			"operation (unary minus)."
		);
	}
	assert (d.m_intval != numeric_limits<Decimal::int_type>::min());
	Decimal ret = d;
	ret.m_intval *= -1;
	return ret;
}

Decimal Decimal::minimum()
{
	static bool calculated_already = false;
	static Decimal ret;
	if (calculated_already)
	{
		return ret;
	}
	assert (!calculated_already);
	ret = Decimal(numeric_limits<int_type>::min(), 0);
	calculated_already = true;
	return ret;
}

Decimal Decimal::maximum()
{
	static bool calculated_already = false;
	static Decimal ret;
	if (calculated_already)
	{
		return ret;
	}
	assert (!calculated_already);
	ret = Decimal(numeric_limits<int_type>::max(), 0);
	calculated_already = true;
	return ret;
}


}  // namespace jewel
