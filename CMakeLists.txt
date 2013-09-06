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
     checked_arithmetic_detail.cpp
	 decimal.cpp
	 exception.cpp
	 log.cpp
	 num_digits.cpp
	 tolerant_string.cpp)

set (test_sources
		tests/test.cpp
		tests/capped_string_tests.cpp
		tests/checked_arithmetic_tests.cpp
		tests/decimal_special_tests.cpp
		tests/decimal_tests.cpp
		tests/exception_special_tests.cpp
		tests/exception_tests.cpp
		tests/num_digits_tests.cpp
		tests/on_windows_tests.cpp
		tests/optional_tests.cpp
		tests/power_vector_tests.cpp
		tests/stopwatch_tests.cpp
		tests/tolerant_string_tests.cpp)

add_executable (jewel_test ${library_sources} ${test_sources})

add_library(jewel ${library_sources})
#add_library(jewel SHARED ${library_sources})

if (WIN32)
	set (boost_locale_lib boost_locale-mgw46-mt-1_53)
else ()
	set (boost_locale_lib boost_locale-mt)
endif ()

target_link_libraries (jewel_test
		       ${boost_locale_lib}
                       UnitTest++)
if (WIN32)
	set (lib_installation_dir C:/lib)
else ()
	set (lib_installation_dir /usr/local/lib)
endif ()

install(TARGETS jewel
	ARCHIVE DESTINATION ${lib_installation_dir})

if (WIN32)
	set (header_installation_dir C:/include)
else ()
	set (header_installation_dir /usr/local/include)
endif ()

install(DIRECTORY jewel DESTINATION ${header_installation_dir})

install(FILES
		array_utilities.hpp
		assert.hpp
		capped_string.hpp
		checked_arithmetic.hpp
		checked_arithmetic_detail.hpp
		log.hpp
		decimal.hpp
        decimal_exceptions.hpp
		decimal_fwd.hpp
		exception.hpp
		flag_set.hpp
		helper_macros.hpp
		num_digits.hpp
		on_windows.hpp
		optional.hpp
		output_aux.hpp
		overview.dox
		power_vector.hpp
		signature.hpp
		smallest_sufficient_unsigned_type.hpp
		stopwatch.hpp
		tolerant_string.hpp
		DESTINATION
		"${header_installation_dir}/jewel")


