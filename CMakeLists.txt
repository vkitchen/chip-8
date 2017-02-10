#
# CMAKELISTS.TXT
# --------------
#
# Copyright (c) 2017 Vaughan Kitchen
#
# Released under the MIT license (https://opensource.org/licenses/MIT)
#

project(CHIP8)

cmake_minimum_required(VERSION 2.6)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CONFIGURATION_TYPES Debug)

set(CHIP8_FILES
	source/file.c
	source/interpreter.c
	source/memory.c
	source/render.c
	source/string2.c
	)

add_library(CHIP8lib ${CHIP8_FILES})

#
# Build any external dependencies
#
include_directories(external/Unity-2.4.0/src)
add_subdirectory(external)

# Find headers
include_directories(source)

add_executable(chip-8 source/main.c)
target_link_libraries(chip-8 CHIP8lib -lSDL2)

#
# Build the SDL test program
#
add_executable(sdltest source/sdltest.c)
target_link_libraries(sdltest -lSDL2)

#
# Build unit tests
#
if(UNIX)
	add_executable(unittest tests/main.c)
	target_link_libraries(unittest unity CHIP8lib)
endif()
