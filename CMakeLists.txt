cmake_minimum_required(VERSION 3.22)

PROJECT(ctle VERSION 0.2 LANGUAGES CXX C)

include(FetchContent)

# vcpkg 
FetchContent_Declare(
	vcpkg
	GIT_REPOSITORY https://github.com/microsoft/vcpkg.git
	GIT_TAG        2023.01.09 #f14984af3738e69f197bf0e647a8dca12de92996 # 2023.01.09
    SOURCE_DIR vcpkg
)

FetchContent_MakeAvailable(vcpkg)

FetchContent_GetProperties(vcpkg)
if(NOT vcpkg_POPULATED)
    message(FATAL_ERROR "Failed to fetch  vcpkg. Aborting....")
else()
    if(WIN32)
        message(STATUS "Run bootstrap-vcpkg")
        execute_process(COMMAND cmd /C "${vcpkg_SOURCE_DIR}/bootstrap-vcpkg.bat" )
        message(STATUS "Installing x64-windows triplet")
        execute_process(COMMAND cmd /C "${vcpkg_SOURCE_DIR}/vcpkg.exe install --triplet x64-windows" )
    else()
        execute_process(COMMAND cmd /C "${vcpkg_SOURCE_DIR}/bootstrap-vcpkg.sh" )
        message(STATUS "Installing x64-linux triplet")
        execute_process(COMMAND cmd /C "${vcpkg_SOURCE_DIR}/vcpkg install --triplet x64-linux" )
    endif()
    # include toolchain file. This is usually setup through command line if vcpkg is a submodule or already checked out in anoter location.
    include(${CMAKE_TOOLCHAIN_FILE})
    
    # enable vcpkg manifest mode
    set(ENV{VCPKG_FEATURE_FLAGS} "manifests")
endif()



# options can be setup in CMakePresets.json/CMkaeUserPrests.json (cacheVariables). Or passed through the command lines.
option(ENABLE_UNIT_TESTING "Enable Test Builds" OFF)
option(ENABLE_PERFORMACE_TESTING "Enable Test Builds" OFF)

# set ctel lib name
set(CTLE_LIB_NAME "ctle")

# common project settings
include(${CMAKE_SOURCE_DIR}/cmake/project_settings.cmake)

# enable cache system with not MSVC
if(NOT MSVC)
    include(${CMAKE_SOURCE_DIR}/cmake/cache_settings.cmake)
endif()

# standard compiler warnings setup
include(${CMAKE_SOURCE_DIR}/cmake/compiler_warnings.cmake)
set_project_warnings(${CTLE_LIB_NAME})

# setup ctle library
add_library(${CTLE_LIB_NAME} INTERFACE)

# target include directories
target_include_directories(${CTLE_LIB_NAME} 
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/ctle
        $<INSTALL_INTERFACE:ctle>
)

# setup public headers
set_target_properties(${CTLE_LIB_NAME} PROPERTIES
    PUBLIC_HEADER
        ctle/bimap.h
        ctle/idx_vector.h
        ctle/log.h
        ctle/optional_idx_vector.h
        ctle/optional_value.h
        ctle/optional_vector.h
        ctle/readers_writer_lock.h
        ctle/status_return.h
        ctle/status.h
        ctle/string_funcs.h
        ctle/thread_safe_map.h
)

if(ENABLE_UNIT_TESTING)
  enable_testing()
  include(CTest)
  add_subdirectory(unit_tests)
endif()

if(ENABLE_PERFORMACE_TESTING)
  add_subdirectory(perf_tests)
endif()

include(GNUInstallDirs)

install(
    TARGETS ${CTLE_LIB_NAME} 
    EXPORT ${CTLE_LIB_NAME}-config
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/ctle
)
        
install(
    EXPORT ${CTLE_LIB_NAME}-config
    NAMESPACE ${CTLE_LIB_NAME}::
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${CTLE_LIB_NAME}
)
             


