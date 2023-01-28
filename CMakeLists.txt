cmake_minimum_required(VERSION 3.21)
project( ctle )

include(FetchContent)
	
# if this is the main build of ctle, build the testing code
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)			
				
	# googletest
	FetchContent_Declare(
	    googletest
	    URL https://github.com/google/googletest/archive/58d77fa8070e8cec2dc1ed015d66b454c8d78850.zip
	    )

	FetchContent_MakeAvailable( 
		googletest 
		)
		
	# lots of warnings and all warnings as errors
	if(MSVC)
		add_compile_options(/W4 /WX)
	else()
		add_compile_options(-Wall -Wextra -pedantic -Werror)
	endif()

	# unit_tests
	add_executable( 
        unit_tests
        ./unit_tests/unit_tests.cpp
        ./unit_tests/unit_tests.h
		
		#./unit_tests/basic_tests.cpp
		)
	target_link_libraries( 	
        unit_tests 
		gtest_main 
		)

# testing
endif()
