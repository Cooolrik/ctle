set(UNIT_TEST_APP "unit_tests")

find_package(GTest CONFIG REQUIRED)

include(GoogleTest)

add_executable(${UNIT_TEST_APP})

target_sources(${UNIT_TEST_APP} 
    PRIVATE 
        unit_tests.h
        unit_tests.cpp 
        basic_tests.cpp)

target_include_directories(${UNIT_TEST_APP} PRIVATE .)

target_link_libraries(${UNIT_TEST_APP} PRIVATE GTest::gtest GTest::gtest_main ctle)

target_compile_options(${UNIT_TEST_APP} PRIVATE ${COMPILER_WARNINGS})

gtest_discover_tests(${UNIT_TEST_APP})

