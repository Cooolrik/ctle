#!/bin/bash

# if any of the tests return error, the script will exit, and return error
set -e

echo "Building & testing C++11"
mkdir -p build_c++11
cd build_c++11
cmake .. -DCMAKE_CXX_STANDARD=11 -DCTLE_BUILD_TESTS=ON
cmake --build .
if [[ "$OSTYPE" == "linux-gnu"* ]]; then
	./unit_tests > ../results_c++11
else
	./Debug/unit_tests > ../results_c++11
fi 
cd ..

echo "Building & testing C++14"
mkdir -p build_c++14
cd build_c++14
cmake .. -DCMAKE_CXX_STANDARD=14 -DCTLE_BUILD_TESTS=ON
cmake --build .
if [[ "$OSTYPE" == "linux-gnu"* ]]; then
	./unit_tests > ../results_c++14
else
	./Debug/unit_tests > ../results_c++14
fi 
cd ..

echo "Building & testing C++17"
mkdir -p build_c++17
cd build_c++17
cmake .. -DCMAKE_CXX_STANDARD=17 -DCTLE_BUILD_TESTS=ON
cmake --build .
if [[ "$OSTYPE" == "linux-gnu"* ]]; then
	./unit_tests > ../results_c++17
else
	./Debug/unit_tests > ../results_c++17
fi 
cd ..

echo "Building & testing C++20"
mkdir -p build_c++20
cd build_c++20
cmake .. -DCMAKE_CXX_STANDARD=20 -DCTLE_BUILD_TESTS=ON
cmake --build .
if [[ "$OSTYPE" == "linux-gnu"* ]]; then
	./unit_tests > ../results_c++20
else
	./Debug/unit_tests > ../results_c++20
fi 
cd ..