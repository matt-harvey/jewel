#ifndef GUARD_exception_macros_hpp
#define GUARD_exception_macros_hpp

#include <boost/exception/exception.hpp>
#include <stdexcept>
#include <string>

/** @file exception_macros.hpp
 *
 * @brief Provides macros to ease the declaration and definition of
 * exception classes.
 *
 * The boilerplate involved in creating exception
 * classes can discourage their creation. While macros are generally to
 * be avoided, having a reasonably fine-grained structure of exceptions
 * is a good thing - which unfortunately is discouraged by the amount
 * of boilerplate involved in creating such a structure. These macros
 * aim to vastly reduce the repetitive boilerplate required to create
 * a hierarchy of exception classes.
 *
 * Note this file \c #includes the following headers:\n
 * 	\c <boost/exception/exception.hpp>\n
 * 	\c <stdexcept>\n
 * 	\c <string>\n
 */


/**
 * Macro to declare and define an exception class named \c CLASS, that
 * inherits virtually from both \c boost::exception and
 * from \c std::exception. Instances of the newly created exception class,
 * can be initialized with either a \c std::string \c const (representing
 * an error message), or with no parameters.
 *
 * Note the macro definition omits a trailing semi-colon. A semi-colon should
 * always be appended when the macro is invoked.
 */
#define JEWEL_STANDARD_EXCEPTION(CLASS)\
	class CLASS: public boost::exception, public std::exception\
	{\
	public:\
		CLASS() throw()\
		{\
		}\
		explicit CLASS(std::string const& p_message):\
			m_message(p_message)\
		{\
		}\
		virtual ~CLASS() throw()\
		{\
		}\
		virtual const char* what() const throw()\
		{\
			return m_message.c_str();\
		}\
	protected:\
		std::string m_message;\
	}\


/**
 * Macro to declare and define an exception class called \c DERIVED_CLASS,
 * inheriting from \c BASE_CLASS. The \c BASE_CLASS must have a constructor
 * that takes a \c std::string \c const&, representing an error message.
 * 
 * \c DERIVED_CLASS will have both a default constructor, and a constructor
 * that takes a \c std::string \c const& , representing an error message.
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
		explicit DERIVED_CLASS(std::string const& p_message):\
			BASE_CLASS(p_message)\
		{\
		}\
		virtual ~DERIVED_CLASS() throw()\
		{\
		}\
	}\



#endif  // GUARD_exception_macros_hpp
