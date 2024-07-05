echo "Setting up dev environment in build folder"
mkdir -p build
cd build
cmake .. -DCMAKE_CXX_STANDARD=17 -DCTLE_BUILD_TESTS=ON
cmake --build .
cd ..
