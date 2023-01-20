set(UNIT_TEST_APP "unit_test")

find_package(GTest CONFIG REQUIRED)

include(GoogleTest)

add_executable(${UNIT_TEST_APP})

target_sources(${UNIT_TEST_APP} 
    PRIVATE 
        utils.cpp 
        BasicTests.cpp)

target_include_directories(${UNIT_TEST_APP} PRIVATE .)

gtest_discover_tests(${UNIT_TEST_APP})

target_link_libraries(${UNIT_TEST_APP} PRIVATE GTest::gmock GTest::gtest GTest::gmock_main GTest::gtest_main ctle)

target_compile_options(${UNIT_TEST_APP} PRIVATE ${COMPILER_WARNINGS})

target_precompile_headers(${UNIT_TEST_APP} 
    PRIVATE
        ${CMAKE_SOURCE_DIR}/ctle/idx_vector.h
        ${CMAKE_SOURCE_DIR}/ctle/optional_idx_vector.h
        ${CMAKE_SOURCE_DIR}/ctle/optional_value.h
        ${CMAKE_SOURCE_DIR}/ctle/optional_vector.h
        ${CMAKE_SOURCE_DIR}/ctle/thread_safe_map.h
        ${CMAKE_SOURCE_DIR}/ctle/bimap.h
        ${CMAKE_SOURCE_DIR}/ctle/string_funcs.h
        ${CMAKE_SOURCE_DIR}/ctle/status_return.h
    PRIVATE 
        <stddef.h>
        <stdlib.h>
        <time.h>
        <sstream>
        <functional>
    PRIVATE
        utils.h
)