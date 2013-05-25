// Copyright (c) 2013, Matthew Harvey. All rights reserved.

#ifndef GUARD_signature_hpp
#define GUARD_signature_hpp

#include <boost/noncopyable.hpp>


namespace jewel
{


/**
 * The instantiation of this template for a class T is a
 * non-copyable class the constructor of which is only
 * accessible by
 * class T. This provides a mechanism whereby a
 * function that wants to ensure at compile time
 * that it can only be called by class T, can achieve
 * this simply by having a
 * parameter of type Signature<T>& (or Signature<T> const&).
 * This provides a fine-grained, function-level means of
 * access control, that is not directly provided by
 * the \e friend mechanism.
 */
template <class T>
class Signature:
	private boost::noncopyable
{
public:
	// WARNING this is non-conformant prior to C++11; although
	// some C++98 compilers neverthless allow it.
	friend T;
private:
	Signature();
};


template <class T>
inline
Signature<T>::Signature()
{
}


}  // namespace jewel

#endif  // GUARD_signature_hpp
