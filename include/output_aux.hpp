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

#ifndef GUARD_output_aux_hpp_10487737604644028
#define GUARD_output_aux_hpp_10487737604644028


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

#endif  // GUARD_output_aux_hpp_10487737604644028
