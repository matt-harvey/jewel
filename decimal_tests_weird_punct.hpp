#ifndef GUARD_decimal_tests_weird_punct_hpp
#define GUARD_decimal_tests_weird_punct_hpp

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


#endif  // GUARD_decimal_tests_weird_punct_hpp
