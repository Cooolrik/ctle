# ctle
C++ Template Library Extension classes - Extension classes for Windows and Linux STL

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
