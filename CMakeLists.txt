cmake_minimum_required (VERSION 2.6)

project (jewel)

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

target_link_libraries (jewel_test
					   boost_locale-mt
                       unittest++)

install(TARGETS jewel
       ARCHIVE DESTINATION lib)

install(DIRECTORY jewel DESTINATION include)

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
		include/jewel)


