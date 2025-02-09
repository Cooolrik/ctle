cmake_minimum_required(VERSION 3.21)
file(READ "./version" CTLE_VERSION)
project( 
	"ctle" 
	VERSION 		"${CTLE_VERSION}"
	DESCRIPTION 	"A header-only library with template extensions for Linux and Windows. Only depends on standard template library."
	HOMEPAGE_URL 	"https://github.com/Cooolrik/ctle"
	)
message(STATUS "ctle library version: ${PROJECT_VERSION}")
include(GNUInstallDirs)

# if this is the main project, create test project as default, and install it
option(CTLE_BUILD_INSTALL "Build the library install make path" ON )
option(CTLE_BUILD_TESTS "Build all tests" OFF )
option(CTLE_GENERATE_CODE "Run code generation in the cmake process" OFF )

# if selected, generate dynamic code 
if(CTLE_GENERATE_CODE) 
	find_package(PythonInterp 3.10 REQUIRED)
	find_package(Python 3.10 REQUIRED)
	execute_process( 
		COMMAND 			${PYTHON_EXECUTABLE} generate.py
		WORKING_DIRECTORY 	${CMAKE_CURRENT_LIST_DIR}/code_gen
		RESULT_VARIABLE 	py_result
	)
	message(STATUS "Result of running code gen - generate.py: ${py_result}")
endif()

# list all header and inline files in 'include/ctle' into CTLE_FILE_SET. This is the library in full.
file(GLOB CTLE_FILE_SET CONFIGURE_DEPENDS "include/ctle/*.h" "include/ctle/*.inl")

# add our library, define as a header-only library
add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(
	${PROJECT_NAME} 
	INTERFACE 	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> 
				$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
	)

# if selected, generate the install path
if(CTLE_BUILD_INSTALL)
	message(STATUS "Building the library install path")

	install(
		TARGETS ${PROJECT_NAME} 
		EXPORT 	${PROJECT_NAME}_targets
		ARCHIVE	DESTINATION	${CMAKE_INSTALL_LIBDIR}
		LIBRARY	DESTINATION ${CMAKE_INSTALL_LIBDIR}
		RUNTIME	DESTINATION	${CMAKE_INSTALL_BINDIR}
		)

	include(CMakePackageConfigHelpers)
		write_basic_package_version_file(
		"${PROJECT_NAME}ConfigVersion.cmake"
		VERSION			${PROJECT_VERSION}
		COMPATIBILITY 	SameMajorVersion
		)

	configure_package_config_file(
		"${PROJECT_SOURCE_DIR}/cmake/CtleProjectConfig.cmake.in"
		"${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
		INSTALL_DESTINATION 	${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake
		)

	install(
		EXPORT 		${PROJECT_NAME}_targets
		FILE 		${PROJECT_NAME}Targets.cmake
		NAMESPACE 	${PROJECT_NAME}::
		DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake
		)

	install(
		FILES 		${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
					${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
		DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake
		)

	install(
		DIRECTORY 	${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}
		DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
		)
endif(CTLE_BUILD_INSTALL)

# if selected, build the testing code
if(CTLE_BUILD_TESTS)			
	message(STATUS "Creating ctle test project")

	# (note: the fetched modules are only needed to test behavior and interops, not needed for use in a project)
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
	
	# picosha2 - used by hasher.h to calculate sha256 hashes
	FetchContent_Declare( 
		picosha2
		GIT_REPOSITORY https://github.com/okdshin/PicoSHA2.git
		GIT_TAG		   27fcf6979298949e8a462e16d09a0351c18fcaf2 # (2022 Aug 08)
	)

	# xxHash - used by hasher.h to calculate xxHash hashes XXH64 and XXH128
	FetchContent_Declare( 
		xxhash
		GIT_REPOSITORY https://github.com/Cyan4973/xxHash.git
		GIT_TAG		   bbb27a5efb85b92a0486cf361a8635715a53f6ba # (2023 Jul 21)
	)

	FetchContent_MakeAvailable( 
		googletest 
		glm
		picosha2
		xxhash
		)
		
	# level 4 warnings
	if(MSVC)
		add_compile_options(/W4 /bigobj)
	else()
		add_compile_options(-Wall -Wextra -pedantic)
	endif()

	# list all header and inline files in 'include/ctle'. This is the library in full.
	file(GLOB CTLE_TEST_FILE_SET CONFIGURE_DEPENDS "./unit_tests/*.h" "./unit_tests/*.cpp")

	# unit_tests
	add_executable( 
        unit_tests
		
		# the ctle library files
		${CTLE_FILE_SET}

		# the test files
		${CTLE_TEST_FILE_SET}

		# dependencies
		${xxhash_SOURCE_DIR}/xxhash.c

		./ctle.natvis
		)

	target_include_directories( 
		unit_tests 
		
		PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include
		PUBLIC ${glm_SOURCE_DIR}
		PUBLIC ${picosha2_SOURCE_DIR}
		PUBLIC ${xxhash_SOURCE_DIR}
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



