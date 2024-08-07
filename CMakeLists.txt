cmake_minimum_required(VERSION 3.21)
project( ctle )

option( CTLE_BUILD_TESTS "Build the ctle tests" OFF )

find_package(PythonInterp 3.10 REQUIRED)
find_package(Python 3.10 REQUIRED)

# generate needed code
execute_process( 
	COMMAND 			${PYTHON_EXECUTABLE} generate.py
	WORKING_DIRECTORY 	${CMAKE_CURRENT_LIST_DIR}
	RESULT_VARIABLE 	py_result
)
message(STATUS "Result of generate.py: ${py_result}")

# if selected, build the testing code
# (note: the fetched modules are only needed to test behavior and interops, not needed for use in a project)
if(CTLE_BUILD_TESTS)			
	
	include(FetchContent)
				
	# googletest
	FetchContent_Declare(
		googletest
		GIT_REPOSITORY 	https://github.com/google/googletest.git
		GIT_TAG			58d77fa8070e8cec2dc1ed015d66b454c8d78850 # 1.12.1 
		)

	# glm 
	# (use a 0.9.9.9 from Aug-2020, because 0.9.9.8 triggers a deprecated feature warning in C++20 in GCC)
	FetchContent_Declare(
		glm
		GIT_REPOSITORY https://github.com/g-truc/glm.git
		GIT_TAG        6fdeff4d67f3db493d47c44da20aa1efaa6574ef # (2020 Aug 06)
	)
	
	# picosha2 - used by hash.h to calculate sha256 hashes
	FetchContent_Declare( 
		picosha2
		GIT_REPOSITORY https://github.com/okdshin/PicoSHA2.git
		GIT_TAG		   27fcf6979298949e8a462e16d09a0351c18fcaf2 # (2022 Aug 08)
	)

	FetchContent_MakeAvailable( 
		googletest 
		glm
		picosha2
		)
		
	# lots of warnings and all warnings as errors
	if(MSVC)
		add_compile_options(/W4)
	else()
		add_compile_options(-Wall -Wextra -pedantic) # -Werror
	endif()

	# unit_tests
	add_executable( 
        unit_tests
		
        ./unit_tests/unit_tests.cpp
        ./unit_tests/unit_tests.h

		./ctle/bimap.h
		./ctle/bitmap_font.h
		./ctle/endianness.h
		./ctle/file_funcs.h
		./ctle/idx_vector.h
		./ctle/log.h
		./ctle/multithread_pool.h
		./ctle/optional_idx_vector.h
		./ctle/optional_value.h
		./ctle/optional_vector.h
		./ctle/prop.h
		./ctle/readers_writer_lock.h
		./ctle/status.h
		./ctle/status_return.h
		./ctle/string_funcs.h
		./ctle/thread_safe_map.h
		./ctle/types.h
		./ctle/uuid.h
		./ctle/hash.h
		./ctle/util.h
		./ctle/sockets.h
		./ctle/_macros.inl
		./ctle/_undef_macros.inl

		./unit_tests/test_bimap.cpp
		./unit_tests/test_bitmap_font.cpp
		./unit_tests/test_endianness.cpp
		./unit_tests/test_file_funcs.cpp
		./unit_tests/test_idx_vector.cpp
		./unit_tests/test_log.cpp
		./unit_tests/test_multithread_pool.cpp
		./unit_tests/test_optional_idx_vector.cpp
		./unit_tests/test_optional_value.cpp
		./unit_tests/test_optional_vector.cpp
		./unit_tests/test_prop.cpp
		./unit_tests/test_readers_writer_lock.cpp
		./unit_tests/test_status.cpp
		./unit_tests/test_status_return.cpp
		./unit_tests/test_string_funcs.cpp
		./unit_tests/test_thread_safe_map.cpp
		./unit_tests/test_types.cpp
		./unit_tests/test_util.cpp
		./unit_tests/test_uuid.cpp
		./unit_tests/test_hash.cpp
		./unit_tests/test_macros.cpp
		./unit_tests/test_sockets.cpp

		./ctle.natvis
		)

	target_include_directories( 
		unit_tests 
		
		PUBLIC ${glm_SOURCE_DIR}
		PUBLIC ${picosha2_SOURCE_DIR}
		)

	if(MSVC)
		target_link_libraries( 	
			unit_tests 
			
			gtest_main 
			ws2_32
			)
	else()
		target_link_libraries( 	
			unit_tests 
			
			gtest_main 
			)
	endif()

# testing
endif(CTLE_BUILD_TESTS)
