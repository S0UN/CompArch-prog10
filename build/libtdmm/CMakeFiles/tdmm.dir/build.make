# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.31

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /opt/homebrew/bin/cmake

# The command to remove a file.
RM = /opt/homebrew/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/victortangton/CompArch-prog10

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/victortangton/CompArch-prog10/build

# Include any dependencies generated for this target.
include libtdmm/CMakeFiles/tdmm.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include libtdmm/CMakeFiles/tdmm.dir/compiler_depend.make

# Include the progress variables for this target.
include libtdmm/CMakeFiles/tdmm.dir/progress.make

# Include the compile flags for this target's objects.
include libtdmm/CMakeFiles/tdmm.dir/flags.make

libtdmm/CMakeFiles/tdmm.dir/codegen:
.PHONY : libtdmm/CMakeFiles/tdmm.dir/codegen

libtdmm/CMakeFiles/tdmm.dir/tdmm.c.o: libtdmm/CMakeFiles/tdmm.dir/flags.make
libtdmm/CMakeFiles/tdmm.dir/tdmm.c.o: /Users/victortangton/CompArch-prog10/libtdmm/tdmm.c
libtdmm/CMakeFiles/tdmm.dir/tdmm.c.o: libtdmm/CMakeFiles/tdmm.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/victortangton/CompArch-prog10/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object libtdmm/CMakeFiles/tdmm.dir/tdmm.c.o"
	cd /Users/victortangton/CompArch-prog10/build/libtdmm && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT libtdmm/CMakeFiles/tdmm.dir/tdmm.c.o -MF CMakeFiles/tdmm.dir/tdmm.c.o.d -o CMakeFiles/tdmm.dir/tdmm.c.o -c /Users/victortangton/CompArch-prog10/libtdmm/tdmm.c

libtdmm/CMakeFiles/tdmm.dir/tdmm.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/tdmm.dir/tdmm.c.i"
	cd /Users/victortangton/CompArch-prog10/build/libtdmm && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/victortangton/CompArch-prog10/libtdmm/tdmm.c > CMakeFiles/tdmm.dir/tdmm.c.i

libtdmm/CMakeFiles/tdmm.dir/tdmm.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/tdmm.dir/tdmm.c.s"
	cd /Users/victortangton/CompArch-prog10/build/libtdmm && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/victortangton/CompArch-prog10/libtdmm/tdmm.c -o CMakeFiles/tdmm.dir/tdmm.c.s

# Object files for target tdmm
tdmm_OBJECTS = \
"CMakeFiles/tdmm.dir/tdmm.c.o"

# External object files for target tdmm
tdmm_EXTERNAL_OBJECTS =

libtdmm/libtdmm.a: libtdmm/CMakeFiles/tdmm.dir/tdmm.c.o
libtdmm/libtdmm.a: libtdmm/CMakeFiles/tdmm.dir/build.make
libtdmm/libtdmm.a: libtdmm/CMakeFiles/tdmm.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/Users/victortangton/CompArch-prog10/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C static library libtdmm.a"
	cd /Users/victortangton/CompArch-prog10/build/libtdmm && $(CMAKE_COMMAND) -P CMakeFiles/tdmm.dir/cmake_clean_target.cmake
	cd /Users/victortangton/CompArch-prog10/build/libtdmm && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/tdmm.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
libtdmm/CMakeFiles/tdmm.dir/build: libtdmm/libtdmm.a
.PHONY : libtdmm/CMakeFiles/tdmm.dir/build

libtdmm/CMakeFiles/tdmm.dir/clean:
	cd /Users/victortangton/CompArch-prog10/build/libtdmm && $(CMAKE_COMMAND) -P CMakeFiles/tdmm.dir/cmake_clean.cmake
.PHONY : libtdmm/CMakeFiles/tdmm.dir/clean

libtdmm/CMakeFiles/tdmm.dir/depend:
	cd /Users/victortangton/CompArch-prog10/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/victortangton/CompArch-prog10 /Users/victortangton/CompArch-prog10/libtdmm /Users/victortangton/CompArch-prog10/build /Users/victortangton/CompArch-prog10/build/libtdmm /Users/victortangton/CompArch-prog10/build/libtdmm/CMakeFiles/tdmm.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : libtdmm/CMakeFiles/tdmm.dir/depend

