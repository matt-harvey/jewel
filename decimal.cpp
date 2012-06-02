#include "decimal.hpp"
#include "num_digits.hpp"  // for num_digits
#include "arithmetic_exceptions.hpp"

#include <algorithm>
#include <cassert>
#include <checked_arithmetic.hpp>
#include <cmath>    // for pow
#include <cstdlib>  // for abs
#include <istream>
#include <limits>
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
using std::abs;
using std::find;
using std::isdigit;
using std::istream;
using std::numeric_limits;
using std::basic_ostream;
using std::basic_ostringstream;
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
Decimal::BASE = 10;

Decimal::int_type const
Decimal::ROUNDING_THRESHOLD = 5;

size_t const
Decimal::MAX_PLACES = NumDigits::num_digits(numeric_limits<int_type>::min());

char const
Decimal::SPOT = '.';

// static member functions


void Decimal::co_normalize(Decimal& x, Decimal& y)
{
	if (x.m_places == y.m_places)
	{
		// do nothing
	}
	else if (x.m_places < y.m_places)
	{
		if (x.set_fractional_precision(y.m_places) != 0)
		{
			throw (UnsafeArithmeticException("Unsafe attempt to "
			  "set fractional precision in course of co-normalization"
			  " attempt."));
		}
	}
	else
	{
		assert (y.m_places < x.m_places);
		if (y.set_fractional_precision(x.m_places) != 0)
		{
			throw (UnsafeArithmeticException("Unsafe attempt to "
			  "set fractional precision in course of co-normalization"
			  " attempt."));
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
	for (size_t j = 0; j != MAX_PLACES; ++j)
	{
		lookup_table.push_back(next_power);
		next_power *= BASE;
	}
	assert (lookup_table.size() == MAX_PLACES);
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
		check = temp.set_fractional_precision(1);
		assert (check == 0);		// No error occurred
	#else
		temp.set_fractional_precision(1);
	#endif
	int_type ret = temp.m_intval;

	// Then truncate the fractional digit.
	// We don't want rounding here!
	ret /= BASE;
	return ret;
}


void
Decimal::rationalize(unsigned short min_places)
{
	while ((m_places > min_places) && (m_intval % BASE == 0))
	{
		if (m_places <= 0)
		{
			cerr << endl << endl << m_places << endl << endl;
		}
		assert (m_places > 0);
		m_intval /= BASE;
		--m_places;
	}
	return;
}


/*
I have commented these constructors out unless and until
I really need them.
// Construct from a native integer of int_type and an unsigned short
Decimal::Decimal(Decimal::int_type p_intval, unsigned short places):
  m_intval(p_intval), m_places(places)
{
	if (places > MAX_PLACES)
	{
		m_intval = 0;
		m_places = 0;
		throw (UnsafeArithmeticException("Decimal places passed"
		  " to Decimal constructor exceed maximum"));
	}
}

// Construct from an int and an unsigned short
Decimal::Decimal(int p_intval, unsigned short places):
  m_intval(p_intval), m_places(places)
{
	if (places > MAX_PLACES)
	{
		m_intval = 0;
		m_places = 0;
		throw (UnsafeArithmeticException("Decimal places passed"
		  " to Decimal constructor exceed maximum"));
	}
}

*/

Decimal::Decimal(string const& str): m_intval(0), m_places(0)
{
	// Writing though indexes here to try to make it as fast
	// as possible.
	// It is about 25% faster than using push_back.
	// (Maybe not worth it... though risk of writing through unchecked
	// indexes is ameliorated via plentiful asserts.)
	
	typedef string::size_type sz_t;
	if (str.empty())
	{
		m_intval = 0;
		m_places = 0;
		throw UnsafeArithmeticException(
		  "Cannot construct Decimal from an empty string");
	}
	sz_t const str_size = str.size();
	
	// To hold string representation of underlying integer
	// We will decrease this size later if there's a spot (decimal point)
	// as we won't hold the spot in str_rep.
	string str_rep(str_size, '\0');

	sz_t si = 0;
	if (str[0] == '-')
	{
		assert (si < str_size);
		str_rep[si] = '-';
		++si;
	}
	// Note copying str[si] to a local const here seem to speed things up.
	for ( ; str[si] != SPOT && si != str_size; ++si)
	{
		assert (si < str.size());
		if (!isdigit(str[si]))  // Note: this is fairly cheap.
		{
			m_intval = 0;
			m_places = 0;
			throw UnsafeArithmeticException("Invalid string passed "
			  "to Decimal constructor.");
		}
		assert (str_size == str_rep.size());
		assert (si < str_size);
		assert (si < str.size());
		str_rep[si] = str[si];
	}
	sz_t spot_position = 0;   // for the position of decimal point	
	if (str[si] == SPOT)
	{
		// We have a spot.
		// We have a str_rep that's one too big
		sz_t reduced_size = str_size;
		str_rep.resize(--reduced_size);
		assert (reduced_size == str_rep.size());	
		assert (reduced_size < str_size);
		assert (str_size >= 1);

		sz_t ri = si;  // We now need a separate index for str_rep.
		
		// Jump over the spot in str
		++si;

		// Now let's get the remaining the digits
		for ( ; si != str_size; ++si)
		{
			++spot_position;        // To count no. of fractional places
			assert (si < str.size());
			if (!isdigit(str[si]))  // Note: this is fairly cheap.
			{
				m_intval = 0;
				m_places = 0;
				throw UnsafeArithmeticException("Invalid string passed to"
				  " Decimal constructor.");
			}
			assert (reduced_size == str_rep.size());
			assert (ri < reduced_size);
			assert (si < str.size());
			str_rep[ri] = str[si];
			++ri;
		}
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
		m_places = 0;
		throw UnsafeArithmeticException("Cannot create a Decimal as large as"
		  " is implied by this string.");
	}
	if (spot_position > MAX_PLACES)
	{
		m_intval = 0;
		m_places = 0;
		throw (UnsafeArithmeticException("Attempt to set m_places "
		  "to a value exceeding MAX_PLACES."));
	}
	m_places = NUM_CAST<unsigned short>(spot_position);
}


// miscellaneous functions

// This function was a quick way of reading from a stream - MUCH quicker than
// the operator>> function, or initialization from a string. However I want
// to hide it and comment it out unless and until it's really required.
/*
istream& Decimal::read_parts_from_stream(istream& is)
{
	unsigned short new_places;
	is >> m_intval >> new_places;
	if (new_places > MAX_PLACES)
	{
		m_intval = 0;
		new_places = 0;
		throw UnsafeArithmeticException("Attempted to set"
		  " m_places to a value of exceeding MAX_PLACES.");
	}
	m_places = new_places;
	return is;
}
*/

int Decimal::set_fractional_precision(unsigned short p_places)
{
	#ifndef NDEBUG
		unsigned short const DEBUGVARIABLE_orig_places = m_places;
		int_type const DEBUGVARIABLE_orig_intval = m_intval;
	#endif

	if (m_places == p_places)
	{
		return 0;
	}

	if (m_places < p_places)
	{	
		if (p_places > MAX_PLACES)
		{
			assert (m_places == DEBUGVARIABLE_orig_places);
			assert (m_intval == DEBUGVARIABLE_orig_intval);
			return 1;
		}
		double base = BASE;  // necessary only as pow needs a double
		int_type multiplier =
		  NUM_CAST<int_type>(pow(base, p_places - m_places));

		if (CheckedArithmetic::multiplication_is_unsafe(m_intval,
		  multiplier))
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
			m_intval /= BASE;
		}

		// with one more place still to eliminate, we calculate
		// whether rounding is required
		bool remainder =
		  abs(m_intval % BASE) >= ROUNDING_THRESHOLD;

		// now remove the remaining place
		m_intval /= BASE;

		// and add rounding if required
		if (remainder)
		{
			if (m_intval > 0) ++m_intval;
			else --m_intval;
		}
	}
	m_places = p_places;
	return 0;
}




// operators

Decimal const& Decimal::operator++()
{
	#ifndef NDEBUG
		unsigned short const benchmark_places = m_places;
	#endif
	if (CheckedArithmetic::addition_is_unsafe(m_intval,
	  implicit_divisor()))
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
		unsigned short const benchmark_places = m_places;
	#endif
	if (CheckedArithmetic::subtraction_is_unsafe(m_intval,
	  implicit_divisor()))
	{
		throw UnsafeArithmeticException("Subtraction may cause "
		  "overflow.");
	}
	m_intval -= implicit_divisor();
	assert (m_places >= benchmark_places);
	return *this;
}




Decimal& Decimal::operator+=(Decimal rhs)
{
	#ifndef NDEBUG
		unsigned short const benchmark_places = max(m_places, rhs.m_places);
	#endif
	co_normalize(*this, rhs);
	if (CheckedArithmetic::addition_is_unsafe(m_intval, rhs.m_intval))
	{
		throw UnsafeArithmeticException("Addition may cause overflow.");
	}
	m_intval += rhs.m_intval;
	assert (m_places >= benchmark_places);
	return *this;
}



Decimal& Decimal::operator-=(Decimal rhs)
{
	#ifndef NDEBUG
		unsigned short const benchmark_places = max(m_places, rhs.m_places);
	#endif
	co_normalize(*this, rhs);
	if (CheckedArithmetic::subtraction_is_unsafe(m_intval, rhs.m_intval))
	{
		throw UnsafeArithmeticException("Subtraction may cause "
		  "overflow.");
	}
	m_intval -= rhs.m_intval;
	assert (m_places >= benchmark_places);
	return *this;
}

Decimal& Decimal::operator*=(Decimal rhs)
{
	// There is a fair bit of poking and prodding here to make sure
	// we don't overflow. Some of this is probably redundant.

	// Preserve member variables in case of need to reset
	// them later.
	int_type const original_intval = m_intval;
	unsigned short const original_places = m_places;

	unsigned short const rh_places = rhs.m_places;

	// Remember required sign of product
	bool const diff_signs = (( m_intval > 0 && rhs.m_intval < 0 ) ||
	                         ( m_intval < 0 && rhs.m_intval > 0 ));
	
	// Irritating but best to prevent complications
	if ( (m_intval == numeric_limits<int_type>::min()) ||
	     (rhs.m_intval == numeric_limits<int_type>::min()) )
	{
		throw UnsafeArithmeticException("Cannot safely perform multiplication"
		  " on the smallest possible Decimal.");
	}

	// Make absolute
	if (m_intval < 0) m_intval *= -1;
	if (rhs.m_intval < 0) rhs.m_intval *= -1;

	// Multiply the underlying integers of the multiplicands. Hold in
	// a long double for now, to avoid needless overflow.
	long double proxy_intval = NUM_CAST<long double>(m_intval) *
	  NUM_CAST<long double>(rhs.m_intval);

	// Now test to reduce the level of precision that we are targeting, until
	// we can safely fit proxy_intval into int_val. We record our "level
	// of precision reduction" in places_to_lose.
	long double max_as_long_double = numeric_limits<int_type>::max();
	unsigned short places_to_lose = 0;
	while (proxy_intval > max_as_long_double)
	{
		proxy_intval /= NUM_CAST<long double>(BASE);
		++places_to_lose;
	}
	m_intval = NUM_CAST<int_type>(proxy_intval);
	
	// Now add the places of the multiplicands together.
	m_places += rh_places;

	// If we can do so safely, reduce m_places according to the level
	// of precision-reduction calculated above. Otherwise reset and throw.
	if (m_places < places_to_lose)
	{
		m_intval = original_intval;
		m_places = original_places;
		throw UnsafeArithmeticException("Unsafe multiplication.");
	}
	m_places -= places_to_lose;

	// Make negative if required.
	if (diff_signs)
	{
		m_intval *= -1;
	}

	// Ensure we don't exceed MAX_PLACES.
	if (m_places > MAX_PLACES)
	{
		if (set_fractional_precision(MAX_PLACES) != 0)
		{
			throw (UnsafeArithmeticException("Unsafe multiplication."));
		}
	}

	rationalize();
	return *this;
}


Decimal& Decimal::operator/=(Decimal rhs)
{
	Decimal const orig = *this;
	if (rhs.m_intval == 0)
	{
		throw (UnsafeArithmeticException("Division by zero"));
	}
	int_type const orig_intval = m_intval;
	unsigned short const orig_places = m_places;
	rhs.rationalize();
	while (set_fractional_precision(m_places + 1) == 0)
	{
	}
	while ( (m_intval < rhs.m_intval) && (rhs.m_places > 0) )
	{
		rhs.set_fractional_precision(rhs.m_places - 1);
	}
	if (rhs.m_places > m_places)
	{
		m_intval = orig_intval;
		m_places = orig_places;
		throw (UnsafeArithmeticException("Unsafe division."));
	}
	m_intval /= rhs.m_intval;
	m_places -= rhs.m_places;
	
	rationalize();
	return *this;

	/*
	// Get both operands on the same footing
	// Note, if we can't co_normalize safely, then we can't divide safely.
	// Note, co_normalize throws if unsafe.
	co_normalize(*this, rhs);

	// The meat of the calculation
	long double doub_new_val = NUM_CAST<long double>(m_intval) /
						       NUM_CAST<long double>(rhs.m_intval) *
	                           NUM_CAST<long double>(implicit_divisor());

	// Now test to reduce the level of precision that we are targeting, until
	// we can safely fit proxy_intval into int_val. We record our "level
	// of precision reduction" in places_to_lose.
	long double max_as_long_double = numeric_limits<int_type>::max();
	unsigned short places_to_lose = 0;
	while (doub_new_val + NUM_CAST<long double>(1)
	       > max_as_long_double)   // The "+1" protects
	                               // for int_type MIN...
	{
		doub_new_val /= NUM_CAST<long double>(BASE);
		++places_to_lose;
	}

	// Remove places if we can, otherwise throw.
	if (places_to_lose > m_places)
	{
		throw (UnsafeArithmeticException("Unsafe division."));
	}
	m_places -= places_to_lose;

	// Now get *this as precise as we safely can
	m_intval = doub_new_val + 1;  // conservative initial value
	                              // (which we revise later)
	while (true)
	{
		#ifndef NDEBUG
			unsigned short DEBUGVARIABLE_max_known_safe = m_places;
		#endif
		if (set_fractional_precision(m_places + 1) != 0)
		{
			// then it wasn't safe...
			assert (m_places == DEBUGVARIABLE_max_known_safe);
			m_intval = NUM_CAST<int_type>(doub_new_val);
			break;
		}
		doub_new_val *= NUM_CAST<double>(BASE);
	}
	
	rationalize();

	return *this;
	*/
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
	assert ( (m_intval < 0 && rhs.m_intval < 0) ||
	         (m_intval > 0 && rhs.m_intval > 0) );
	
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
	string& smallerstring = ( leftstr.size() < rightstr.size() ?
	                          leftstr : rightstr );
	size_t const greatersize = ( leftstr.size() < rightstr.size() ?
	                             rightstr.size() : leftstr.size() );
	
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
	return ( temp_lhs.m_intval == rhs.m_intval &&
	         temp_lhs.m_places == rhs.m_places );
}


Decimal round(Decimal const& x, unsigned int decimal_places)
{
	Decimal ret = x;
	if (ret.set_fractional_precision(decimal_places) != 0)
	{	
		throw (UnsafeArithmeticException("Decimal number cannot "
		  "safely be rounded to this number of places."));
	}
	return ret;
}


Decimal operator-(Decimal const& d)
{
	if (d.m_intval == numeric_limits<Decimal::int_type>::min())
	{
		throw (UnsafeArithmeticException("Unsafe arithmetic "
		  "operation (unary minus)."));
	}
	assert (d.m_intval != numeric_limits<Decimal::int_type>::min());
	Decimal ret = d;
	ret.m_intval *= -1;
	return ret;
}

// I could also implement this as simple wrapper for a private static
// const member variable, defined as equal to the value of a minimum_aux()
// function called only once initialization work. This might be a bit
// more efficient for callers; but I don't think it's worth the extra effort.
// Same goes for Decimal::maximum().
Decimal Decimal::minimum()
{
	static bool calculated_already;
	static Decimal ret;
	if (calculated_already)
	{
		return ret;
	}
	assert (!calculated_already);
	ostringstream oss;
	oss << numeric_limits<int_type>::min();
	ret = Decimal(oss.str());
	return ret;
}

Decimal Decimal::maximum()
{
	static bool calculated_already;
	static Decimal ret;
	if (calculated_already)
	{
		return ret;
	}
	assert (!calculated_already);
	ostringstream oss;
	oss << numeric_limits<int_type>::max();
	ret = Decimal(oss.str());
	return ret;
}


}  // namespace jewel
