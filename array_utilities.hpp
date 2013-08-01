#ifndef GUARD_array_utilities_hpp
#define GUARD_array_utilities_hpp

#include <cstddef>

/** @file array_utilities.hpp
 *
 * @brief Utility functions for working with built-in arrays.
 *
 * @author Matthew Harvey
 * @date 01 August 2013
 *
 * Copyright (c) 2013, Matthew Harvey. All rights reserved.
 */

namespace jewel
{

// DECLARATIONS

/**
 * @returns the number of elements in array \e p_array.
 *
 * Does not throw.
 *
 * @todo Test.
 */
template <typename Elem, std::size_t sz>
std::size_t size(Elem const (&p_array)[sz]);

/**
 * @returns a pointer the first element of the array \e p_array.
 *
 * Does not throw.
 *
 * @todo Test.
 */
template <typename Elem, std::size_t sz>
Elem* begin(Elem (&p_array)[sz]);

/**
 * @returns a pointer-to-constant to the first element of the array
 * \e p_array.
 *
 * Does not throw.
 *
 * @todo Test.
 */
template <typename Elem, std::size_t sz>
Elem const* begin(Elem const (&p_array)[sz]);

/**
 * @returns a pointer-to-constant to one-past-the-end of the
 * array.
 */
template <typename Elem, std::size_t sz>
Elem const* end(Elem const (&p_array)[sz]);



// IMPLEMENTATIONS

template <typename Elem, std::size_t sz>
inline
std::size_t
size(Elem const (&p_array)[sz])
{
	(void)p_array;  // silence compiler re. unused parameter
	return sz;
}

template <typename Elem, std::size_t sz>
inline
Elem*
begin(Elem (&p_array)[sz])
{
	return p_array;
}

template <typename Elem, std::size_t sz>
inline
Elem const*
begin(Elem const (&p_array)[sz])
{
	return p_array;
}

template <typename Elem, std::size_t sz>
inline
Elem const*
end(Elem const (&p_array)[sz])
{
	return p_array + sz;
}


}  // namespace jewel

#endif  // GUARD_array_utilities_hpp
