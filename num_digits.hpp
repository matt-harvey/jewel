#ifndef GUARD_num_digits_hpp
#define GUARD_num_digits_hpp

/** \file num_digits.hpp
 *
 * \brief Facility to count the number of digits in a number of integral type 
 *
 * \author Matthew Harvey
 * \date 2012-05-13
 * 
 * Copyright (c) 2012, Matthew Harvey. All rights reserved.
 */


#include <cstddef>


namespace jewel
{

/**\class NumDigits
 * \brief Wrapper class for static num_digits function.
 *
 * Only the parameter types specifically provided for in the overloads can be
 * passed to the num_digits function. Passing other types will cause
 * compilation failure, rather than conversion to the allowed types.
 */
class NumDigits
{
public:
	/**Counts the number of digits in a given number of integral type.
	 *
	 * (The negative sign is not counted as a digit.)
	 *@param x the number whose digits we want to count
	 *@param base the numerical base of representation of x (defaults to 10)
	 *
	 * Exception safety: the <em>nothrow guarantee</em> is offered by
	 * all the public functions in NumDigits.
	 */
	static std::size_t num_digits(int x, int base = 10);
	static std::size_t num_digits(short x, short base = 10);
	static std::size_t num_digits(long x, long base = 10);
	static std::size_t num_digits(long long x, long long base = 10);
	static std::size_t num_digits(signed char x, signed char base = 10);
	static std::size_t num_digits(unsigned int x, unsigned int base = 10);
	static std::size_t num_digits(unsigned short x, unsigned short base = 10);
	static std::size_t num_digits(unsigned long x, unsigned long base = 10);
	static std::size_t num_digits
	(	unsigned long long x,
		unsigned long long base = 10
	);
	static std::size_t num_digits(unsigned char x, unsigned char base = 10);
private:
	
	// Constructor is private and unimplemented, to prevent this class
	// from being instantiated.
	NumDigits();

	// Capture parameters of disallowed type in this unimplemented function
	// template, to cause compile-time failure. Only the types explicitly
	// allowed for in the public overloaded functions will work; and moreover,
	// disallowed types will _not_ be automatically converted to allowed types.
	template <typename T>
	static std::size_t num_digits(T x, T base = 10);

	// Counts the number of digits in a number of integral type.
	// Called by the public functions, after converting x to absolute value
	// where required.
	template <typename T>
	static std::size_t num_digits_aux(T x, T base);

};


// FUNCTION TEMPLATE IMPLEMENTATIONS

template <typename T>
inline
std::size_t NumDigits::num_digits_aux(T x, T base)
{
	if (x == 0) return 1;
	std::size_t digits = 0;
	for (T i = x; i != 0; i /= base) ++digits;
	return digits;
}


}  // namespace jewel



#endif  // GUARD_utilities_hpp
