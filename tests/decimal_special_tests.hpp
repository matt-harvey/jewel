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


#ifndef GUARD_decimal_special_tests_hpp_4550346198748027
#define GUARD_decimal_special_tests_hpp_4550346198748027


/**
 * This file contains special testing functions (not managed by UnitTest++),
 * which should only be invoked from main.cpp.
 */

namespace jewel
{

namespace detail
{

/** Speed test for arithmetic operations using Decimal class
 * Outputs to std::cout results of test.
 */
void decimal_speed_test();

/** Helper function used in decimal_csv_test.
 */
bool is_dagger(char c);

/** Test processing of many Decimals from a csv file. These are two
 * digit decimals typical of accounting data, in two delimited columns,
 * which should sum to zero.
 */
void decimal_csv_test();



}  // namespace detail

}  // namespace jewel



#endif  // GUARD_decimal_special_tests_hpp_4550346198748027
