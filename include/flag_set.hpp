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

#ifndef GUARD_flag_set_hpp_4153674614687299
#define GUARD_flag_set_hpp_4153674614687299

/** @file
 *
 * @brief Provides facilities for working with sets of boolean flags.
 *
 * @see jewel::InvalidFlagException
 */

#include "assert.hpp"
#include "exception.hpp"


namespace jewel
{

/** @class jewel::InvalidFlagException
 *
 * @extends jewel::Exception
 *
 * Defines an exception type to be thrown when a boolean / bit flag is
 * invalid is some context.
 */

/// @cond
JEWEL_DERIVED_EXCEPTION(InvalidFlagException, jewel::Exception);
/// @endcond

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
 * @tparam EnumT is an enum defined in client code, in which the enumerators are
 * initialized with progressive powers of 2, i.e.: 1, 2, 4 etc.. Each
 * enumerator represents a boolean flag. EnumT should be a plain enum,
 * not an enum class.\n\n
 *
 * @tparam mask is an integer mask of those enumerators in EnumT that are
 * considered relevant for this
 * instantiation of FlagSet. E.g. suppose \e EnumT is: \n
 * <b><tt> enum Color { red = 1, blue = 2, green = 4, orange = 8 };</tt></b>\n
 * Then \e mask might be <em>(red | green)</em>, which
 * expresses that only red and green are relevant for this instantiation
 * of FlagSet.\n\n
 *
 * @tparam default_value determines the set of flags which an instance of this
 * instantiation of FlagSet
 * will hold if it is constructed with the default constructor. This must
 * either
 * be 0, or else be some combination of the flags in EnumT such that
 * all flags are in \e mask (compilation will fail if not). If this
 * template parameter is not specified, it will default to 0, so
 * that a default-constructed instance will have no flags set.\n
 *
 * By default, the underlying integer value holding the flags is stored in
 * an <em>unsigned int</em>. An alternative underlying integral type can
 * be supplied be specializing EnumTraits for \e EnumT. It is the
 * client's responsibility to ensure that this type will be sufficiently
 * large.
 *
 * @see jewel::InvalidFlagException
 */
template
<    typename EnumT,
    typename EnumTraits<EnumT>::IntT mask,
    typename EnumTraits<EnumT>::IntT default_value = 0
>
class FlagSet
{
// typedefs
public:

    typedef typename EnumTraits<EnumT>::IntT IntT;

// constructors, assignment and destructor
public:

    /**
     * By default the flags are initialized with \e default_value.
     */
    FlagSet();

    FlagSet(FlagSet const&) = default;
    FlagSet(FlagSet&&) = default;
    FlagSet& operator=(FlagSet const&) = default;
    FlagSet& operator=(FlagSet&&) = default;

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
     * Ensure \e default_value is consistent with \e mask.
     */
    static_assert
    (   (default_value & mask) || !default_value,
        "default_value for FlagSet is inconconsistent with mask."
    );

};




// MEMBER FUNCTION DEFINITIONS


template
<    typename EnumT,
    typename EnumTraits<EnumT>::IntT mask,
    typename EnumTraits<EnumT>::IntT default_value
>
inline
FlagSet<EnumT, mask, default_value>::FlagSet(): m_value(default_value)
{
}


template
<    typename EnumT,
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
<    typename EnumT,
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
<    typename EnumT,
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
<    typename EnumT,
    typename EnumTraits<EnumT>::IntT mask,
    typename EnumTraits<EnumT>::IntT default_value
>
void
FlagSet<EnumT, mask, default_value>::check_flag_acceptance(EnumT p_flag) const
{
    if ((p_flag | mask) != mask)
    {
        JEWEL_THROW
        (   InvalidFlagException,
            "Flag not recognized by this instantiation of FlagSet."
        );
    }
    return;
}


template
<    typename EnumT,
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


#endif  // GUARD_flag_set_hpp_4153674614687299
