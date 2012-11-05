#include "optional.hpp"
#include <unittest++/UnitTest++.h>
#include <string>
#include <vector>

using boost::optional;
using jewel::UninitializedOptionalException;
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

