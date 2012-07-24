#include "decimal.hpp"
#include "arithmetic_exceptions.hpp"
#include "checked_arithmetic.hpp"
#include "num_digits.hpp"  // for num_digits

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
 * If boost::numeric_cast is ever thrown, there's something wrong
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

// define static member constants

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
			throw UnsafeArithmeticException
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
			throw UnsafeArithmeticException
			(	"Unsafe attempt to set fractional precision in course "
				"of co-normalization attempt."
			);
		}
	}
	return;
}


// some member functions

Decimal::int_type
Decimal::implicit_divisor() const
{	
	// I could do this with a raw array
	// if I get desperate for speed.
	// I could even make the raw array a static
	// member variable.
	// But the below is more maintainable if I
	// later change the underlying integral type.
	static vector<int_type> lookup_table;
	static bool calculated_already = false;
	if (calculated_already)
	{
		return lookup_table[m_places];
	}
	assert (!calculated_already);
	int_type next_power = 1;
	for (size_t j = 0; j != s_max_places; ++j)
	{
		lookup_table.push_back(next_power);
		next_power *= s_base;
	}
	assert (lookup_table.size() == s_max_places);
	calculated_already = true;
	return lookup_table[m_places];
}


Decimal::int_type
Decimal::whole_part() const
{
	if (m_places == 0)
	{
		return m_intval;
	}
	assert (m_places > 0);
	Decimal temp = *this;
	
	// First set to fractional precison of 1.
	// This should be safe.
	#ifndef NDEBUG
		int check = 0;
		check = temp.rescale(1);
		assert (check == 0);		// No error occurred
	#else
		temp.rescale(1);
	#endif
	int_type ret = temp.m_intval;

	// Then truncate the fractional digit.
	// We don't want rounding here!
	ret /= s_base;
	return ret;
}


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
		assert (m_intval == 0);
		assert (m_places == 0);
		throw UnsafeArithmeticException
		(	"Cannot construct Decimal from an empty string"
		);
	}
	sz_t const str_size = str.size();
	
	// To hold string representation of underlying integer...
	// We will decrease this size later if there's a spot (decimal point)
	// as we won't hold the spot in str_rep.
	string str_rep(str_size, '\0');

	string::const_iterator si = str.begin();  // We'll through from this
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
			assert (m_intval == 0);
			assert (m_places == 0);
			throw UnsafeArithmeticException
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
				throw UnsafeArithmeticException
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
		assert (m_intval == 0);
		assert (m_places == 0);
		throw UnsafeArithmeticException
		(	"Attempt to set m_places "
			"to a value exceeding that returned by "
			"Decimal::maximum_precision()."
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
		m_intval = 0;
		assert (m_places == 0);
		throw UnsafeArithmeticException
		(	"Cannot create a Decimal as large as"
			" is implied by this string."
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
		double base = s_base;  // necessary only as pow needs a double
		int_type multiplier = NUM_CAST<int_type>
		(	pow(base, p_places - m_places)
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




// operators

Decimal const& Decimal::operator++()
{
	#ifndef NDEBUG
		places_type const benchmark_places = m_places;
	#endif
	if (addition_is_unsafe(m_intval, implicit_divisor()))
	{
		throw UnsafeArithmeticException("Addition may cause overflow.");
	}
	m_intval += implicit_divisor();
	assert (m_places >= benchmark_places);
	return *this;
}




Decimal const& Decimal::operator--()
{
	#ifndef NDEBUG
		places_type const benchmark_places = m_places;
	#endif
	if (subtraction_is_unsafe(m_intval, implicit_divisor()))
	{
		throw UnsafeArithmeticException
		(	"Subtraction may cause "
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
		throw UnsafeArithmeticException("Addition may cause overflow.");
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
		throw UnsafeArithmeticException("Subtraction may cause overflow.");
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
		throw UnsafeArithmeticException
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
			rescale(m_places - 1);
		}
		if (signs_differ) m_intval *= -1;
		rationalize();
		return *this;
	}

	*this = orig;
	throw UnsafeArithmeticException("Unsafe multiplication.");
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
		throw (UnsafeArithmeticException("Division by zero"));
	}
	
	// To prevent complications
	if ( m_intval == numeric_limits<int_type>::min() ||
	  rhs.m_intval == numeric_limits<int_type>::min() )
	{
		assert (*this == orig);
		throw UnsafeArithmeticException
		(	"Smallest possible Decimal cannot "
			"feature in division operation."
		);
	}
	assert (NumDigits::num_digits(rhs.m_intval) <= maximum_precision());
	if (NumDigits::num_digits(rhs.m_intval) == maximum_precision())
	{
		assert (*this == orig);
		throw UnsafeArithmeticException
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
		throw (UnsafeArithmeticException("Unsafe division."));
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
			throw UnsafeArithmeticException("Unsafe division.");
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
	// Take care of the easy cases first
	if (m_places == rhs.m_places)
	{
		return m_intval < rhs.m_intval;
	}
	if (m_intval < int_type(0) && rhs.m_intval >= int_type(0))
	{
		return true;
	}
	if (m_intval > 0 && rhs.m_intval <= 0)
	{
		return false;
	}
	if (m_intval == 0)
	{
		return rhs.m_intval > 0;
	}
	assert
	(	(m_intval < 0 && rhs.m_intval < 0) ||
		(m_intval > 0 && rhs.m_intval > 0)
	);
	
	// Now we're left with the more
	// difficult cases.

	// Try comparing the whole parts only.
	int_type const left_whole_part = whole_part();
	int_type const right_whole_part = rhs.whole_part();
	if (left_whole_part < right_whole_part)
	{
		return true;
	}
	assert (left_whole_part >= right_whole_part);
	if (left_whole_part > right_whole_part)
	{
		return false;
	}
	assert (left_whole_part == right_whole_part);

	// Record whether negative
	bool const is_negative = (m_intval < 0);

	// If one has fractional places but the other doesn't, then
	// we can compare, as we know the whole parts are equal.
	if ( (m_places > 0) && (rhs.m_places <= 0) )
	{
		return is_negative;
	}
	if ( (rhs.m_places > 0) && (m_places <= 0) )
	{
		return !is_negative;
	}

	// Now we're forced to compare
	// the fractional parts.
	ostringstream ossleft;
	ossleft << *this;
	ostringstream ossright;
	ossright << rhs;
	string leftstr = ossleft.str();
	string rightstr = ossright.str();
	assert ( (leftstr[0] == '-') == (rightstr[0] == '-') );
	string& smallerstring =
	(	leftstr.size() < rightstr.size() ?
	    leftstr : rightstr
	);
	size_t const greatersize =
	(	leftstr.size() < rightstr.size() ?
		rightstr.size() : leftstr.size()
	);
	
	// Get the strings to be the same size by adding zeroes to the
	// shorter one.
	while (smallerstring.size() != greatersize)
	{
		smallerstring.push_back('0');
	}
	assert (leftstr.size() == rightstr.size());
	assert (leftstr.size() == greatersize);

	// Now compare digit by digit.
	for (size_t j = 0; j != greatersize; ++j)
	{
		if (leftstr[j] < rightstr[j])
		{
			return !is_negative;
		}
		if (leftstr[j] > rightstr[j])
		{
			return is_negative;
		}
	}
	return false;	
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
		throw UnsafeArithmeticException
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
		throw UnsafeArithmeticException
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
	ostringstream oss;
	oss << numeric_limits<int_type>::min();
	ret = Decimal(oss.str());
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
	ostringstream oss;
	oss << numeric_limits<int_type>::max();
	ret = Decimal(oss.str());
	calculated_already = true;
	return ret;
}


}  // namespace jewel
