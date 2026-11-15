# ctle
C++ Template Library Extension classes - Extension classes for Windows and Linux STL

## C++ standard checks

`./testall.sh` runs the GoogleTest suites in C++20 and C++17. Its C++14 stage only
compiles and links `ctle_cxx14_compile_check`; it does not run a C++14 test suite.

The compile-only application includes every public ctle header, defines
`CTLE_IMPLEMENTATION`, and enables the optional GLM, Vulkan and hashing support.
It does not link GoogleTest, whose C++17 requirement would otherwise upgrade its
language mode. The target explicitly selects C++14 without compiler extensions,
and a compile-time assertion checks `_MSVC_LANG` or `__cplusplus` to reject any
silent upgrade.

To build this check independently:

```bash
cmake -S . -B build/native -DCTLE_BUILD_TESTS=ON -DCMAKE_CXX_STANDARD=17
cmake --build build/native --target ctle_cxx14_compile_check --parallel 2
```

The target still compiles as C++14 even in this C++17 build directory. It is also
available in the Windows cross-build as
`cmake --build build/windows --target ctle_cxx14_compile_check --parallel 2`.
The application is not executed; this checks compilation and linking, not runtime
behavior or every possible template instantiation.

## Windows cross-build and Wine tests

On Ubuntu/Debian, install the 64-bit POSIX-threaded MinGW toolchain and Wine:

```bash
sudo apt-get install g++-mingw-w64-x86-64-posix ninja-build wine wine64
```

From the repository root, select an isolated Wine prefix. Run these exports in
each new terminal used for the cross-build:

```bash
export WINEPREFIX="$PWD/build/wine-prefix"
export WINEARCH=win64
export WINEDLLOVERRIDES="mscoree,mshtml="
```

Initialize the prefix once, as your normal user, not with sudo:

```bash
mkdir -p "$WINEPREFIX"
wineboot -u
```

The DLL overrides disable optional Mono/Gecko components, which the tests do not
need. The tests are 64-bit and do not require wine32.

Configure a separate Windows build directory and build the tests:

```bash
cmake -S . -B build/windows -G Ninja \
  --toolchain cmake/mingw-w64.cmake \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_CXX_STANDARD=17 \
  -DCTLE_BUILD_TESTS=ON \
  -DCTLE_BUILD_INSTALL=OFF \
  -DCTLE_GENERATE_CODE=OFF
cmake --build build/windows --target unit_tests --parallel 2
```

Do not reuse a native Linux build directory. The toolchain links MinGW runtime
libraries statically; no additional compiler runtime DLLs need to be copied.
Standalone-header checks are included in the test build.

Run the Windows executable from its build directory:

```bash
(cd build/windows && WINEDEBUG=-all wine ./unit_tests.exe \
  --gtest_filter='process_windows_arguments.*:process.*')
(cd build/windows && WINEDEBUG=-all wine ./unit_tests.exe)
```

Omit `WINEDEBUG=-all` when investigating Wine startup failures. For headless
environments that require a display, install `xvfb` and `xauth` and prefix Wine
commands with `xvfb-run -a`. MinGW/Wine testing complements native Windows/MSVC CI.

## license
Copyright (c) 2022-2024 Ulrik Lindahl
Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE
