# Chip 8 Emulator

## Build Instructions for Linux

Dependencies are CMake and Doxygen

### To build (Linux)

In project root run

1. mkdir build
2. cd build
3. cmake ..
4. make
5. ./chip-8

### To build (Windows)

Requires Visual 14 2015

1. mkdir build
2. cd build
3. cmake -G "NMake Makefiles" ..
4. nmake
5. chip-8


### To clean

either `rm -R build` or `git clean -fdx`

### To generate docs

In project root run `doxygen`
