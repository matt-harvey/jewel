/*
 * Copyright 2013 Matthew Harvey
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef GUARD_exception_hpp_6015026497661614
#define GUARD_exception_hpp_6015026497661614


#include "capped_string.hpp"
#include <cstddef>
#include <cstring>
#include <iostream>
#include <ostream>
#include <stdexcept>
#include <utility>

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
 * can be initialized with no parameters, with a <tt> char const* </tt>,
 * representing
 * an error message, or with a range of other parameters.
 * The char const*, if lengthier
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
 * Exception safety: all of the \e member functions in this class offer
 * the <em>nothrow guarantee</em>. But see the documentation in the
 * (non-member) stream output operator, which theoretically can throw if
 * exceptions are enabled on the stream.
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
	 * \e what() or \e message(). Typically displayed in console if exception
	 * not caught.
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
	) noexcept;

	Exception(Exception const&) noexcept;

	Exception(Exception&& rhs) noexcept;

	Exception& operator=(Exception const&) = delete;

	Exception& operator=(Exception&&) = delete;

	virtual ~Exception() noexcept;

	virtual char const* what() const noexcept;

	static size_t max_message_size() noexcept;

	/**
	 * Retrieve the diagnostic message associated with the exception.
	 * This is equivalent to calling \e what(). Appears as \e message()
	 * for consistency with the pattern for retrieving the other
	 * attributes passed to the constructor. If the \e p_message parameter
	 * was passed 0, this will return an empty string.
	 *
	 * See class documetation re. possibility of truncation.
	 */
	char const* message() const noexcept;

	/**
	 * Retrieve the name of the string passed to \e p_type in the constructor.
	 * Returns empty string if this was passed 0.
	 *
	 * See class documentation re. possibility of truncation.
	 */
	char const* type() const noexcept;

	/**
	 * Retrieve the function name passed to \e p_function in the constructor.
	 * Returns the empty string if this was passed 0.
	 *
	 * See class documentation re. possibility of truncation.
	 */
	char const* function() const noexcept;

	/**
	 * Retrieve the string passed to \e p_filepath in the constructor.
	 * Returns the empty string if this was passed 0.
	 */
	char const* filepath() const noexcept;

	/**
	 * Return the line number in the source file from which the exception
	 * was thrown (or whatever was passed to \e p_line in the constructor).
	 * A return value of -1 indicates that this information was not supplied
	 * to the constructor.
	 */
	long line() const noexcept;

private:
	enum
	{
		string_capacity = 211
	};

	typedef CappedString<string_capacity> String;
	String m_message;
	String m_type;
	String m_function;
	String m_filepath;
	long m_line;	
};


// NON-MEMBER FUNCTIONS

/**
 * @relates Exception
 *
 * Output a description of a jewel::Exception to \e os. This description is
 * more detailed than that returned by \e what() and \e message(). It
 * contains information about all the information that was passed to the
 * Exception constructor.
 *
 * It is theoretically possible for this function to throw an exception, but
 * only if exceptions are enabled on \e os, and there is an error when
 * outputting to the stream (unlikely).
 *
 * Note outputting the exception will \e not necessarily cause \e os to be
 * flushed. The user of this function should flush the stream manually after
 * calling this function if they want to make sure it is flushed.
 *
 * TODO MEDIUM PRIORITY Write a test for this.
 */
template <typename charT, typename traits>
std::basic_ostream<charT, traits>&
operator<<(std::basic_ostream<charT, traits>& os, Exception const& e);

}  // namespace jewel


#include "assert.hpp"

#ifdef JEWEL_ENABLE_EXCEPTION_LOGGING
#	include "log.hpp"
#endif


/** @def JEWEL_DERIVED_EXCEPTION(DERIVED_CLASS, BASE_CLASS)
 *
 * @relates Exception
 *
 * @hideinitializer
 *  
 * Macro to declare and define an exception class called \e DERIVED_CLASS,
 * inheriting from \e BASE_CLASS. The \e BASE_CLASS must have a constructor
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
		) noexcept: \
			BASE_CLASS \
			(	p_message, \
				p_type, \
				p_function, \
				p_filepath, \
				p_line \
			) \
		{\
		}\
		virtual ~DERIVED_CLASS() noexcept \
		{\
		}\
	}\



/** @def JEWEL_THROW(TYPE, MESSAGE)
 *
 * @relates Exception
 *
 * @hideinitializer
 *
 * Macro to throw an exception of type \e TYPE, where \e TYPE inherits from
 * (or is) jewel::Exception. Pass a const char* to
 * \e MESSAGE, which will
 * determine the string returned by \e what() (and
 * thus displayed at the terminal). The macro also automatically populates
 * other fields in the exception, which can later be retrieved by calling
 * jewel::Exception::type, jewel::Exception::function,
 * jewel::Exception::filepath and
 * jewel::Exception::line methods of the exception object.
 *
 * If \b JEWEL_ENABLE_EXCEPTION_LOGGING is defined, then this macro will
 * also invoke jewel::Log::log to write to the log with severity level
 * jewel::Log::warning, with details of the exception. Note this will have
 * effect even if \b JEWEL_ENABLE_LOGGING is not defined.
 */

#ifdef JEWEL_ENABLE_EXCEPTION_LOGGING
#	define JEWEL_THROW(TYPE, MESSAGE) \
		jewel::Log::log \
		(	jewel::Log::warning, \
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



namespace jewel
{

// IMPLEMENT NON-MEMBER FUNCTIONS

template <typename charT, typename traits>
std::basic_ostream<charT, traits>&
operator<<(std::basic_ostream<charT, traits>& os, Exception const& e)
{
	using std::endl;
	using std::make_pair;
	using std::pair;
	using std::strlen;
	if (!os)
	{
		return os;
	}
	pair<char const* const, char const* const> const data[] =
	{	make_pair("Message", e.message()),
		make_pair("Exception type", e.type()),
		make_pair("Function in which exception was thrown", e.function()),
		make_pair("Filepath", e.filepath())
	};
	os << "BEGIN EXCEPTION DESCRIPTION" << '\n';
	char const* const sep = ": ";
	for (auto const& datum: data)
	{
		if (strlen(datum.second) != 0)
		{
			JEWEL_ASSERT (strlen(datum.second) > 0);
			os << datum.first << sep << datum.second << '\n';
		}
	}
	if (e.line() >= 0)
	{
		os << "Line" << sep << e.line() << '\n';
	}
	os << "END EXCEPTION DESCRIPTION" << '\n';
	return os;
}

}  // namespace jewel


#endif  // GUARD_exception_hpp_6015026497661614
