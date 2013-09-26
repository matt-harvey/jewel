#ifndef GUARD_stopwatch_hpp_021388252275711044
#define GUARD_stopwatch_hpp_021388252275711044

/** \file stopwatch.hpp
 *
 * \brief Stopwatch class for timing intervals
 *
 * \author Matthew Harvey
 * \date 13 May 2012
 *
 * Copyright (c) 2012, Matthew Harvey. All rights reserved.
 */



#include <ctime>
#include <iomanip>
#include <ios>
#include <iostream>
#include <string>

namespace jewel
{

/** Stopwatch class for timing intervals
 *
 * Use this class to create a Stopwatch, then later call its log() method
 * or its seconds_elapsed() method to find the number of seconds elapsed since
 * the creation of the Stopwatch.
 *
 * @todo HIGH PRIORITY For very long timings (longer than a few seconds) this
 * seems to be failing.
 * 
 * Exception safety: Functions in this class do not offer any particular
 * exception safety guarantees.
 */
class Stopwatch
{
public:
	/**
	 * The Stopwatch will start counting immediately upon creation
	 */
	Stopwatch();

	Stopwatch(Stopwatch const&) = delete;
	Stopwatch(Stopwatch&&) = delete;
	Stopwatch& operator=(Stopwatch const&) = delete;
	Stopwatch& operator=(Stopwatch&&) = delete;
	~Stopwatch() = default;

	/** Write a message to an output stream re. the number of seconds elapsed.
	 *
	 * The message is in the form: \n <tt> n seconds elapsed. </tt>
	 *
	 * The message ends in a newline.
	 * @param os stream to write message to (std::clog by default).
	 * @return the same stream, by reference.
	 */
	std::ostream& log(std::ostream& os = std::clog) const;

	/**
	 * @return the number of seconds elapsed since the Stopwatch was created.
	 */
	double seconds_elapsed() const;
private:

	std::clock_t m_start_time;
};

// IMPLEMENTATION OF INLINE FUNCTIONS

inline
Stopwatch::Stopwatch():
 	m_start_time(std::clock())
{
}

inline
double Stopwatch::seconds_elapsed() const
{
	return double(std::clock() - m_start_time) / double(CLOCKS_PER_SEC);
}

inline
std::ostream& Stopwatch::log(std::ostream& os) const
{
	std::streamsize const prec = os.precision();
	os << std::setprecision(6) << seconds_elapsed() << " seconds elapsed."
	   << std::setprecision(prec) << std::endl;
	return os;
}

	



}  // namespace jewel

#endif  // GUARD_stopwatch_hpp_021388252275711044
