/*
 * This file is part of the Jewel project and is distributed under the
 * terms of the License contained in the file LICENSE.txt distributed
 * with this package.
 * 
 * Author: Matthew Harvey <matthew@matthewharvey.net>
 *
 * Copyright (c) 2012-2013, Matthew Harvey.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */


#include "num_digits.hpp"
#include "assert.hpp"

namespace jewel
{

std::size_t
NumDigits::num_digits(int x, int base)
{
	JEWEL_ASSERT (base > 1);
	return num_digits_aux(x, base);
}

std::size_t
NumDigits::num_digits(short x, short base)
{
	JEWEL_ASSERT (base > 1);
	return num_digits_aux(x, base);
}

std::size_t
NumDigits::num_digits(long x, long base)
{
	JEWEL_ASSERT (base > 1);
	return num_digits_aux(x, base);
}

std::size_t
NumDigits::num_digits(long long x, long long base)
{
	JEWEL_ASSERT (base > 1);
	return num_digits_aux(x, base);
}

std::size_t
NumDigits::num_digits(signed char x, signed char base)
{
	JEWEL_ASSERT (base > 1);
	return num_digits_aux(x, base);
}

std::size_t
NumDigits::num_digits(unsigned int x, unsigned int base)
{
	JEWEL_ASSERT (base > 1);
	return num_digits_aux(x, base);
}

std::size_t
NumDigits::num_digits(unsigned short x, unsigned short base)
{
	JEWEL_ASSERT (base > 1);
	return num_digits_aux(x, base);
}

std::size_t
NumDigits::num_digits(unsigned long x, unsigned long base)
{
	JEWEL_ASSERT (base > 1);
	return num_digits_aux(x, base);
}

std::size_t
NumDigits::num_digits(unsigned long long x, unsigned long long base)
{
	JEWEL_ASSERT (base > 1);
	return num_digits_aux(x, base);
}

std::size_t
NumDigits::num_digits(unsigned char x, unsigned char base)
{
	JEWEL_ASSERT (base > 1);
	return num_digits_aux(x, base);
}


}  // namespace jewel
