# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.19

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
CMAKE_COMMAND = /snap/clion/151/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /snap/clion/151/bin/cmake/linux/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/sancho/Documents/GitRepos/MessageXchanger/MessageXChanger

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/sancho/Documents/GitRepos/MessageXchanger/MessageXChanger/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/MessageXChanger.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/MessageXChanger.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/MessageXChanger.dir/flags.make

CMakeFiles/MessageXChanger.dir/main.c.o: CMakeFiles/MessageXChanger.dir/flags.make
CMakeFiles/MessageXChanger.dir/main.c.o: ../main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sancho/Documents/GitRepos/MessageXchanger/MessageXChanger/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/MessageXChanger.dir/main.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/MessageXChanger.dir/main.c.o -c /home/sancho/Documents/GitRepos/MessageXchanger/MessageXChanger/main.c

CMakeFiles/MessageXChanger.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/MessageXChanger.dir/main.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/sancho/Documents/GitRepos/MessageXchanger/MessageXChanger/main.c > CMakeFiles/MessageXChanger.dir/main.c.i

CMakeFiles/MessageXChanger.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/MessageXChanger.dir/main.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/sancho/Documents/GitRepos/MessageXchanger/MessageXChanger/main.c -o CMakeFiles/MessageXChanger.dir/main.c.s

# Object files for target MessageXChanger
MessageXChanger_OBJECTS = \
"CMakeFiles/MessageXChanger.dir/main.c.o"

# External object files for target MessageXChanger
MessageXChanger_EXTERNAL_OBJECTS =

MessageXChanger: CMakeFiles/MessageXChanger.dir/main.c.o
MessageXChanger: CMakeFiles/MessageXChanger.dir/build.make
MessageXChanger: CMakeFiles/MessageXChanger.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/sancho/Documents/GitRepos/MessageXchanger/MessageXChanger/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable MessageXChanger"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/MessageXChanger.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/MessageXChanger.dir/build: MessageXChanger

.PHONY : CMakeFiles/MessageXChanger.dir/build

CMakeFiles/MessageXChanger.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/MessageXChanger.dir/cmake_clean.cmake
.PHONY : CMakeFiles/MessageXChanger.dir/clean

CMakeFiles/MessageXChanger.dir/depend:
	cd /home/sancho/Documents/GitRepos/MessageXchanger/MessageXChanger/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/sancho/Documents/GitRepos/MessageXchanger/MessageXChanger /home/sancho/Documents/GitRepos/MessageXchanger/MessageXChanger /home/sancho/Documents/GitRepos/MessageXchanger/MessageXChanger/cmake-build-debug /home/sancho/Documents/GitRepos/MessageXchanger/MessageXChanger/cmake-build-debug /home/sancho/Documents/GitRepos/MessageXchanger/MessageXChanger/cmake-build-debug/CMakeFiles/MessageXChanger.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/MessageXChanger.dir/depend

