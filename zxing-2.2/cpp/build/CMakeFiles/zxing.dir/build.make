# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

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
CMAKE_COMMAND = "/Applications/CMake 2.8-11.app/Contents/bin/cmake"

# The command to remove a file.
RM = "/Applications/CMake 2.8-11.app/Contents/bin/cmake" -E remove -f

# Escaping for special characters.
EQUALS = =

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = "/Applications/CMake 2.8-11.app/Contents/bin/ccmake"

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/arvind/git/stripes-prototyping/zxing-2.2/cpp

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/arvind/git/stripes-prototyping/zxing-2.2/cpp/build

# Include any dependencies generated for this target.
include CMakeFiles/zxing.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/zxing.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/zxing.dir/flags.make

CMakeFiles/zxing.dir/cli/src/ImageReaderSource.cpp.o: CMakeFiles/zxing.dir/flags.make
CMakeFiles/zxing.dir/cli/src/ImageReaderSource.cpp.o: ../cli/src/ImageReaderSource.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/arvind/git/stripes-prototyping/zxing-2.2/cpp/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/zxing.dir/cli/src/ImageReaderSource.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/zxing.dir/cli/src/ImageReaderSource.cpp.o -c /Users/arvind/git/stripes-prototyping/zxing-2.2/cpp/cli/src/ImageReaderSource.cpp

CMakeFiles/zxing.dir/cli/src/ImageReaderSource.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/zxing.dir/cli/src/ImageReaderSource.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/arvind/git/stripes-prototyping/zxing-2.2/cpp/cli/src/ImageReaderSource.cpp > CMakeFiles/zxing.dir/cli/src/ImageReaderSource.cpp.i

CMakeFiles/zxing.dir/cli/src/ImageReaderSource.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/zxing.dir/cli/src/ImageReaderSource.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/arvind/git/stripes-prototyping/zxing-2.2/cpp/cli/src/ImageReaderSource.cpp -o CMakeFiles/zxing.dir/cli/src/ImageReaderSource.cpp.s

CMakeFiles/zxing.dir/cli/src/ImageReaderSource.cpp.o.requires:
.PHONY : CMakeFiles/zxing.dir/cli/src/ImageReaderSource.cpp.o.requires

CMakeFiles/zxing.dir/cli/src/ImageReaderSource.cpp.o.provides: CMakeFiles/zxing.dir/cli/src/ImageReaderSource.cpp.o.requires
	$(MAKE) -f CMakeFiles/zxing.dir/build.make CMakeFiles/zxing.dir/cli/src/ImageReaderSource.cpp.o.provides.build
.PHONY : CMakeFiles/zxing.dir/cli/src/ImageReaderSource.cpp.o.provides

CMakeFiles/zxing.dir/cli/src/ImageReaderSource.cpp.o.provides.build: CMakeFiles/zxing.dir/cli/src/ImageReaderSource.cpp.o

CMakeFiles/zxing.dir/cli/src/jpgd.cpp.o: CMakeFiles/zxing.dir/flags.make
CMakeFiles/zxing.dir/cli/src/jpgd.cpp.o: ../cli/src/jpgd.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/arvind/git/stripes-prototyping/zxing-2.2/cpp/build/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/zxing.dir/cli/src/jpgd.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/zxing.dir/cli/src/jpgd.cpp.o -c /Users/arvind/git/stripes-prototyping/zxing-2.2/cpp/cli/src/jpgd.cpp

CMakeFiles/zxing.dir/cli/src/jpgd.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/zxing.dir/cli/src/jpgd.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/arvind/git/stripes-prototyping/zxing-2.2/cpp/cli/src/jpgd.cpp > CMakeFiles/zxing.dir/cli/src/jpgd.cpp.i

CMakeFiles/zxing.dir/cli/src/jpgd.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/zxing.dir/cli/src/jpgd.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/arvind/git/stripes-prototyping/zxing-2.2/cpp/cli/src/jpgd.cpp -o CMakeFiles/zxing.dir/cli/src/jpgd.cpp.s

CMakeFiles/zxing.dir/cli/src/jpgd.cpp.o.requires:
.PHONY : CMakeFiles/zxing.dir/cli/src/jpgd.cpp.o.requires

CMakeFiles/zxing.dir/cli/src/jpgd.cpp.o.provides: CMakeFiles/zxing.dir/cli/src/jpgd.cpp.o.requires
	$(MAKE) -f CMakeFiles/zxing.dir/build.make CMakeFiles/zxing.dir/cli/src/jpgd.cpp.o.provides.build
.PHONY : CMakeFiles/zxing.dir/cli/src/jpgd.cpp.o.provides

CMakeFiles/zxing.dir/cli/src/jpgd.cpp.o.provides.build: CMakeFiles/zxing.dir/cli/src/jpgd.cpp.o

CMakeFiles/zxing.dir/cli/src/lodepng.cpp.o: CMakeFiles/zxing.dir/flags.make
CMakeFiles/zxing.dir/cli/src/lodepng.cpp.o: ../cli/src/lodepng.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/arvind/git/stripes-prototyping/zxing-2.2/cpp/build/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/zxing.dir/cli/src/lodepng.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/zxing.dir/cli/src/lodepng.cpp.o -c /Users/arvind/git/stripes-prototyping/zxing-2.2/cpp/cli/src/lodepng.cpp

