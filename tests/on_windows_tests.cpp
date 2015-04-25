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

#include "on_windows.hpp"

#include <UnitTest++/UnitTest++.h>


#include <iostream>

TEST(on_windows_test)
{
    bool on_windows = false;

    (void)on_windows;   // prevent warning re. unused variable

#   ifdef JEWEL_ON_WINDOWS
        on_windows = true;
#   endif

#   if defined(_WIN32)
        CHECK(on_windows);
#   endif

#   ifdef _WIN32
        CHECK(on_windows);
#   endif

#   if defined(__WIN32__)
        CHECK(on_windows);
#   endif

#   ifdef __WIN32__
        CHECK(on_windows);
#   endif

#   if defined(WIN32)
        CHECK(on_windows);
#   endif

#   ifdef WIN32
        CHECK(on_windows);
#   endif

#   ifdef JEWEL_ON_WINDOWS
#       if !defined(WIN32) && !defined(_WIN32) && !defined(__WIN32__)
            CHECK(false);
#       endif
#   endif

}
