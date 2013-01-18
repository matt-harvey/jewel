cmake_minimum_required (VERSION 2.6)

project (jewel)


if (MSYS)
	include_directories(C:/MSYS/1.0/local/include)
	link_directories(C:/MSYS/1.0/local/lib)
endif ()

set (library_sources
     checked_arithmetic_detail.cpp
	 decimal.cpp
	 exception.cpp
	 num_digits.cpp)

set (test_sources
		test.cpp
		checked_arithmetic_tests.cpp
		decimal_special_tests.cpp
		decimal_tests.cpp
		exception_special_tests.cpp
		exception_tests.cpp
		num_digits_tests.cpp
		optional_tests.cpp
		stopwatch_tests.cpp
		power_vector_tests.cpp)

add_executable (jewel_test ${library_sources} ${test_sources})

add_library(jewel ${library_sources})

if (MSYS)
	set (boost_locale_lib boost_locale)
else ()
	set (boost_locale_lib boost_locale-mt)
endif ()

target_link_libraries (jewel_test
		       ${boost_locale_lib}
                       unittest++)
if (MSYS)
	set (lib_installation_dir C:/MSYS/1.0/local/lib)
else ()
	set (lib_installation_dir /usr/local/lib)
endif ()

install(TARGETS jewel
	ARCHIVE DESTINATION ${lib_installation_dir})

if (MSYS)
	set (header_installation_dir C:/MSYS/1.0/local/include)
else ()
	set (header_installation_dir /usr/local/include)
endif ()

install(DIRECTORY jewel DESTINATION ${header_installation_dir})

install(FILES
        decimal_exceptions.hpp
		checked_arithmetic.hpp
		checked_arithmetic_detail.hpp
		debug_log.hpp
		decimal.hpp
		exception.hpp
		num_digits.hpp
		optional.hpp
		output_aux.hpp
		overview.dox
		power_vector.hpp
		stopwatch.hpp
		DESTINATION
		"${header_installation_dir}/jewel")


