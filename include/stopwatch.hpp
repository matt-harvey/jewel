/*
 * Copyright 2012-2013 Matthew Harvey
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

#ifndef GUARD_stopwatch_hpp_021388252275711044
#define GUARD_stopwatch_hpp_021388252275711044

/** \file stopwatch.hpp
 *
 * \brief Stopwatch class for timing intervals
 */

#include <ctime>
#include <iomanip>
#include <ios>
#include <iostream>
#include <string>

namespace jewel
{

/** Stopwatch class for timing intervals in CPU time.
 *
 * Use this class to create a Stopwatch, then later call its log() method
 * or its seconds_elapsed() method to find the number of seconds elapsed in
 * CPU time since the creation of the Stopwatch.
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

	/** Write a message to an output stream re. the number of seconds of CPU
	 * time elapsed on the Stopwatch.
	 *
	 * The message is in the form: \n <tt> n seconds elapsed. </tt>
	 *
	 * The message ends in a newline.
	 * @param os stream to write message to (std::clog by default).
	 * @return the same stream, by reference.
	 */
	std::ostream& log(std::ostream& os = std::clog) const;

	/**
	 * @return the number of seconds CPU time elapsed since the Stopwatch was
	 * created, or returns -1 if this can't be calculated.
	 */
	long double seconds_elapsed() const;
private:

	std::clock_t const m_start_time;
};

// IMPLEMENTATION OF INLINE FUNCTIONS

inline
Stopwatch::Stopwatch(): m_start_time(std::clock())
{
}

inline
long double Stopwatch::seconds_elapsed() const
{

	if (m_start_time == -1)
	{
		return -1;
	}
	return
		static_cast<long double>(std::clock() - m_start_time) /
		static_cast<long double>(CLOCKS_PER_SEC);
}

inline
std::ostream& Stopwatch::log(std::ostream& os) const
{
	std::streamsize const prec = os.precision();
	os << std::setprecision(6) << seconds_elapsed()
	   << " seconds CPU time."
	   << std::setprecision(prec) << std::endl;
	return os;
}

	



}  // namespace jewel

#endif  // GUARD_stopwatch_hpp_021388252275711044
