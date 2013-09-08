// Copyright (c) 2013, Matthew Harvey. All rights reserved.

#ifndef GUARD_exception_hpp
#define GUARD_exception_hpp

#ifdef JEWEL_ENABLE_EXCEPTION_LOGGING
#	include "log.hpp"
#endif

#include "capped_string.hpp"
#include <cstddef>
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
 * \c max_message_size(), will be truncated to that maximum length, with
 * the text "[TRUNCATED]" appended to indicate truncation has occurred.
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
 * @todo HIGH PRIORITY Further testing.
 */
class Exception: public virtual std::exception
{
public:

	/**
	 * Constructs a throwable exception object containing various
	 * information. The parameters are all optional. The string
	 * parameters may be passed either 0 or the empty string to
	 * indicate that this particular piece of information is not
	 * being supplied. The \e p_line parameter may be passed -1 to
	 * indicate that the line number is not being supplied.
	 *
	 * See class documentation re. possibility of the string parameters
	 * being truncated.
	 *
	 * @param p_message Message to be stored in Exception object explaining
	 * the reason for the exception. May be later retrieved via call to
	 * c_str(). Typically displayed in console if exception not caught.
	 *
	 * @param p_type The type of the exception, i.e. the name of the class
	 * of which the exception is an instance.
	 *
	 * @param p_function The function from which the exception is being
	 * thrown.
	 *
	 * @param p_filepath The filepath name of the source file from which the
	 * exception is being thrown.
	 *
	 * @param p_line The line number in the source file where the exception
	 * is being thrown. If passed -1, this indicates that the line number
	 * is not being supplied.
	 */
	explicit Exception
	(	char const* p_message = 0,
		char const* p_type = 0,
		char const* p_function = 0,
		char const* p_filepath = 0,
		long p_line = -1  // -1 means not provided
	) throw();

	Exception(Exception const& rhs) throw();
	virtual ~Exception() throw();
	virtual char const* what() const throw();
	static size_t max_message_size() throw();

	/**
	 * Retrieve the diagnostic message associated with the exception.
	 * This is equivalent to calling \e what(). Appears as \e message()
	 * for consistency with the pattern for retrieving the other
	 * attributes passed to the constructor. If the \e p_message parameter
	 * was passed 0, this will return an empty string.
	 *
	 * See class documetation re. possibility of truncation.
	 */
	char const* message() const throw();

	/**
	 * Retrieve the name of the string passed to \e p_type in the constructor.
	 * Returns empty string if this was passed 0.
	 *
	 * See class documentation re. possibility of truncation.
	 */
	char const* type() const throw();

	/**
	 * Retrieve the function name passed to \e p_function in the constructor.
	 * Returns the empty string if this was passed 0.
	 *
	 * See class documentation re. possibility of truncation.
	 */
	char const* function() const throw();

	/**
	 * Retrieve the string passed to \e p_filepath in the constructor.
	 * Returns the empty string if this was passed 0.
	 */
	char const* filepath() const throw();

	/**
	 * Return the line number in the source file from which the exception
	 * was thrown (or whatever was passed to \e p_line in the constructor).
	 * A return value of -1 indicates that this information was not supplied
	 * to the constructor.
	 */
	long line() const throw();

private:
	enum
	{
		string_capacity = 211
	};

	Exception const operator=(Exception const&);  // unimplemented

	typedef CappedString<string_capacity> String;
	String m_message;
	String m_type;
	String m_function;
	String m_filepath;
	long m_line;	
};


}  // namespace jewel



/**
 * Macro to declare and define an exception class called \c DERIVED_CLASS,
 * inheriting from \c BASE_CLASS. The \c BASE_CLASS must have a constructor
 * that takes the same parameters as jewel::Exception. This is most easily
 * achieved by actually deriving from jewel::Exception, either directly
 * or via another parent class or classes.
 *
 * Note the macro definition omits a trailing semi-colon. A semi-colon should
 * always be appended when the macro is invoked.
 */
#define JEWEL_DERIVED_EXCEPTION(DERIVED_CLASS, BASE_CLASS)\
	class DERIVED_CLASS: public BASE_CLASS\
	{\
	public:\
		explicit DERIVED_CLASS \
		(	char const* p_message = 0, \
			char const* p_type = 0, \
			char const* p_function = 0, \
			char const* p_filepath = 0, \
			long p_line = -1 \
		) throw(): \
			BASE_CLASS \
			(	p_message, \
				p_type, \
				p_function, \
				p_filepath, \
				p_line \
			) \
		{\
		}\
		virtual ~DERIVED_CLASS() throw()\
		{\
		}\
	}\



/**
 * Macro to throw an exception of type \e TYPE, where \e TYPE inherits from
 * (or is) \e jewel::Exception. Pass a const char* to
 * \e MESSAGE, which will
 * determine the string returned by \e what() (and
 * thus displayed at the terminal). The macro also automatically populates
 * other fields in the exception, which can later be retrieved by calling
 * \e type(), \e function(), \e filepath() and
 * \e line() methods of the exception object.
 *
 * If JEWEL_ENABLE_EXCEPTION_LOGGING is defined, then this macro will
 * also invoke jewel::Log::log to write to the log with severity level
 * jewel::Log::error, with details of the exception. Note this will have
 * effect even if JEWEL_ENABLE_LOGGING is not defined.
 */
#ifdef JEWEL_ENABLE_EXCEPTION_LOGGING
#	define JEWEL_THROW(TYPE, MESSAGE) \
		jewel::Log::log \
		(	jewel::Log::error, \
			MESSAGE, \
			__func__, \
			__FILE__, \
			__LINE__, \
			__DATE__, \
			__TIME__, \
			#TYPE \
		); \
		throw TYPE \
		(	MESSAGE, \
			#TYPE, \
			__func__, \
			__FILE__, \
			__LINE__ \
		);
#else
#	define JEWEL_THROW(TYPE, MESSAGE) \
		throw TYPE \
		(	MESSAGE, \
			#TYPE, \
			__func__, \
			__FILE__, \
			__LINE__ \
		);
#endif  // JEWEL_ENABLE_EXCEPTION_LOGGING


#endif  // GUARD_exception_hpp
