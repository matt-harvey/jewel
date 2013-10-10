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


#include "optional.hpp"
#include <UnitTest++/UnitTest++.h>
#include <string>
#include <vector>

using boost::optional;
using jewel::UninitializedOptionalException;
using jewel::clear;
using jewel::value;
using std::string;
using std::vector;

// @cond (to hide this from Doxygen)

class Dummy
{
public:
	Dummy():
		m_a(0),
		m_b("")
	{
	}

	Dummy(int p_a, string const& p_b):
		m_a(p_a),
		m_b(p_b)
	{
	}
	
	bool
	operator==(Dummy const& rhs) const
	{
		return m_a == rhs.m_a && m_b == rhs.m_b && m_c == rhs.m_c;
	}

private:
	int m_a;
	string m_b;
	vector<string> m_c;	
};
// @endcond

TEST(value)
{
	optional<int> x = 3;
	CHECK_EQUAL(*x, 3);
	CHECK_EQUAL(value(x), 3);
	CHECK_EQUAL(value(x), *x);

	optional<int> y;
	CHECK_THROW(value(y), UninitializedOptionalException);

	string const sentence("There's a whole sentence in here!");
	optional<string> s(sentence);
	CHECK_EQUAL(*s, sentence);
	CHECK_EQUAL(value(s), sentence);
	CHECK_EQUAL(*s, value(s));

	optional<string> j;
	CHECK_THROW(value(j), UninitializedOptionalException);

	optional<Dummy> d;
	Dummy e;
	CHECK_THROW(e = value(d), UninitializedOptionalException);
	
	optional<Dummy> d2(Dummy(3, "Hello"));
	CHECK(*d2 == value(d2));
}

TEST(clear)
{
	optional<int> x = 3;
	CHECK_EQUAL(*x, 3);
	CHECK_EQUAL(value(x), 3);
	clear(x);
	CHECK_THROW(value(x), UninitializedOptionalException);
	x = 4;
	CHECK_EQUAL(value(x), 4);

	string const sentence("Hello.");
	optional<string> s(sentence);
	CHECK_EQUAL(value(s), "Hello.");
	clear(s);
	CHECK_THROW(value(s), UninitializedOptionalException);
	s = "Yes";
	CHECK_EQUAL(value(s), "Yes");

	optional<Dummy> d(Dummy(3, "Yo!"));
	CHECK(*d == value(d));
	clear(d);
	CHECK_THROW(value(d), UninitializedOptionalException);
}


