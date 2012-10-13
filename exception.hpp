#ifndef GUARD_exception_hpp
#define GUARD_exception_hpp

#include <stdexcept>

/** @file exception.hpp
 *
 * @brief Exception related utilities.
 */



namespace jewel
{

/**
 * This is an exception class that inherits virtually from
 * \c std::exception. It is intended in turn to be the base class
 * for further custom exception classes.
 * Instances of \c jewel::Exception
 * can be initialized with either a \c char \c const* \c (representing
 * an error message), or with no parameters. The char const*, if lengthier
 * than the length returned by the static member function
 * \c max_message_size(), will be truncated to that maximum length.
 * This mechanism is used to avoid the
 * dynamic memory allocation associated with \c std::string. Since dynamic
 * memory allocation could fail, it is arguably a bad idea to store the
 * error message associated with an exception in a \c std::string.
 * Note that instances of this class will store a copy of the
 * entire error message passed to the constructor, not just a pointer thereto.
 *
 * Exception safety: all of the functions in this class offer
 * the <em>nothrow guarantee</em>.
 * 
 * @todo HIGH PRIORIY Test exact point of truncation.
 */
class Exception: public virtual std::exception
{
	Exception const operator=(Exception const&);  // unimplemented
	static size_t const s_message_buffer_size = 200;
	static char const* s_truncation_flag;
	char m_message[s_message_buffer_size];
public:
	Exception() throw();
	explicit Exception(char const* p_message) throw();
	virtual ~Exception() throw();
	virtual char const* what() const throw();
	static size_t max_message_size() throw();
	Exception(Exception const& rhs) throw();
};


}  // namespace jewel



/**
 * Macro to declare and define an exception class called \c DERIVED_CLASS,
 * inheriting from \c BASE_CLASS. The \c BASE_CLASS must have a constructor
 * that takes a \c char \c const*, representing an error message.
 * 
 * \c DERIVED_CLASS will have both a default constructor, and a constructor
 * that takes a \c char \c const*, representing an error message.
 *
 * Note the macro definition omits a trailing semi-colon. A semi-colon should
 * always be appended when the macro is invoked.
 */
#define JEWEL_DERIVED_EXCEPTION(DERIVED_CLASS, BASE_CLASS)\
	class DERIVED_CLASS: public BASE_CLASS\
	{\
	public:\
		DERIVED_CLASS() throw()\
		{\
		}\
		explicit DERIVED_CLASS(char const* p_message) throw():\
			BASE_CLASS(p_message)\
		{\
		}\
		virtual ~DERIVED_CLASS() throw()\
		{\
		}\
	}\



#endif  // GUARD_exception_hpp
