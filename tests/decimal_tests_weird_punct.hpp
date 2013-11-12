/*
 * Copyright 2013 Matthew Harvey
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
