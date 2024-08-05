###
# Copyright 2012, 2013 Matthew Harvey
# 
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
###

# Preliminaries

cmake_minimum_required (VERSION 2.8)

# Specify project and version

project (Jewel)
set (version_major 1)
set (version_minor 0)
set (version_patch 2)

# Custom configuration options

option (
    BUILD_SHARED_LIBS
    "Build shared (rather than static) library (ON/OFF)?"
    OFF
)
option (
    ENABLE_ASSERTION_LOGGING
    "Enable logging of assertion failures within the compiled Jewel library itself (ON/OFF)?"
    ON
)
option (
    ENABLE_EXCEPTION_LOGGING
    "Enable logging of exceptions thrown from within the compiled Jewel library itself (ON/OFF)?"
    ON
)

# Definitions to be passed to the compiler

add_definitions (
    -DJEWEL_VERSION_MAJOR=${version_major}
    -DJEWEL_VERSION_MINOR=${version_minor}
    -DJEWEL_VERSION_PATCH=${version_patch}
)
add_definitions (-DJEWEL_ENABLE_LOGGING)
if (ENABLE_ASSERTION_LOGGING)
    add_definitions (-DJEWEL_ENABLE_ASSERTION_LOGGING)
endif ()
if (ENABLE_EXCEPTION_LOGGING)
    add_definitions (-DJEWEL_ENABLE_EXCEPTION_LOGGING)
endif ()
if (CMAKE_COMPILER_IS_GNUCXX)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
endif ()

# Find dependencies

find_package (Boost 1.53.0 REQUIRED)
find_library (UNIT_TEST_LIBRARY UnitTest++)

# Build instructions

if (Boost_FOUND)

    include_directories (include ${Boost_INCLUDE_DIRS} ${UNIT_TEST_INCLUDES})

    # Building the library

    set (
        library_sources
        src/decimal.cpp
        src/exception.cpp
        src/info.cpp
        src/log.cpp
        src/num_digits.cpp
        src/checked_arithmetic_detail.cpp
        src/version.cpp
    )
    set (library_name jewel)
    add_library (${library_name} ${library_sources})

    if (UNIT_TEST_LIBRARY_FOUND)
      # Building the tests
      set (
          test_sources
          tests/test.cpp
          tests/capped_string_tests.cpp
          tests/checked_arithmetic_tests.cpp
          tests/decimal_special_tests.cpp
          tests/decimal_tests.cpp
          tests/exception_special_tests.cpp
          tests/exception_tests.cpp
          tests/flag_set_tests.cpp
          tests/num_digits_tests.cpp
          tests/on_windows_tests.cpp
          tests/optional_tests.cpp
          tests/smallest_sufficient_unsigned_type_tests.cpp
          tests/stopwatch_tests.cpp
          tests/version_tests.cpp
      )
      add_executable (test_driver ${test_sources})
      target_link_libraries (test_driver ${UNIT_TEST_LIBRARY} ${library_name})
      if (WIN32)
          set (test_execution_command ".\\test_driver.exe")
      else ()
          set (test_execution_command "./test_driver")
      endif ()
      add_custom_target (
          test
          ALL
          COMMAND "${test_execution_command}"
          DEPENDS ${library_name} test_driver
      )
    endif()

    # Building the Decimal speed trial

    set (
        speed_trial_sources
        trials/decimal_speed_trial.cpp
    )
    add_executable (decimal_speed_trial ${speed_trial_sources})
    target_link_libraries (decimal_speed_trial ${library_name})

    # Installation instructions

    set (lib_installation_dir "${CMAKE_INSTALL_PREFIX}/lib")
    set (header_installation_dir "${CMAKE_INSTALL_PREFIX}/include/${library_name}")
    install (
        TARGETS ${library_name}
        ARCHIVE DESTINATION ${lib_installation_dir}
        LIBRARY DESTINATION ${lib_installation_dir}
    )
    install (
        FILES
            include/assert.hpp
            include/capped_string.hpp
            include/capped_string_fwd.hpp
            include/checked_arithmetic.hpp
            include/log.hpp
            include/decimal.hpp
            include/decimal_exceptions.hpp
            include/decimal_fwd.hpp
            include/exception.hpp
            include/flag_set.hpp
            include/info.hpp
            include/num_digits.hpp
            include/on_windows.hpp
            include/optional.hpp
            include/signature.hpp
            include/stopwatch.hpp
            include/version.hpp
            include/version_fwd.hpp
        DESTINATION
            ${header_installation_dir}
    )
    install (
        FILES
            include/detail/checked_arithmetic_detail.hpp
            include/detail/helper_macros.hpp
            include/detail/smallest_sufficient_unsigned_type.hpp
        DESTINATION
            "${header_installation_dir}/detail"
    )

    # Building the tarball source package

    if (UNIX)
        set (package_name "${library_name}-${version_major}.${version_minor}.${version_patch}-working")
        set (tarball_name "${package_name}.tar.gz")
        set (
            packaged_items
            include
            src
            tests
            tools
            trials
            CMakeLists.txt
            Doxyfile
            LICENSE
            NOTICE
            README
            overview.dox
            coding_style
        )
        add_custom_target (
            package
            COMMAND
                mkdir ${package_name} &&
                cp -r ${packaged_items} ${package_name} &&
                tar -sczf ${tarball_name} ${package_name} &&
                rm -rf ${package_name}
            DEPENDS ${library_name} test
        )
        set_directory_properties (
            PROPERTIES
                ADDITIONAL_MAKE_CLEAN_FILES "${tarball_name}"
        )
    endif ()

    # Building the documentation

    add_custom_target (
        docs
        COMMAND doxygen Doxyfile
    )

endif ()
