# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

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
CMAKE_SOURCE_DIR = /home/name/git_files

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/name/git_files/build

# Include any dependencies generated for this target.
include apps/CMakeFiles/network_simulator.dir/depend.make

# Include the progress variables for this target.
include apps/CMakeFiles/network_simulator.dir/progress.make

# Include the compile flags for this target's objects.
include apps/CMakeFiles/network_simulator.dir/flags.make

apps/CMakeFiles/network_simulator.dir/network_simulator.cc.o: apps/CMakeFiles/network_simulator.dir/flags.make
apps/CMakeFiles/network_simulator.dir/network_simulator.cc.o: ../apps/network_simulator.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/name/git_files/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object apps/CMakeFiles/network_simulator.dir/network_simulator.cc.o"
	cd /home/name/git_files/build/apps && /usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/network_simulator.dir/network_simulator.cc.o -c /home/name/git_files/apps/network_simulator.cc

apps/CMakeFiles/network_simulator.dir/network_simulator.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/network_simulator.dir/network_simulator.cc.i"
	cd /home/name/git_files/build/apps && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/name/git_files/apps/network_simulator.cc > CMakeFiles/network_simulator.dir/network_simulator.cc.i

apps/CMakeFiles/network_simulator.dir/network_simulator.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/network_simulator.dir/network_simulator.cc.s"
	cd /home/name/git_files/build/apps && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/name/git_files/apps/network_simulator.cc -o CMakeFiles/network_simulator.dir/network_simulator.cc.s

# Object files for target network_simulator
network_simulator_OBJECTS = \
"CMakeFiles/network_simulator.dir/network_simulator.cc.o"

# External object files for target network_simulator
network_simulator_EXTERNAL_OBJECTS =

apps/network_simulator: apps/CMakeFiles/network_simulator.dir/network_simulator.cc.o
apps/network_simulator: apps/CMakeFiles/network_simulator.dir/build.make
apps/network_simulator: libsponge/libsponge.a
apps/network_simulator: /usr/lib/x86_64-linux-gnu/libpthread.so
apps/network_simulator: apps/CMakeFiles/network_simulator.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/name/git_files/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable network_simulator"
	cd /home/name/git_files/build/apps && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/network_simulator.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
apps/CMakeFiles/network_simulator.dir/build: apps/network_simulator

.PHONY : apps/CMakeFiles/network_simulator.dir/build

apps/CMakeFiles/network_simulator.dir/clean:
	cd /home/name/git_files/build/apps && $(CMAKE_COMMAND) -P CMakeFiles/network_simulator.dir/cmake_clean.cmake
.PHONY : apps/CMakeFiles/network_simulator.dir/clean

apps/CMakeFiles/network_simulator.dir/depend:
	cd /home/name/git_files/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/name/git_files /home/name/git_files/apps /home/name/git_files/build /home/name/git_files/build/apps /home/name/git_files/build/apps/CMakeFiles/network_simulator.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : apps/CMakeFiles/network_simulator.dir/depend

