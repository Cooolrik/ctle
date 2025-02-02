#!/bin/bash
echo -e "\e[32;49;1m[bootstrap.sh] Generating project in ./build folder...\e[0m"
mkdir -p build
cmake -B ./build -S . -DCMAKE_CXX_STANDARD=14 -DCTLE_BUILD_INSTALL=OFF -DCTLE_BUILD_TESTS=ON -DCTLE_GENERATE_CODE=ON
echo -e "\e[32;49;1m[bootstrap.sh] Building project in ./build folder...\e[0m"
cmake --build ./build