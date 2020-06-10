### Windows

This guide assumes using MinGW-w64 with GCC 9.2.
Unfortunately, official builds are not provided, but mingw-w64-x86_64-9.2.0-7.0.0-r4.7z (or whatever the latest stable build is) provided at winlibs.com works nicely.

For other compilers, the following steps should be almost identical - you need to change `-G "generator name"` [generator command](https://cmake.org/cmake/help/latest/manual/cmake-generators.7.html) to your needs.

You need to download and build SDL by yourself and point CMake where the library should be installed.
Use e.g. `c:/path/to/compiler/cmake_install`, Program Files is a bad idea.
To do this, do following:

```shell
git clone https://github.com/spurious/SDL-mirror c:/path/to/sdl
cd /d c:/path/to/sdl
mkdir build && cd build
cmake .. -G "MinGW Makefiles" -DCMAKE_INSTALL_PREFIX=c:/path/to/cmake_install -DSDL_STATIC=ON
cmake --build .
cmake --install . # on older versions: cmake --build . --target install
```

Be sure to configure `CMAKE_PREFIX_PATH` envVar to c:/path/to/cmake_install!
Without it, CMake won't be able to find installed libraries.
Then, do the following:

```shell
git clone https://github.com/Skarmux/X-Settlers c:/path/to/X-Settlers
cd /d c:/path/to/X-Settlers
mkdir build && cd build
cmake .. -G "MinGW Makefiles" -DCMAKE_INSTALL_PREFIX=c:/path/to/cmake_install
cmake --build .
```

### Linux

You can specify renderer by passing `-DRENDERER=OpenGL` or `Vulkan`. Example build commands:

```shell
sudo apt install libsdl2-dev
cd build && mkdir build
cmake .. -DRENDERER=OpenGL
cmake --build .
```