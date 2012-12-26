#ifndef GUARD_output_aux_hpp
#define GUARD_output_aux_hpp

#include <ostream>
#include <sstream>

namespace jewel
{

/**
 * @todo HIGH PRIORITY Documentation and testing.
 */
template <typename charT, typename traits, typename T>
void output_aux
(	std::basic_ostream<charT, traits>& os,
	T const& x,
	void (*do_output)(std::basic_ostream<charT, traits>&, T const&)
)
{
	if (!os)
	{
		return;
	}
	try
	{
		std::basic_ostringstream<charT, traits> ss;
		ss.exceptions(os.exceptions());
		ss.imbue(os.getloc());
		do_output(os, x);
		if (!ss)
		{
			os.setstate(ss.rdstate());
			return;
		}
		os << ss.str();
	}
	catch (...)
	{
		os.setstate(std::ios_base::badbit);
	}
	return;
}



}  // namespace jewel

#endif  // GUARD_output_aux_hpp
