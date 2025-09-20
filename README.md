# Behin Projects

![build](https://img.shields.io/badge/build-passing-green)
![License](https://img.shields.io/badge/License-GPL3-8A2BE2)
![C++](https://img.shields.io/badge/c%2B%2B-14%2F17%2F20-orange)
    
**Behin**.

- Library, executable and test code separated in distinct folders
- Use of modern CMake for building and compiling
- Unit testing using [Catch2](https://github.com/catchorg/Catch2)
- General purpose libraries: [Qt6](https://www.qt.io/product/qt6)
- Continuous integration testing with Github Actions and [pre-commit](https://pre-commit.com/)
- Code documentation with [Doxygen](https://doxygen.nl/) and [Github Pages](http://192.168.11.187:8989/behin)
- Tooling: Clang-Format, Cmake-Format, Clang-tidy, Sanitizers

## Structure

``` text
├── CMakeLists.txt
├── app
│   ├── CMakesLists.txt
│   └── main.cc
│
├── cmake
│   └── cmake modules
├── docs
│   ├── Doxyfile
│   └── html/
├── external
│   ├── CMakesLists.txt
│   ├── ...
├── src
│   ├── CMakesLists.txt
│   ├── src/...
│
└── tests
    ├── CMakeLists.txt
    └── test_*.cc
```

Library code goes into [src/](src/), main program code in [app/](app) and tests go in [tests/](tests/).

## Software Requirements

- Git 2.34.1+
- CMake 3.21+
- GNU Makefile
- Doxygen
- MSVC 2017 (or higher), G++9 (or higher), Clang++9 (or higher)
- Optional: Code Coverage (only on GNU|Clang): gcovr
- Optional: Makefile, Doxygen, Conan, VCPKG

## Building

First, clone this repo and do the preliminary work:

```shell
git clone --recursive http://192.168.11.187:8989/behin
mkdir build
```

- App Executable

```shell
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build . --config Release --target main
cd app
./main
```

- Unit testing

```shell
cmake -H. -Bbuild -DCMAKE_BUILD_TYPE="Debug"
cmake --build build --config Debug
cd build
ctest .
```

- Documentation

```shell
cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake --build . --config Debug --target docs
```

- Code Coverage (Unix only)

```shell
cmake -H. -Bbuild -DCMAKE_BUILD_TYPE=Debug -DENABLE_COVERAGE=On
cmake --build build --config Debug --target coverage -j4
cd build
ctest .
```

For more info about CMake see [here](./README_cmake.md).
