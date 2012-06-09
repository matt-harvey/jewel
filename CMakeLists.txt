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
target_link_libraries (main unittest++)

# add a target to generate API documentation with Doxygen
find_package(Doxygen)
if(DOXYGEN_FOUND)
	configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in
	               ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
	add_custom_target(doc
	                  ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
					  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
					  COMMENT "Generating API documentation with Doxygen" VERBATIM)
endif(DOXYGEN_FOUND)







