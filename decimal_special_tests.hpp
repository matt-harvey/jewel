#ifndef GUARD_decimal_special_tests_hpp
#define GUARD_decimal_special_tests_hpp


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



#endif  // GUARD_decimal_special_tests_hpp
