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

#ifndef GUARD_on_windows_hpp_15737586289522046
#define GUARD_on_windows_hpp_15737586289522046


#	if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)

		/**
		 * Single macro that is defined if and only if we are
		 * on Windows, otherwise is not defined at all. This avoids having
		 * to type multiple underscore-related variations of "WIN32" every
		 * time we want to test whether we are on Windows.
		 */
#		define JEWEL_ON_WINDOWS

#	endif




#endif  // GUARD_on_windows_hpp_15737586289522046
