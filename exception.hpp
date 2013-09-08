// Copyright (c) 2013, Matthew Harvey. All rights reserved.

#ifndef GUARD_exception_hpp
#define GUARD_exception_hpp

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
 * @todo HIGH PRIORITY Document member functions.
 *
 * @todo HIGH PRIORITY Further testing.
 */
class Exception: public virtual std::exception
{
public:

	Exception() throw();
	explicit Exception
	(	char const* p_message,
		char const* p_type = 0,
		char const* p_throwing_function = 0,
		char const* p_throwing_filepath = 0,
		long p_throwing_line_number = -1  // -1 means not provided
	) throw();

	Exception(Exception const& rhs) throw();
	virtual ~Exception() throw();
	virtual char const* what() const throw();
	static size_t max_message_size() throw();

	char const* message() const;
	char const* type() const;
	char const* throwing_function() const;
	char const* throwing_filepath() const;
	std::size_t throwing_line_number() const;

private:
	enum
	{
		string_capacity = 211
	};

	Exception const operator=(Exception const&);  // unimplemented

	typedef CappedString<string_capacity> String;
	String m_message;
	String m_type;
	String m_throwing_function;
	String m_throwing_filepath;
	long m_throwing_line_number;	
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
		DERIVED_CLASS() throw()\
		{\
		}\
		explicit DERIVED_CLASS \
		(	char const* p_message, \
			char const* p_type = 0, \
			char const* p_throwing_function = 0, \
			char const* p_throwing_filepath = 0, \
			long p_throwing_line_number = -1 \
		) throw(): \
			BASE_CLASS \
			(	p_message, \
				p_type, \
				p_throwing_function, \
				p_throwing_filepath, \
				p_throwing_line_number \
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
 * \e type(), \e throwing_function(), \e throwing_filepath() and
 * \e throwing_line_number() methods of the exception object.
 */
#define JEWEL_THROW(TYPE, MESSAGE) \
	throw TYPE \
	(	MESSAGE, \
		#TYPE, \
		__func__, \
		__FILE__, \
		__LINE__ \
	);

		


#endif  // GUARD_exception_hpp
