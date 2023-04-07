cmake_minimum_required(VERSION 3.21)
project( ctle )

option( CTLE_BUILD_TESTS "Build the ctle tests" OFF )

find_package(PythonInterp REQUIRED)
find_package(Python REQUIRED)

# generate needed code
execute_process( 
	COMMAND 			${PYTHON_EXECUTABLE} generate.py
	WORKING_DIRECTORY 	${CMAKE_CURRENT_LIST_DIR}
	RESULT_VARIABLE 	py_result
)
message(STATUS "Result of generate.py: ${py_result}")

# if this is the main build of ctle, build the testing code
if(CTLE_BUILD_TESTS)			
	
	include(FetchContent)
				
	# googletest
	FetchContent_Declare(
		googletest
		GIT_REPOSITORY 	https://github.com/google/googletest.git
		GIT_TAG			58d77fa8070e8cec2dc1ed015d66b454c8d78850 # 1.12.1 
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

		./unit_tests/test_bimap.cpp
		./unit_tests/test_bitmap_font.cpp
		./unit_tests/test_endianness.cpp
		./unit_tests/test_file_funcs.cpp
		./unit_tests/test_idx_vector.cpp
		./unit_tests/test_log.cpp
		./unit_tests/test_optional_idx_vector.cpp
		./unit_tests/test_optional_value.cpp
		./unit_tests/test_optional_vector.cpp
		./unit_tests/test_readers_writer_lock.cpp
		./unit_tests/test_status.cpp
		./unit_tests/test_status_return.cpp
		./unit_tests/test_string_funcs.cpp
		./unit_tests/test_thread_safe_map.cpp
		./unit_tests/test_uuid.cpp
		)
	target_link_libraries( 	
        unit_tests 
		gtest_main 
		)

# testing
endif(CTLE_BUILD_TESTS)
