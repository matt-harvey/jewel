#ifndef GUARD_exception_macros_hpp
#define GUARD_exception_macros_hpp

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
 * @todo HIGH PRIORITY
 * Make CLASS also be default constructible, so that it will work with the
 * Boost exception framework. Also make CLASS inherit from boost::exception
 * as well as from std::exception. Specify in the documentation that the
 * inheritance is virtual. Also, consider including headers <stdexcept>,
 * <string> and <boost/exceptions/all.hpp> in this file. (This would make for
 * greater convenience but also less transparency.)
 *
 */


/**
 * Macro to declare and define an exception class named \c CLASS, inheriting
 * from std::exception. Instances of the newly created exception class, unlike
 * std::exception, should be initialized with a std::string, representing
 * an error message.
 *
 * The macro should be invoked only in a location where declarations for
 * std::string and std::exception are available (typically in a location that
 * has included <string> and <stdexcept>).
 *
 * Note the macro definition omits a trailing semi-colon. A semi-colon should
 * always be appended when the macro is invoked.
 */
#define JEWEL_STANDARD_EXCEPTION(CLASS)\
	class CLASS: public std::exception\
	{\
	public:\
		explicit CLASS(std::string p_message):\
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
 * that takes a std::string, representing an error message.
 *
 * The macro should be invoked only in a location where declarations for
 * std::string and std::exception are available (typically in a location that
 * has included <string> and <stdexcept>).
 *
 * Note the macro definition omits a trailing semi-colon. A semi-colon should
 * always be appended when the macro is invoked.
 */
#define JEWEL_DERIVED_EXCEPTION(DERIVED_CLASS, BASE_CLASS)\
	class DERIVED_CLASS: public BASE_CLASS\
	{\
	public:\
		explicit DERIVED_CLASS(std::string p_message):\
			BASE_CLASS(p_message)\
		{\
		}\
		virtual ~DERIVED_CLASS() throw()\
		{\
		}\
	}\



#endif  // GUARD_exception_macros_hpp
