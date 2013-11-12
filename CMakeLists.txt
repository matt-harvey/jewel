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

	set (
		speed_trial_sources
		trials/decimal_speed_trial.cpp
	)

	add_definitions (
		-DJEWEL_ENABLE_LOGGING
		-DJEWEL_ENABLE_ASSERTION_LOGGING
		-DJEWEL_ENABLE_EXCEPTION_LOGGING
		# -DJEWEL_PERFORM_DECIMAL_OUTPUT_FAILURE_TEST
	)

	set (library_name jewel)

	add_library (${library_name} ${library_sources})

	add_executable (test_driver ${test_sources})
	add_executable (decimal_speed_trial ${speed_trial_sources})

	if (WIN32)
		set (test_execution_command ".\\test_driver.exe")
	else ()
		set (test_execution_command "./test_driver")
	endif ()

	target_link_libraries (test_driver UnitTest++ ${library_name})
	target_link_libraries (decimal_speed_trial ${library_name})

	add_custom_target (
		test
		ALL
		COMMAND "${test_execution_command}"
		DEPENDS ${library_name} test_driver
	)

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

	add_custom_target (
		docs
		COMMAND doxygen Doxyfile
	)

	set_directory_properties (
		PROPERTIES
			ADDITIONAL_MAKE_CLEAN_FILES "${tarball_name}"
	)

endif ()
