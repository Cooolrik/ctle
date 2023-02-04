cmake_minimum_required(VERSION 3.21)
project( ctle )

include(FetchContent)
	
# if this is the main build of ctle, build the testing code
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)			
				
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

	# c++14
	set (CMAKE_CXX_STANDARD 14)

	# unit_tests
	add_executable( 
        unit_tests
        ./unit_tests/unit_tests.cpp
        ./unit_tests/unit_tests.h

		./unit_tests/test_bimap.cpp                            
		./unit_tests/test_endianness.cpp                       
		./unit_tests/test_idx_vector.cpp                       
		./unit_tests/test_list                                 
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
endif()
