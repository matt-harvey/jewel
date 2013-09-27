cmake_minimum_required (VERSION 2.6)

project (jewel)


if (WIN32)
	include_directories(C:/Dev-Cpp
	                    C:/Dev-Cpp/include/common
						C:/include
						.)
	link_directories(C:/Dev-Cpp/lib
	                 C:/Dev-Cpp/MinGW/lib
					 C:/lib)
else ()
	include_directories(.)
endif ()

set (library_sources
	 decimal.cpp
	 exception.cpp
	 log.cpp
	 num_digits.cpp
	 tolerant_string.cpp
	 detail/checked_arithmetic_detail.cpp)

set (test_sources
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
		tests/stopwatch_tests.cpp
		tests/tolerant_string_tests.cpp)

add_definitions(-DJEWEL_ENABLE_LOGGING
				-DJEWEL_ENABLE_ASSERTION_LOGGING
                -DJEWEL_ENABLE_EXCEPTION_LOGGING)

add_executable (jewel_test ${library_sources} ${test_sources})

add_library(jewel ${library_sources})
#add_library(jewel SHARED ${library_sources})

target_link_libraries (jewel_test
                       UnitTest++)
if (WIN32)
	set (lib_installation_dir C:/lib)
else ()
	set (lib_installation_dir /usr/local/lib)
endif ()

install(TARGETS jewel
	ARCHIVE DESTINATION ${lib_installation_dir})

if (WIN32)
	set (outer_header_installation_dir C:/include)
else ()
	set (outer_header_installation_dir /usr/local/include)
endif ()

set (inner_header_installation_dir "${outer_header_installation_dir}/jewel")

install(DIRECTORY jewel DESTINATION ${outer_header_installation_dir})

install(FILES
		assert.hpp
		capped_string.hpp
		checked_arithmetic.hpp
		log.hpp
		decimal.hpp
        decimal_exceptions.hpp
		decimal_fwd.hpp
		exception.hpp
		flag_set.hpp
		num_digits.hpp
		on_windows.hpp
		optional.hpp
		output_aux.hpp
		overview.dox
		signature.hpp
		stopwatch.hpp
		tolerant_string.hpp
		DESTINATION
		${inner_header_installation_dir})

install(FILES
		detail/checked_arithmetic_detail.hpp
		detail/helper_macros.hpp
		detail/smallest_sufficient_unsigned_type.hpp
		DESTINATION
		"${inner_header_installation_dir}/detail")

