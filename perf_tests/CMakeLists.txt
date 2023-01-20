set(PERF_TEST_APP "perf_test")

add_executable(${PERF_TEST_APP})

target_sources(${PERF_TEST_APP} PRIVATE perf_tests.cpp)

target_include_directories(${PERF_TEST_APP} PRIVATE .)

target_link_libraries(${PERF_TEST_APP} PRIVATE ctle)

target_compile_options(${PERF_TEST_APP} PRIVATE ${COMPILER_WARNINGS})