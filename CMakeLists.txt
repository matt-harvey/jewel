cmake_minimum_required (VERSION 2.6)
project (main)
add_executable (main 
                main.cpp
                checked_arithmetic_detail.cpp
                checked_arithmetic_tests.cpp
		        decimal.cpp
				decimal_special_tests.cpp
		        decimal_tests.cpp
				exception.cpp
				exception_special_tests.cpp
				exception_tests.cpp
				num_digits.cpp
		        num_digits_tests.cpp
				optional_tests.cpp
		        stopwatch_tests.cpp
		        power_vector_tests.cpp)
add_library(jewel
            checked_arithmetic_detail.cpp
	        decimal.cpp
			exception.cpp
			num_digits.cpp)
target_link_libraries (main
					   boost_locale-mt
                       unittest++)

install(TARGETS
		jewel
		DESTINATION
		/home/matthew/lib/jewel)

install(FILES
        decimal_exceptions.hpp
		checked_arithmetic.hpp
		checked_arithmetic_detail.hpp
		debug_log.hpp
		decimal.hpp
		exception.hpp
		Doxyfile
		num_digits.hpp
		optional.hpp
		output_aux.hpp
		overview.dox
		power_vector.hpp
		stopwatch.hpp
		DESTINATION
		/home/matthew/lib/jewel)
		






