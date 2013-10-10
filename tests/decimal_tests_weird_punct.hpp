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


#ifndef GUARD_decimal_tests_weird_punct_hpp_7668562358090916
#define GUARD_decimal_tests_weird_punct_hpp_7668562358090916

#include <locale>
#include <string>

namespace weird_punct
{


// @cond
// Class to test robustness of Decimal output
// in respecting even bizarre numpunct facets
class WeirdPunct:
	public std::numpunct_byname<char>
{
public:
	WeirdPunct(char const* name):
		std::numpunct_byname<char>(name)
	{
	}
protected:
	virtual std::string do_grouping() const
	{
		return "\1\2\3";
	}
	virtual char do_decimal_point() const
	{
		return '^';
	}
	virtual char do_thousands_sep() const
	{
		return 'w';
	}
};
// @endcond


}  // namespace weird_punct


#endif  // GUARD_decimal_tests_weird_punct_hpp_7668562358090916
