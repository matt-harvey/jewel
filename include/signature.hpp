#ifndef GUARD_signature_hpp_897804761257863
#define GUARD_signature_hpp_897804761257863

// Copyright (c) 2013, Matthew Harvey. All rights reserved.

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
class Signature
{
public:
	friend T;
	Signature(Signature const&) = delete;
	Signature(Signature&&) = delete;
	Signature& operator=(Signature const&) = delete;
	Signature& operator=(Signature&&) = delete;
	~Signature() = default;
private:
	Signature() = default;
};



}  // namespace jewel

#endif  // GUARD_signature_hpp_897804761257863