CMakeFiles/zxing.dir/cli/src/lodepng.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/zxing.dir/cli/src/lodepng.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/arvind/git/stripes-prototyping/zxing-2.2/cpp/cli/src/lodepng.cpp > CMakeFiles/zxing.dir/cli/src/lodepng.cpp.i

CMakeFiles/zxing.dir/cli/src/lodepng.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/zxing.dir/cli/src/lodepng.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/arvind/git/stripes-prototyping/zxing-2.2/cpp/cli/src/lodepng.cpp -o CMakeFiles/zxing.dir/cli/src/lodepng.cpp.s

CMakeFiles/zxing.dir/cli/src/lodepng.cpp.o.requires:
.PHONY : CMakeFiles/zxing.dir/cli/src/lodepng.cpp.o.requires

CMakeFiles/zxing.dir/cli/src/lodepng.cpp.o.provides: CMakeFiles/zxing.dir/cli/src/lodepng.cpp.o.requires
	$(MAKE) -f CMakeFiles/zxing.dir/build.make CMakeFiles/zxing.dir/cli/src/lodepng.cpp.o.provides.build
.PHONY : CMakeFiles/zxing.dir/cli/src/lodepng.cpp.o.provides

CMakeFiles/zxing.dir/cli/src/lodepng.cpp.o.provides.build: CMakeFiles/zxing.dir/cli/src/lodepng.cpp.o

CMakeFiles/zxing.dir/cli/src/main.cpp.o: CMakeFiles/zxing.dir/flags.make
CMakeFiles/zxing.dir/cli/src/main.cpp.o: ../cli/src/main.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/arvind/git/stripes-prototyping/zxing-2.2/cpp/build/CMakeFiles $(CMAKE_PROGRESS_4)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/zxing.dir/cli/src/main.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/zxing.dir/cli/src/main.cpp.o -c /Users/arvind/git/stripes-prototyping/zxing-2.2/cpp/cli/src/main.cpp

CMakeFiles/zxing.dir/cli/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/zxing.dir/cli/src/main.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/arvind/git/stripes-prototyping/zxing-2.2/cpp/cli/src/main.cpp > CMakeFiles/zxing.dir/cli/src/main.cpp.i

CMakeFiles/zxing.dir/cli/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/zxing.dir/cli/src/main.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/arvind/git/stripes-prototyping/zxing-2.2/cpp/cli/src/main.cpp -o CMakeFiles/zxing.dir/cli/src/main.cpp.s

CMakeFiles/zxing.dir/cli/src/main.cpp.o.requires:
.PHONY : CMakeFiles/zxing.dir/cli/src/main.cpp.o.requires

CMakeFiles/zxing.dir/cli/src/main.cpp.o.provides: CMakeFiles/zxing.dir/cli/src/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/zxing.dir/build.make CMakeFiles/zxing.dir/cli/src/main.cpp.o.provides.build
.PHONY : CMakeFiles/zxing.dir/cli/src/main.cpp.o.provides

CMakeFiles/zxing.dir/cli/src/main.cpp.o.provides.build: CMakeFiles/zxing.dir/cli/src/main.cpp.o

# Object files for target zxing
zxing_OBJECTS = \
"CMakeFiles/zxing.dir/cli/src/ImageReaderSource.cpp.o" \
"CMakeFiles/zxing.dir/cli/src/jpgd.cpp.o" \
"CMakeFiles/zxing.dir/cli/src/lodepng.cpp.o" \
"CMakeFiles/zxing.dir/cli/src/main.cpp.o"

# External object files for target zxing
zxing_EXTERNAL_OBJECTS =

zxing: CMakeFiles/zxing.dir/cli/src/ImageReaderSource.cpp.o
zxing: CMakeFiles/zxing.dir/cli/src/jpgd.cpp.o
zxing: CMakeFiles/zxing.dir/cli/src/lodepng.cpp.o
zxing: CMakeFiles/zxing.dir/cli/src/main.cpp.o
zxing: CMakeFiles/zxing.dir/build.make
zxing: libzxing.a
zxing: /usr/lib/libiconv.dylib
zxing: CMakeFiles/zxing.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable zxing"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/zxing.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/zxing.dir/build: zxing
.PHONY : CMakeFiles/zxing.dir/build

CMakeFiles/zxing.dir/requires: CMakeFiles/zxing.dir/cli/src/ImageReaderSource.cpp.o.requires
CMakeFiles/zxing.dir/requires: CMakeFiles/zxing.dir/cli/src/jpgd.cpp.o.requires
CMakeFiles/zxing.dir/requires: CMakeFiles/zxing.dir/cli/src/lodepng.cpp.o.requires
CMakeFiles/zxing.dir/requires: CMakeFiles/zxing.dir/cli/src/main.cpp.o.requires
.PHONY : CMakeFiles/zxing.dir/requires

CMakeFiles/zxing.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/zxing.dir/cmake_clean.cmake
.PHONY : CMakeFiles/zxing.dir/clean

CMakeFiles/zxing.dir/depend:
	cd /Users/arvind/git/stripes-prototyping/zxing-2.2/cpp/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/arvind/git/stripes-prototyping/zxing-2.2/cpp /Users/arvind/git/stripes-prototyping/zxing-2.2/cpp /Users/arvind/git/stripes-prototyping/zxing-2.2/cpp/build /Users/arvind/git/stripes-prototyping/zxing-2.2/cpp/build /Users/arvind/git/stripes-prototyping/zxing-2.2/cpp/build/CMakeFiles/zxing.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/zxing.dir/depend

