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
include tests/CMakeFiles/fsm_stream_reassembler_single.dir/depend.make

# Include the progress variables for this target.
include tests/CMakeFiles/fsm_stream_reassembler_single.dir/progress.make

# Include the compile flags for this target's objects.
include tests/CMakeFiles/fsm_stream_reassembler_single.dir/flags.make

tests/CMakeFiles/fsm_stream_reassembler_single.dir/fsm_stream_reassembler_single.cc.o: tests/CMakeFiles/fsm_stream_reassembler_single.dir/flags.make
tests/CMakeFiles/fsm_stream_reassembler_single.dir/fsm_stream_reassembler_single.cc.o: ../tests/fsm_stream_reassembler_single.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/name/git_files/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object tests/CMakeFiles/fsm_stream_reassembler_single.dir/fsm_stream_reassembler_single.cc.o"
	cd /home/name/git_files/build/tests && /usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/fsm_stream_reassembler_single.dir/fsm_stream_reassembler_single.cc.o -c /home/name/git_files/tests/fsm_stream_reassembler_single.cc

tests/CMakeFiles/fsm_stream_reassembler_single.dir/fsm_stream_reassembler_single.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/fsm_stream_reassembler_single.dir/fsm_stream_reassembler_single.cc.i"
	cd /home/name/git_files/build/tests && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/name/git_files/tests/fsm_stream_reassembler_single.cc > CMakeFiles/fsm_stream_reassembler_single.dir/fsm_stream_reassembler_single.cc.i

tests/CMakeFiles/fsm_stream_reassembler_single.dir/fsm_stream_reassembler_single.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/fsm_stream_reassembler_single.dir/fsm_stream_reassembler_single.cc.s"
	cd /home/name/git_files/build/tests && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/name/git_files/tests/fsm_stream_reassembler_single.cc -o CMakeFiles/fsm_stream_reassembler_single.dir/fsm_stream_reassembler_single.cc.s

# Object files for target fsm_stream_reassembler_single
fsm_stream_reassembler_single_OBJECTS = \
"CMakeFiles/fsm_stream_reassembler_single.dir/fsm_stream_reassembler_single.cc.o"

# External object files for target fsm_stream_reassembler_single
fsm_stream_reassembler_single_EXTERNAL_OBJECTS =

tests/fsm_stream_reassembler_single: tests/CMakeFiles/fsm_stream_reassembler_single.dir/fsm_stream_reassembler_single.cc.o
tests/fsm_stream_reassembler_single: tests/CMakeFiles/fsm_stream_reassembler_single.dir/build.make
tests/fsm_stream_reassembler_single: tests/libspongechecks.a
tests/fsm_stream_reassembler_single: libsponge/libsponge.a
tests/fsm_stream_reassembler_single: tests/CMakeFiles/fsm_stream_reassembler_single.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/name/git_files/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable fsm_stream_reassembler_single"
	cd /home/name/git_files/build/tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/fsm_stream_reassembler_single.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tests/CMakeFiles/fsm_stream_reassembler_single.dir/build: tests/fsm_stream_reassembler_single

.PHONY : tests/CMakeFiles/fsm_stream_reassembler_single.dir/build

tests/CMakeFiles/fsm_stream_reassembler_single.dir/clean:
	cd /home/name/git_files/build/tests && $(CMAKE_COMMAND) -P CMakeFiles/fsm_stream_reassembler_single.dir/cmake_clean.cmake
.PHONY : tests/CMakeFiles/fsm_stream_reassembler_single.dir/clean

tests/CMakeFiles/fsm_stream_reassembler_single.dir/depend:
	cd /home/name/git_files/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/name/git_files /home/name/git_files/tests /home/name/git_files/build /home/name/git_files/build/tests /home/name/git_files/build/tests/CMakeFiles/fsm_stream_reassembler_single.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tests/CMakeFiles/fsm_stream_reassembler_single.dir/depend

