# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.6

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/tim/Desktop/chip-8-master

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/tim/Desktop/chip-8-master/build

# Include any dependencies generated for this target.
include CMakeFiles/CHIP8lib.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/CHIP8lib.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/CHIP8lib.dir/flags.make

CMakeFiles/CHIP8lib.dir/source/main.c.o: CMakeFiles/CHIP8lib.dir/flags.make
CMakeFiles/CHIP8lib.dir/source/main.c.o: ../source/main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tim/Desktop/chip-8-master/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/CHIP8lib.dir/source/main.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/CHIP8lib.dir/source/main.c.o   -c /home/tim/Desktop/chip-8-master/source/main.c

CMakeFiles/CHIP8lib.dir/source/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/CHIP8lib.dir/source/main.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/tim/Desktop/chip-8-master/source/main.c > CMakeFiles/CHIP8lib.dir/source/main.c.i

CMakeFiles/CHIP8lib.dir/source/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/CHIP8lib.dir/source/main.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/tim/Desktop/chip-8-master/source/main.c -o CMakeFiles/CHIP8lib.dir/source/main.c.s

CMakeFiles/CHIP8lib.dir/source/main.c.o.requires:

.PHONY : CMakeFiles/CHIP8lib.dir/source/main.c.o.requires

CMakeFiles/CHIP8lib.dir/source/main.c.o.provides: CMakeFiles/CHIP8lib.dir/source/main.c.o.requires
	$(MAKE) -f CMakeFiles/CHIP8lib.dir/build.make CMakeFiles/CHIP8lib.dir/source/main.c.o.provides.build
.PHONY : CMakeFiles/CHIP8lib.dir/source/main.c.o.provides

CMakeFiles/CHIP8lib.dir/source/main.c.o.provides.build: CMakeFiles/CHIP8lib.dir/source/main.c.o


CMakeFiles/CHIP8lib.dir/source/memory.c.o: CMakeFiles/CHIP8lib.dir/flags.make
CMakeFiles/CHIP8lib.dir/source/memory.c.o: ../source/memory.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tim/Desktop/chip-8-master/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/CHIP8lib.dir/source/memory.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/CHIP8lib.dir/source/memory.c.o   -c /home/tim/Desktop/chip-8-master/source/memory.c

CMakeFiles/CHIP8lib.dir/source/memory.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/CHIP8lib.dir/source/memory.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/tim/Desktop/chip-8-master/source/memory.c > CMakeFiles/CHIP8lib.dir/source/memory.c.i

CMakeFiles/CHIP8lib.dir/source/memory.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/CHIP8lib.dir/source/memory.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/tim/Desktop/chip-8-master/source/memory.c -o CMakeFiles/CHIP8lib.dir/source/memory.c.s

CMakeFiles/CHIP8lib.dir/source/memory.c.o.requires:

.PHONY : CMakeFiles/CHIP8lib.dir/source/memory.c.o.requires

CMakeFiles/CHIP8lib.dir/source/memory.c.o.provides: CMakeFiles/CHIP8lib.dir/source/memory.c.o.requires
	$(MAKE) -f CMakeFiles/CHIP8lib.dir/build.make CMakeFiles/CHIP8lib.dir/source/memory.c.o.provides.build
.PHONY : CMakeFiles/CHIP8lib.dir/source/memory.c.o.provides

CMakeFiles/CHIP8lib.dir/source/memory.c.o.provides.build: CMakeFiles/CHIP8lib.dir/source/memory.c.o


CMakeFiles/CHIP8lib.dir/source/string2.c.o: CMakeFiles/CHIP8lib.dir/flags.make
CMakeFiles/CHIP8lib.dir/source/string2.c.o: ../source/string2.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tim/Desktop/chip-8-master/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/CHIP8lib.dir/source/string2.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/CHIP8lib.dir/source/string2.c.o   -c /home/tim/Desktop/chip-8-master/source/string2.c

CMakeFiles/CHIP8lib.dir/source/string2.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/CHIP8lib.dir/source/string2.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/tim/Desktop/chip-8-master/source/string2.c > CMakeFiles/CHIP8lib.dir/source/string2.c.i

CMakeFiles/CHIP8lib.dir/source/string2.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/CHIP8lib.dir/source/string2.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/tim/Desktop/chip-8-master/source/string2.c -o CMakeFiles/CHIP8lib.dir/source/string2.c.s

CMakeFiles/CHIP8lib.dir/source/string2.c.o.requires:

.PHONY : CMakeFiles/CHIP8lib.dir/source/string2.c.o.requires

CMakeFiles/CHIP8lib.dir/source/string2.c.o.provides: CMakeFiles/CHIP8lib.dir/source/string2.c.o.requires
	$(MAKE) -f CMakeFiles/CHIP8lib.dir/build.make CMakeFiles/CHIP8lib.dir/source/string2.c.o.provides.build
.PHONY : CMakeFiles/CHIP8lib.dir/source/string2.c.o.provides

CMakeFiles/CHIP8lib.dir/source/string2.c.o.provides.build: CMakeFiles/CHIP8lib.dir/source/string2.c.o


# Object files for target CHIP8lib
CHIP8lib_OBJECTS = \
"CMakeFiles/CHIP8lib.dir/source/main.c.o" \
"CMakeFiles/CHIP8lib.dir/source/memory.c.o" \
"CMakeFiles/CHIP8lib.dir/source/string2.c.o"

# External object files for target CHIP8lib
CHIP8lib_EXTERNAL_OBJECTS =

libCHIP8lib.a: CMakeFiles/CHIP8lib.dir/source/main.c.o
libCHIP8lib.a: CMakeFiles/CHIP8lib.dir/source/memory.c.o
libCHIP8lib.a: CMakeFiles/CHIP8lib.dir/source/string2.c.o
libCHIP8lib.a: CMakeFiles/CHIP8lib.dir/build.make
libCHIP8lib.a: CMakeFiles/CHIP8lib.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/tim/Desktop/chip-8-master/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking C static library libCHIP8lib.a"
	$(CMAKE_COMMAND) -P CMakeFiles/CHIP8lib.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/CHIP8lib.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/CHIP8lib.dir/build: libCHIP8lib.a

.PHONY : CMakeFiles/CHIP8lib.dir/build

CMakeFiles/CHIP8lib.dir/requires: CMakeFiles/CHIP8lib.dir/source/main.c.o.requires
CMakeFiles/CHIP8lib.dir/requires: CMakeFiles/CHIP8lib.dir/source/memory.c.o.requires
CMakeFiles/CHIP8lib.dir/requires: CMakeFiles/CHIP8lib.dir/source/string2.c.o.requires

.PHONY : CMakeFiles/CHIP8lib.dir/requires

CMakeFiles/CHIP8lib.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/CHIP8lib.dir/cmake_clean.cmake
.PHONY : CMakeFiles/CHIP8lib.dir/clean

CMakeFiles/CHIP8lib.dir/depend:
	cd /home/tim/Desktop/chip-8-master/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/tim/Desktop/chip-8-master /home/tim/Desktop/chip-8-master /home/tim/Desktop/chip-8-master/build /home/tim/Desktop/chip-8-master/build /home/tim/Desktop/chip-8-master/build/CMakeFiles/CHIP8lib.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/CHIP8lib.dir/depend

