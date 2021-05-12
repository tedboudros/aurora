
## Setup
#### Windows:
- Install **MSYS2** if necessary, for example, in the folder 'C:\msys64'. 
- Run C:\msys64\\**mingw64**.exe (not **msys2**.exe).
- Update MSYS2 (may be necessary to run several times):
`pacman -Syuu`

- Install GCC
`pacman -S base-devel gcc`

- Install CMake, make, SDL, SDL2_Image:
`pacman -S mingw-w64-x86_64-cmake mingw-w64-x86_64-make make mingw-w64-x86_64-SDL2 mingw-w64-x86_64-SDL2_image`

#### Linux:

- Install CMake:
`sudo apt install cmake`

- Install SDL2 and SDL2_Image:
`sudo apt install libsdl2-dev libsdl2-2.0-0 libsdl2-image-dev libsdl2-image-2.0-0`

## Building

For example, we have the project files in "Projects/aurora" and the current folder is "Projects". 
Next we create a separate folder for build files:

`mkdir build-debug && cd build-debug`
or
`mkdir build-release && cd build-release`

And then run CMake like:

`cmake ../aurora/ -DCMAKE_BUILD_TYPE=Debug`
or
`cmake ../aurora/ -DCMAKE_BUILD_TYPE=Release`

On **Windows** it may be necessary to add the MinGW generator (**-G**) because if the Visual Studio compiler is installed then CMake will use it by default:

`cmake -G "MinGW Makefiles" ../aurora/ -DCMAKE_BUILD_TYPE=Debug`
or
`cmake -G "MinGW Makefiles" ../aurora/ -DCMAKE_BUILD_TYPE=Release`

If the build type/configuration isn't specified then CMakeLists.txt will use "Debug" by default.

Building the project:
`cmake --build .`

Installing/copying files to bin/Debug or bin/Release:
`cmake --install .`

Creating a zip-file for the current bin/<Config>:
`cmake --build . --target make_zip`
