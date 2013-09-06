#ifndef GUARD_tolerant_string_hpp
#define GUARD_tolerant_string_hpp

#include <cstddef>
#include <cstdlib>
#include <ios>

namespace jewel
{

/**
 * Represents a dynamically allocated string of char, which is similar to
 * std::string except that:\n
 * (a) It lacks many of the functions and conveniences of std::string; and\n
 * (b) Its member functions never throw exceptions. In particular,
 * \e std::bad_alloc is never thrown. Rather, if there is a failure of
 * memory allocation during construction, assignment or etc.., the
 * TolerantString is marked as invalid, and its contents become equivalent
 * to an empty string. The validity of a TolerantString can
 * be queried via the \e is_valid() member function. This is useful in
 * cases where we want a string class that cannot possibly throw, for
 * example as the member variable of an exception class representing
 * an error message.
 */
class TolerantString
{
public:
	typedef char const* const_iterator;
	typedef char* iterator;
	typedef std::size_t size_type;
	typedef std::ptrdiff_t difference_type;
	typedef char value_type;

	TolerantString();
	TolerantString(char const* p_string);

	TolerantString(TolerantString const& rhs);
	~TolerantString();
	TolerantString& operator=(TolerantString const& rhs);

	/**
	 * Two TolerantString compare as equal if and only if they have the
	 * same length with the same characters in the same order. If neither
	 * string is valid, then they will always compare equal. If one string is
	 * valid and the other is not, then they will never compare as equal,
	 * even if their contents are the same (the contents of an invalid
	 * string are always the empty string).
	 */
	bool operator==(TolerantString const& rhs) const;
	bool operator!=(TolerantString const& rhs) const;

	/**
	 * Similar to std::string::c_str(), with similar caveats. This
	 * returns an empty string if the TolerantString is invalid.
	 */
	char const* c_str() const;

	size_type size() const;

	bool empty() const;

	/**
	 * TolerantString may undergo memory allocation failure internally.
	 * If this happens, then rather than std::bad_alloc being thrown,
	 * the TolerantString simply marked as invalid. The validity of
	 * the TolerantString can be tested with the is_valid() member function.
	 */
	bool is_valid() const;

	void clear();

	void swap(TolerantString& rhs);

private:
	void initialize_from_c_string(char const* p_string);

	bool m_is_valid;
	size_type m_len;
	char* m_data;
	char m_standby[1];

};  // class TolerantString


// NON MEMBER FUNCTIONS

/**
 * Write to an output stream.
 */
template <typename traits>
std::basic_ostream<TolerantString::value_type, traits>&
operator<<
(	std::basic_ostream<TolerantString::value_type, traits>& p_os,
	TolerantString const& p_str
);

/**
 * TODO Provide stream input operator.
 */


// IMPLEMENT INLINE FUNCTIONS AND FUNCTION TEMPLATES

template <typename traits>
std::basic_ostream<TolerantString::value_type, traits>&
operator<<
(	std::basic_ostream<TolerantString::value_type, traits>& p_os,
	TolerantString const& p_str
)
{
	if (!p_str.is_valid())
	{
		p_os.setstate(std::ios_base::failbit);
		return p_os;
	}
	return p_os << p_str.c_str();
}


}  // namespace jewel

#endif  // GUARD_tolerant_string_hpp
