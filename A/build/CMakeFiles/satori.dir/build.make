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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /usr/bin/ccmake

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/studia/projects/JAiO/A

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/studia/projects/JAiO/A/build

# Include any dependencies generated for this target.
include CMakeFiles/satori.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/satori.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/satori.dir/flags.make

CMakeFiles/satori.dir/RegularLanguage.cpp.o: CMakeFiles/satori.dir/flags.make
CMakeFiles/satori.dir/RegularLanguage.cpp.o: ../RegularLanguage.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/studia/projects/JAiO/A/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/satori.dir/RegularLanguage.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/satori.dir/RegularLanguage.cpp.o -c /home/studia/projects/JAiO/A/RegularLanguage.cpp

CMakeFiles/satori.dir/RegularLanguage.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/satori.dir/RegularLanguage.cpp.i"
	$(CMAKE_COMMAND) -E cmake_unimplemented_variable CMAKE_CXX_CREATE_PREPROCESSED_SOURCE

CMakeFiles/satori.dir/RegularLanguage.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/satori.dir/RegularLanguage.cpp.s"
	$(CMAKE_COMMAND) -E cmake_unimplemented_variable CMAKE_CXX_CREATE_ASSEMBLY_SOURCE

CMakeFiles/satori.dir/RegularLanguage.cpp.o.requires:
.PHONY : CMakeFiles/satori.dir/RegularLanguage.cpp.o.requires

CMakeFiles/satori.dir/RegularLanguage.cpp.o.provides: CMakeFiles/satori.dir/RegularLanguage.cpp.o.requires
	$(MAKE) -f CMakeFiles/satori.dir/build.make CMakeFiles/satori.dir/RegularLanguage.cpp.o.provides.build
.PHONY : CMakeFiles/satori.dir/RegularLanguage.cpp.o.provides

CMakeFiles/satori.dir/RegularLanguage.cpp.o.provides.build: CMakeFiles/satori.dir/RegularLanguage.cpp.o

CMakeFiles/satori.dir/satori.cxx.o: CMakeFiles/satori.dir/flags.make
CMakeFiles/satori.dir/satori.cxx.o: ../satori.cxx
	$(CMAKE_COMMAND) -E cmake_progress_report /home/studia/projects/JAiO/A/build/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/satori.dir/satori.cxx.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/satori.dir/satori.cxx.o -c /home/studia/projects/JAiO/A/satori.cxx

CMakeFiles/satori.dir/satori.cxx.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/satori.dir/satori.cxx.i"
	$(CMAKE_COMMAND) -E cmake_unimplemented_variable CMAKE_CXX_CREATE_PREPROCESSED_SOURCE

CMakeFiles/satori.dir/satori.cxx.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/satori.dir/satori.cxx.s"
	$(CMAKE_COMMAND) -E cmake_unimplemented_variable CMAKE_CXX_CREATE_ASSEMBLY_SOURCE

CMakeFiles/satori.dir/satori.cxx.o.requires:
.PHONY : CMakeFiles/satori.dir/satori.cxx.o.requires

CMakeFiles/satori.dir/satori.cxx.o.provides: CMakeFiles/satori.dir/satori.cxx.o.requires
	$(MAKE) -f CMakeFiles/satori.dir/build.make CMakeFiles/satori.dir/satori.cxx.o.provides.build
.PHONY : CMakeFiles/satori.dir/satori.cxx.o.provides

CMakeFiles/satori.dir/satori.cxx.o.provides.build: CMakeFiles/satori.dir/satori.cxx.o

# Object files for target satori
satori_OBJECTS = \
"CMakeFiles/satori.dir/RegularLanguage.cpp.o" \
"CMakeFiles/satori.dir/satori.cxx.o"

# External object files for target satori
satori_EXTERNAL_OBJECTS =

satori: CMakeFiles/satori.dir/RegularLanguage.cpp.o
satori: CMakeFiles/satori.dir/satori.cxx.o
satori: CMakeFiles/satori.dir/build.make
satori: CMakeFiles/satori.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable satori"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/satori.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/satori.dir/build: satori
.PHONY : CMakeFiles/satori.dir/build

CMakeFiles/satori.dir/requires: CMakeFiles/satori.dir/RegularLanguage.cpp.o.requires
CMakeFiles/satori.dir/requires: CMakeFiles/satori.dir/satori.cxx.o.requires
.PHONY : CMakeFiles/satori.dir/requires

CMakeFiles/satori.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/satori.dir/cmake_clean.cmake
.PHONY : CMakeFiles/satori.dir/clean

CMakeFiles/satori.dir/depend:
	cd /home/studia/projects/JAiO/A/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/studia/projects/JAiO/A /home/studia/projects/JAiO/A /home/studia/projects/JAiO/A/build /home/studia/projects/JAiO/A/build /home/studia/projects/JAiO/A/build/CMakeFiles/satori.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/satori.dir/depend

