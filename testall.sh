#!/bin/bash

echo "Building & testing C++11"
mkdir build_c++11
cd build_c++11
cmake .. -DCMAKE_CXX_STANDARD=11 -DCTLE_BUILD_TESTS=ON
cmake --build .
./unit_tests > ../results_c++11
cd ..

echo "Building & testing C++14"
mkdir build_c++14
cd build_c++14
cmake .. -DCMAKE_CXX_STANDARD=14 -DCTLE_BUILD_TESTS=ON
cmake --build .
./unit_tests > ../results_c++14
cd ..

echo "Building & testing C++17"
mkdir build_c++17
cd build_c++17
cmake .. -DCMAKE_CXX_STANDARD=17 -DCTLE_BUILD_TESTS=ON
cmake --build .
./unit_tests > ../results_c++17
cd ..

echo "Building & testing C++20"
mkdir build_c++20
cd build_c++20
cmake .. -DCMAKE_CXX_STANDARD=20 -DCTLE_BUILD_TESTS=ON
cmake --build .
./unit_tests > ../results_c++20
cd ..