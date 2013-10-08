cmake_minimum_required (VERSION 2.6)

project (jewel)

if (WIN32)
	include_directories(include
	                    C:/Dev-Cpp
	                    C:/Dev-Cpp/include/common
						C:/include)
	link_directories(C:/Dev-Cpp/lib
	                 C:/Dev-Cpp/MinGW/lib
					 C:/lib)
else ()
	include_directories(include)
endif ()

set (library_sources
	 src/decimal.cpp
	 src/exception.cpp
	 src/log.cpp
	 src/num_digits.cpp
	 src/checked_arithmetic_detail.cpp)

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
		tests/stopwatch_tests.cpp)

add_definitions(-DJEWEL_ENABLE_LOGGING
				-DJEWEL_ENABLE_ASSERTION_LOGGING
                -DJEWEL_ENABLE_EXCEPTION_LOGGING)

add_library(jewel ${library_sources})

add_executable (test ${test_sources})

target_link_libraries (test UnitTest++ jewel)

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

install(FILES
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
		include/output_aux.hpp
		include/signature.hpp
		include/stopwatch.hpp
		DESTINATION
		${inner_header_installation_dir})

install(FILES
		include/detail/checked_arithmetic_detail.hpp
		include/detail/helper_macros.hpp
		include/detail/smallest_sufficient_unsigned_type.hpp
		DESTINATION
		"${inner_header_installation_dir}/detail")

