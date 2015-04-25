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

#ifndef GUARD_num_digits_hpp_48970041167442707
#define GUARD_num_digits_hpp_48970041167442707

/** @file
 *
 * @brief Facility to count the number of digits in a number of integral type 
 */


#include <cstddef>


namespace jewel
{

/**
 * @brief Wrapper class for static num_digits function.
 *
 * Only the parameter types specifically provided for in the overloads can be
 * passed to the num_digits function. Passing other types will cause
 * compilation failure, rather than conversion to the allowed types.
 */
class NumDigits
{
public:

    //@{
    /**
     * Counts the number of digits in a given number of integral type.
     *
     * (The negative sign is not counted as a digit.)
     *@param x the number whose digits we want to count
     *@param base the numerical base of representation of x (defaults to 10)
     *
     * Precondition: Base must be greater than 1.
     *
     * Exception safety: the <em>nothrow guarantee</em>.
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
    (   unsigned long long x,
        unsigned long long base = 10
    );
    static std::size_t num_digits(unsigned char x, unsigned char base = 10);
    //@}

private:
    
    // Constructor is private and unimplemented, to prevent this class
    // from being instantiated.
    NumDigits();

    // Capture parameters of disallowed type in this unimplemented function
    // template, to cause compile-time failure. Only the types explicitly
    // allowed for in the public overloaded functions will work; and moreover,
    // disallowed types will _not_ be automatically converted to allowed
    // types.
    template <typename T>
    static std::size_t num_digits(T x, T base = 10);

    // Counts the number of digits in a number of integral type.
    // Called by the public functions, after converting x to absolute value
    // where required.
    template <typename T>
    static std::size_t num_digits_aux(T x, T base);

};


// FUNCTION TEMPLATE IMPLEMENTATIONS

/// @cond
template <typename T>
inline
std::size_t NumDigits::num_digits_aux(T x, T base)
{
    if (x == 0) return 1;
    std::size_t digits = 0;
    for (T i = x; i != 0; i /= base) ++digits;
    return digits;
}
/// @endcond

}  // namespace jewel



#endif  // GUARD_utilities_hpp_48970041167442707
