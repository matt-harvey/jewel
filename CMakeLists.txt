cmake_minimum_required (VERSION 2.6)

project (jewel)


if (WIN32)
	include_directories(C:/Dev-Cpp
	                    C:/Dev-Cpp/include/common
						C:/include)
	link_directories(C:/Dev-Cpp/lib
	                 C:/Dev-Cpp/MinGW/lib
					 C:/lib)
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
		on_windows_tests.cpp
		optional_tests.cpp
		stopwatch_tests.cpp
		power_vector_tests.cpp)

add_executable (jewel_test ${library_sources} ${test_sources})

#add_library(jewel ${library_sources})
add_library(jewel SHARED ${library_sources})

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
	LIBRARY DESTINATION ${lib_installation_dir})

if (WIN32)
	set (header_installation_dir C:/include)
else ()
	set (header_installation_dir /usr/local/include)
endif ()

install(DIRECTORY jewel DESTINATION ${header_installation_dir})

install(FILES
		checked_arithmetic.hpp
		checked_arithmetic_detail.hpp
		debug_log.hpp
		decimal.hpp
        decimal_exceptions.hpp
		decimal_fwd.hpp
		exception.hpp
		num_digits.hpp
		on_windows.hpp
		optional.hpp
		output_aux.hpp
		overview.dox
		power_vector.hpp
		signature.hpp
		stopwatch.hpp
		DESTINATION
		"${header_installation_dir}/jewel")


