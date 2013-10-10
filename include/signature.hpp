/*
 * This file is part of the Jewel project and is distributed under the
 * terms of the License contained in the file LICENSE.txt distributed
 * with this package.
 * 
 * Author: Matthew Harvey <matthew@matthewharvey.net>
 *
 * Copyright (c) 2012-2013, Matthew Harvey.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
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
