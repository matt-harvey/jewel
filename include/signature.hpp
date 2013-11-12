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

#ifndef GUARD_signature_hpp_897804761257863
#define GUARD_signature_hpp_897804761257863


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
