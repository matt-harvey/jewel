#ifndef GUARD_power_vector_hpp
#define GUARD_power_vector_hpp


/** @file power_vector.hpp
 *
 * @brief Provides function template to return the "power vector"
 * of a std::vector<T>
 *
 * @author Matthew Harvey
 * @date 22 May 2012
 *
 * Copyright (c) 2012, Matthew Harvey. All rights reserved.
 *
 * I chose not to implement this for sets as the necessity
 * for an ordering operation makes it tricky for sets.
 *
 * This function does the following:
 * If we have a std::vector<T> v, then the function will
 * return a std::vector< std::vector<T> >, containing every
 * possible sub-vector of v (analogous to the concept of a
 * subset) THAT PRESERVES THE ORIGINAL ELEMENT ORDER.
 *
 * As far as the ordering of elements is concerned, we
 * can think of the original vector as a binary
 * number, of which the vector's elements represent
 * places, going from most to least significant
 * digits as we read from left to right... The subvectors
 * of this original vector that are included in the power vector
 * are determined by counting from lowest to highest in the series
 * of binary numbers from 0 to the size of the power vector. Each
 * "1" represents an element to be included in one of the subvectors.
 * 
 * This function is not particularly efficient as it returns
 * the std::vector by value.
 * 
 * @throws std::bad_alloc in the unlikely event of memory allocation
 * failure while building the returned vector.
 *
 * Exception safety: <em>strong guarantee</em>.
 */



#include "assert.hpp"
#include <cmath>
#include <iostream>
#include <vector>

namespace jewel
{


// For some reason Doxygen isn't detecting the below comment. That's why
// I've put the description of the function into the file comment.

/** Like a power set function, but for vectors.
 * See file documentation for more detail.
 */
template <typename T> std::vector< std::vector<T> >
power_vector(std::vector<T> const& v)
{
	// typedefs
	typedef typename std::vector<T> v_type;
	typedef typename std::vector<v_type> p_type;
	typedef typename v_type::size_type v_size_type;
	typedef typename p_type::size_type p_size_type;

	// size of vector and size of power vector - we know these already
	const v_size_type v_size = v.size();
	const double temp1(2);
	const double temp2(v_size); 
	const p_size_type p_size = std::pow(temp1, temp2);
	
	// declare the power vector
	p_type ret;

	// populate the power vector one member at a time
	// by iterating through from 0 to p_size
	p_size_type subvec_num = 0;
	while (subvec_num != p_size)
	{
		// subvec_num now determines a unique subvector of v, which
		// we build and then push onto ret.
		// (Think of subvec_num in binary form, with each 1 or 0
		// determining
		// whether the corresponding member of v is in
		// this element of the powervector (ret_elem).)
		// Note we count backwards through the subvectors, to ensure
		// expected ordering is preserved per the original vector
		v_type ret_elem;
		v_size_type vindex = 0;
		p_size_type i = subvec_num;
		p_size_type j = p_size / 2;
		while (j != 0)
		{
			if (i / j)
			{
				JEWEL_ASSERT (i / j == 1);
				ret_elem.push_back(v[vindex]);	
				i -= j;
			}
			j /= 2;
			++vindex;
		}

		ret.push_back(ret_elem);
		++subvec_num;
	}

	JEWEL_ASSERT (ret.size() == p_size);
	return ret;
}



} // namespace jewel

#endif
