###
# Copyright 2012-2013 Matthew Harvey
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

cmake_minimum_required (VERSION 2.6)

project (Jewel)

find_package (Boost 1.53.0)

if (Boost_FOUND)

	include_directories (include)

	set (
		library_sources
		src/decimal.cpp
		src/exception.cpp
		src/log.cpp
		src/num_digits.cpp
		src/checked_arithmetic_detail.cpp
		src/version.cpp
	)

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

	add_definitions (
		-DJEWEL_ENABLE_LOGGING
		-DJEWEL_ENABLE_ASSERTION_LOGGING
		-DJEWEL_ENABLE_EXCEPTION_LOGGING
	)

	set (library_name jewel)

	add_library (${library_name} ${library_sources})

	add_executable (test ${test_sources})

	target_link_libraries (test UnitTest++ ${library_name})

	set (lib_installation_dir "${CMAKE_INSTALL_PREFIX}/lib")

	set (header_installation_dir "${CMAKE_INSTALL_PREFIX}/include/${library_name}")

	install(TARGETS ${library_name} ARCHIVE DESTINATION ${lib_installation_dir})

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
			include/num_digits.hpp
			include/on_windows.hpp
			include/optional.hpp
			include/signature.hpp
			include/stopwatch.hpp
			include/version.hpp
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

	set (version_major 1)
	set (version_minor 0)
	set (version_patch 0)
	set (package_name "${library_name}-${version_major}.${version_minor}.${version_patch}-working")
	set (tarball_name "${package_name}.tar.gz")

	set (
		packaged_items
		include
		src
		tests
		tools
		CMakeLists.txt
		Doxyfile
		INSTALL
		LICENSE
		NOTICE
		README
		overview.dox
	)

	add_custom_target (
		package
		ALL
		COMMAND
			mkdir ${package_name} &&
			cp -r ${packaged_items} ${package_name} &&
			tar -sczf ${tarball_name} ${package_name} &&
			rm -rf ${package_name}
		DEPENDS ${library_name}
	)

	add_custom_target (
		docs
		ALL
		COMMAND doxygen
	)

	set_directory_properties (
		PROPERTIES
			ADDITIONAL_MAKE_CLEAN_FILES ${tarball_name}
			ADDITIONAL_MAKE_CLEAN_FILES "html"
	)

endif ()
