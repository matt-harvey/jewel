#ifndef GUARD_flag_set_hpp
#define GUARD_flag_set_hpp

/** @file flag_set.hpp
 *
 * @brief Provides facilities for working with sets of boolean flags.
 *
 * @author Matthew Harvey
 * @date 31 August 2013
 *
 * Copyright (c) 2013, Matthew Harvey. All rights reserved.
 */

#include "assert.hpp"
#include "exception.hpp"
#include <boost/static_assert.hpp>
#include <jewel/exception.hpp>


namespace jewel
{

/**
 * Defines an exception type to be thrown when a boolean / bit flag is
 * invalid is some context.
 */
JEWEL_DERIVED_EXCEPTION(InvalidFlagException, jewel::Exception);

/**
 * Provides traits used in FlagSet class template. Specialize this
 * for your \e "FlagT" class (see documentation for \e FlagSet) if you want
 * to use different traits than the defaults.
 */
template <typename EnumT>
struct EnumTraits
{
	typedef unsigned int IntT;
};

/**
 * Encapsulates a set of flags. A certain set of flags is
 * defined to be relevant for this particular set, and then
 * the client can switch these flags on or off, and query them,
 * via a reasonably safe and intuitive interface that avoids the use of
 * direct bit-twiddling, but is protected
 * against accidentally switching or query flags not in the
 * relevant set.
 *
 * This is useful in, for example, a family of widgets accepting
 * a large number of options, but for which not all all options
 * are relevant for all widgets. Particular widgets can
 * specify that their options must be of a given instantiation
 * of FlagSet.
 *
 *
 * TEMPLATE PARAMETERS:
 *
 * \e EnumT is an enum defined in client code, in which the enumerators are
 * initialized with progressive powers of 2, i.e.: 1, 2, 4 etc.. Each
 * enumerator represents a boolean flag.
 *
 * \e mask is an integer mask of those enumerators in EnumT that are
 * considered relevant for this
 * instantiation of \e FlagSet. E.g. suppose \e EnumT is: \n
 *
 * <tt>
 * Enum \n
 * { \n
 * \t red = 1, \n
 * \t blue = 2, \n
 * \t green = 4, \n
 * \t orange = 8 \n
 * }; \n
 * </tt>.
 *
 * Then \e mask might be <em>(red | green)</em>, which
 * expresses that only red and green are relevant for this instantiation
 * of \e FlagSet.
 *
 * \e default_value determines the set of flags which an instance of this
 * instantiation of FlagSet
 * will hold if it is constructed with the default constructor. This must
 * either
 * be 0, or else be some combination of the flags in EnumT such that
 * all flags are in \e mask (compilation will fail if not). If this
 * template parameter is not specified, it will default to 0, so
 * that a default-constructed instance will have no flags set.
 *
 * By default, the underlying integer value holding the flags is stored in
 * an <em>unsigned int</em>. If this is not big enough to encompass \e EnumT,
 * compilation will fail. An alternative underlying integral type can
 * be supplied be specializing \e EnumTraits for \e EnumT.
 *
 * @todo HIGH PRIORITY testing
 */
template
<	typename EnumT,
	typename EnumTraits<EnumT>::IntT mask,
	typename EnumTraits<EnumT>::IntT default_value = 0
>
class FlagSet
{
// typedefs
public:

	typedef typename EnumTraits<EnumT>::IntT IntT;

// constructors
public:

	/**
	 * By default the flags are initialized with \e default_value.
	 */
	FlagSet();

// setters
public:

	/**
	 * Set the flag given by p_flag. Only one flag should be set at a time.
	 * If \e p_flag is not in \e mask, then InvalidFlagException will be
	 * thrown.
	 * 
	 * Note that if we want to pass multiple flags simultaneously (having
	 * ORed them together), we can still
	 * do so by explicitly casting from an integer to an \e EnumT. This will
	 * \e not throw an exception, providing all of the ORed flags lie within
	 * the \e mask.
	 *
	 * @returns a reference to \e *this, which enables calls to this function
	 * to be chained.
	 */
	FlagSet& set(EnumT p_flag);

	/**
	 * Clear the flag given by p_flag.
	 *
	 * Exception throwing behaviour is exactly the same as for the \e set
	 * function: the caller cannot clear p_flag that is not in \e mask.
	 *
	 * @returns a reference to \e *this, which enables calls to this function
	 * to be chained.
	 */
	FlagSet& clear(EnumT p_flag);

// getters
public:

	/**
	 * @returns \e true if e\ p_flag is set; otherwise, returns \e false.
	 *
	 * Note exception throwing behaviour is exactly the same as for the \e set
	 * function: the caller cannot test a flag that is not in \e mask.
	 */
	bool test(EnumT p_flag) const;

	/**
	 * @returns the underlying integral representation. This could be useful
	 * for serialization, for example.
	 */
	IntT underlying() const;

// helper functions
private:

	void check_flag_acceptance(EnumT p_flag) const;

// data members
private:

	IntT m_value;

// compile-time assertions
private:

	/**
	 * Ensure \e IntT is big enough to hold the enumeration.
	 */
	BOOST_STATIC_ASSERT
	(	(sizeof(IntT) >= sizeof(EnumT))
	);
	
	/**
	 * Ensure \e default_value is consistent with \e mask.
	 */
	BOOST_STATIC_ASSERT
	(	(default_value & mask) || !default_value
	);

};




// MEMBER FUNCTION DEFINITIONS


template
<	typename EnumT,
	typename EnumTraits<EnumT>::IntT mask,
	typename EnumTraits<EnumT>::IntT default_value
>
inline
FlagSet<EnumT, mask, default_value>::FlagSet(): m_value(default_value)
{
}


template
<	typename EnumT,
	typename EnumTraits<EnumT>::IntT mask,
	typename EnumTraits<EnumT>::IntT default_value
>
FlagSet<EnumT, mask, default_value>&
FlagSet<EnumT, mask, default_value>::set(EnumT p_flag)
{
	check_flag_acceptance(p_flag);
	m_value |= p_flag;
	return *this;
}


template
<	typename EnumT,
	typename EnumTraits<EnumT>::IntT mask,
	typename EnumTraits<EnumT>::IntT default_value
>
FlagSet<EnumT, mask, default_value>&
FlagSet<EnumT, mask, default_value>::clear(EnumT p_flag)
{
	check_flag_acceptance(p_flag);
	m_value &= ~p_flag;
	return *this;
}


template
<	typename EnumT,
	typename EnumTraits<EnumT>::IntT mask,
	typename EnumTraits<EnumT>::IntT default_value
>
bool
FlagSet<EnumT, mask, default_value>::test(EnumT p_flag) const
{
	check_flag_acceptance(p_flag);
	return (m_value & p_flag) != 0;
}


template
<	typename EnumT,
	typename EnumTraits<EnumT>::IntT mask,
	typename EnumTraits<EnumT>::IntT default_value
>
void
FlagSet<EnumT, mask, default_value>::check_flag_acceptance(EnumT p_flag) const
{
	if ((p_flag | mask) != mask)
	{
		JEWEL_THROW
		(	InvalidFlagException,
			"Flag not recognized by this instantiation of FlagSet."
		);
	}
	return;
}


template
<	typename EnumT,
	typename EnumTraits<EnumT>::IntT mask,
	typename EnumTraits<EnumT>::IntT default_value
>
inline
typename EnumTraits<EnumT>::IntT
FlagSet<EnumT, mask, default_value>::underlying() const
{
	return m_value;
}



}  // namespace jewel


#endif  // GUARD_flag_set_hpp
