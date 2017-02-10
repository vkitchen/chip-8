# Chip 8 Emulator

## Build Instructions for Linux

Dependencies are CMake, Doxygen and SDL

### To build (Linux)

In project root run

1. mkdir build
2. cd build
3. cmake ..
4. make
5. ./chip-8

### To build (Windows) BROKEN

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


### Playing Games

Currently the CHIP-8 keyboard is aligned with the left side of a Qwerty layout. This can be modified in keys.h

The CHIP-8 keyboard is layed out as so

	1 2 3 C
	4 5 6 D
	7 8 9 E
	A 0 B F

Which corresponds to

	1 2 3 4
	q w e r
	a s d f
	z x c v
