if(NOT CMAKE_TOOLCHAIN_FILE)
message(
    STATUS "Setting toolchain type to vcpkg toolchain file.")
  set(CMAKE_TOOLCHAIN_FILE
  ${vcpkg_SOURCE_DIR}/scripts/buildsystems/vcpkg.cmake
     CACHE STRING "Choose the type of build." FORCE)
  message(PROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}")
  message(CMAKE_TOOLCHAIN_FILE="${CMAKE_TOOLCHAIN_FILE}")
  message(CMAKE_PREFIX_PATH="${CMAKE_PREFIX_PATH}")
endif()

set(CMAKE_CXX_EXTENSIONS OFF)
