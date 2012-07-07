cmake_minimum_required (VERSION 2.6)
project (main)
include_directories (/home/matthew/lib/jewel)
link_directories (/home/matthew/lib/jewel)
add_executable (main 
                main.cpp
				arithmetic_exceptions.cpp
                checked_arithmetic_detail.cpp
                checked_arithmetic_tests.cpp
		        decimal.cpp
				decimal_special_tests.cpp
		        decimal_tests.cpp
				num_digits.cpp
		        num_digits_tests.cpp
		        stopwatch_tests.cpp
		        power_vector_tests.cpp)
add_library(jewel
            checked_arithmetic_detail.cpp
	        decimal.cpp
			arithmetic_exceptions.cpp
			num_digits.cpp)
target_link_libraries (main unittest++ boost_serialization)

install(TARGETS
		jewel
		DESTINATION
		/home/matthew/lib/jewel)

install(FILES
        arithmetic_exceptions.hpp
		checked_arithmetic.hpp
		checked_arithmetic_detail.hpp
		decimal.hpp
		Doxyfile
		num_digits.hpp
		overview.dox
		power_vector.hpp
		stopwatch.hpp
		DESTINATION
		/home/matthew/lib/jewel)
		






