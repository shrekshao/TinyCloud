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
CMAKE_COMMAND = /home/tianli/local/CLion/clion-2016.3/bin/cmake/bin/cmake

# The command to remove a file.
RM = /home/tianli/local/CLion/clion-2016.3/bin/cmake/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/tianli/upenn/cis505/TinyCloud

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/tianli/upenn/cis505/TinyCloud/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/TinyCloud.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/TinyCloud.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/TinyCloud.dir/flags.make

CMakeFiles/TinyCloud.dir/http_server.cpp.o: CMakeFiles/TinyCloud.dir/flags.make
CMakeFiles/TinyCloud.dir/http_server.cpp.o: ../http_server.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tianli/upenn/cis505/TinyCloud/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/TinyCloud.dir/http_server.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/TinyCloud.dir/http_server.cpp.o -c /home/tianli/upenn/cis505/TinyCloud/http_server.cpp

CMakeFiles/TinyCloud.dir/http_server.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TinyCloud.dir/http_server.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tianli/upenn/cis505/TinyCloud/http_server.cpp > CMakeFiles/TinyCloud.dir/http_server.cpp.i

CMakeFiles/TinyCloud.dir/http_server.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TinyCloud.dir/http_server.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tianli/upenn/cis505/TinyCloud/http_server.cpp -o CMakeFiles/TinyCloud.dir/http_server.cpp.s

CMakeFiles/TinyCloud.dir/http_server.cpp.o.requires:

.PHONY : CMakeFiles/TinyCloud.dir/http_server.cpp.o.requires

CMakeFiles/TinyCloud.dir/http_server.cpp.o.provides: CMakeFiles/TinyCloud.dir/http_server.cpp.o.requires
	$(MAKE) -f CMakeFiles/TinyCloud.dir/build.make CMakeFiles/TinyCloud.dir/http_server.cpp.o.provides.build
.PHONY : CMakeFiles/TinyCloud.dir/http_server.cpp.o.provides

CMakeFiles/TinyCloud.dir/http_server.cpp.o.provides.build: CMakeFiles/TinyCloud.dir/http_server.cpp.o


CMakeFiles/TinyCloud.dir/grpc_backend/backend_server.cpp.o: CMakeFiles/TinyCloud.dir/flags.make
CMakeFiles/TinyCloud.dir/grpc_backend/backend_server.cpp.o: ../grpc_backend/backend_server.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tianli/upenn/cis505/TinyCloud/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/TinyCloud.dir/grpc_backend/backend_server.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/TinyCloud.dir/grpc_backend/backend_server.cpp.o -c /home/tianli/upenn/cis505/TinyCloud/grpc_backend/backend_server.cpp

CMakeFiles/TinyCloud.dir/grpc_backend/backend_server.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TinyCloud.dir/grpc_backend/backend_server.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tianli/upenn/cis505/TinyCloud/grpc_backend/backend_server.cpp > CMakeFiles/TinyCloud.dir/grpc_backend/backend_server.cpp.i

CMakeFiles/TinyCloud.dir/grpc_backend/backend_server.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TinyCloud.dir/grpc_backend/backend_server.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tianli/upenn/cis505/TinyCloud/grpc_backend/backend_server.cpp -o CMakeFiles/TinyCloud.dir/grpc_backend/backend_server.cpp.s

CMakeFiles/TinyCloud.dir/grpc_backend/backend_server.cpp.o.requires:

.PHONY : CMakeFiles/TinyCloud.dir/grpc_backend/backend_server.cpp.o.requires

CMakeFiles/TinyCloud.dir/grpc_backend/backend_server.cpp.o.provides: CMakeFiles/TinyCloud.dir/grpc_backend/backend_server.cpp.o.requires
	$(MAKE) -f CMakeFiles/TinyCloud.dir/build.make CMakeFiles/TinyCloud.dir/grpc_backend/backend_server.cpp.o.provides.build
.PHONY : CMakeFiles/TinyCloud.dir/grpc_backend/backend_server.cpp.o.provides

CMakeFiles/TinyCloud.dir/grpc_backend/backend_server.cpp.o.provides.build: CMakeFiles/TinyCloud.dir/grpc_backend/backend_server.cpp.o


CMakeFiles/TinyCloud.dir/grpc_backend/filelist_wu.cpp.o: CMakeFiles/TinyCloud.dir/flags.make
CMakeFiles/TinyCloud.dir/grpc_backend/filelist_wu.cpp.o: ../grpc_backend/filelist_wu.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tianli/upenn/cis505/TinyCloud/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/TinyCloud.dir/grpc_backend/filelist_wu.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/TinyCloud.dir/grpc_backend/filelist_wu.cpp.o -c /home/tianli/upenn/cis505/TinyCloud/grpc_backend/filelist_wu.cpp

CMakeFiles/TinyCloud.dir/grpc_backend/filelist_wu.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TinyCloud.dir/grpc_backend/filelist_wu.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tianli/upenn/cis505/TinyCloud/grpc_backend/filelist_wu.cpp > CMakeFiles/TinyCloud.dir/grpc_backend/filelist_wu.cpp.i

CMakeFiles/TinyCloud.dir/grpc_backend/filelist_wu.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TinyCloud.dir/grpc_backend/filelist_wu.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tianli/upenn/cis505/TinyCloud/grpc_backend/filelist_wu.cpp -o CMakeFiles/TinyCloud.dir/grpc_backend/filelist_wu.cpp.s

CMakeFiles/TinyCloud.dir/grpc_backend/filelist_wu.cpp.o.requires:

.PHONY : CMakeFiles/TinyCloud.dir/grpc_backend/filelist_wu.cpp.o.requires

CMakeFiles/TinyCloud.dir/grpc_backend/filelist_wu.cpp.o.provides: CMakeFiles/TinyCloud.dir/grpc_backend/filelist_wu.cpp.o.requires
	$(MAKE) -f CMakeFiles/TinyCloud.dir/build.make CMakeFiles/TinyCloud.dir/grpc_backend/filelist_wu.cpp.o.provides.build
.PHONY : CMakeFiles/TinyCloud.dir/grpc_backend/filelist_wu.cpp.o.provides

CMakeFiles/TinyCloud.dir/grpc_backend/filelist_wu.cpp.o.provides.build: CMakeFiles/TinyCloud.dir/grpc_backend/filelist_wu.cpp.o


CMakeFiles/TinyCloud.dir/grpc_backend/Indexer.cpp.o: CMakeFiles/TinyCloud.dir/flags.make
CMakeFiles/TinyCloud.dir/grpc_backend/Indexer.cpp.o: ../grpc_backend/Indexer.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tianli/upenn/cis505/TinyCloud/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/TinyCloud.dir/grpc_backend/Indexer.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/TinyCloud.dir/grpc_backend/Indexer.cpp.o -c /home/tianli/upenn/cis505/TinyCloud/grpc_backend/Indexer.cpp

CMakeFiles/TinyCloud.dir/grpc_backend/Indexer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TinyCloud.dir/grpc_backend/Indexer.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tianli/upenn/cis505/TinyCloud/grpc_backend/Indexer.cpp > CMakeFiles/TinyCloud.dir/grpc_backend/Indexer.cpp.i

CMakeFiles/TinyCloud.dir/grpc_backend/Indexer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TinyCloud.dir/grpc_backend/Indexer.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tianli/upenn/cis505/TinyCloud/grpc_backend/Indexer.cpp -o CMakeFiles/TinyCloud.dir/grpc_backend/Indexer.cpp.s

CMakeFiles/TinyCloud.dir/grpc_backend/Indexer.cpp.o.requires:

.PHONY : CMakeFiles/TinyCloud.dir/grpc_backend/Indexer.cpp.o.requires

CMakeFiles/TinyCloud.dir/grpc_backend/Indexer.cpp.o.provides: CMakeFiles/TinyCloud.dir/grpc_backend/Indexer.cpp.o.requires
	$(MAKE) -f CMakeFiles/TinyCloud.dir/build.make CMakeFiles/TinyCloud.dir/grpc_backend/Indexer.cpp.o.provides.build
.PHONY : CMakeFiles/TinyCloud.dir/grpc_backend/Indexer.cpp.o.provides

CMakeFiles/TinyCloud.dir/grpc_backend/Indexer.cpp.o.provides.build: CMakeFiles/TinyCloud.dir/grpc_backend/Indexer.cpp.o


CMakeFiles/TinyCloud.dir/grpc_backend/file_wu.cpp.o: CMakeFiles/TinyCloud.dir/flags.make
CMakeFiles/TinyCloud.dir/grpc_backend/file_wu.cpp.o: ../grpc_backend/file_wu.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tianli/upenn/cis505/TinyCloud/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/TinyCloud.dir/grpc_backend/file_wu.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/TinyCloud.dir/grpc_backend/file_wu.cpp.o -c /home/tianli/upenn/cis505/TinyCloud/grpc_backend/file_wu.cpp

CMakeFiles/TinyCloud.dir/grpc_backend/file_wu.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TinyCloud.dir/grpc_backend/file_wu.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tianli/upenn/cis505/TinyCloud/grpc_backend/file_wu.cpp > CMakeFiles/TinyCloud.dir/grpc_backend/file_wu.cpp.i

CMakeFiles/TinyCloud.dir/grpc_backend/file_wu.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TinyCloud.dir/grpc_backend/file_wu.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tianli/upenn/cis505/TinyCloud/grpc_backend/file_wu.cpp -o CMakeFiles/TinyCloud.dir/grpc_backend/file_wu.cpp.s

CMakeFiles/TinyCloud.dir/grpc_backend/file_wu.cpp.o.requires:

.PHONY : CMakeFiles/TinyCloud.dir/grpc_backend/file_wu.cpp.o.requires

CMakeFiles/TinyCloud.dir/grpc_backend/file_wu.cpp.o.provides: CMakeFiles/TinyCloud.dir/grpc_backend/file_wu.cpp.o.requires
	$(MAKE) -f CMakeFiles/TinyCloud.dir/build.make CMakeFiles/TinyCloud.dir/grpc_backend/file_wu.cpp.o.provides.build
.PHONY : CMakeFiles/TinyCloud.dir/grpc_backend/file_wu.cpp.o.provides

CMakeFiles/TinyCloud.dir/grpc_backend/file_wu.cpp.o.provides.build: CMakeFiles/TinyCloud.dir/grpc_backend/file_wu.cpp.o


CMakeFiles/TinyCloud.dir/grpc_backend/BigTabler.cpp.o: CMakeFiles/TinyCloud.dir/flags.make
CMakeFiles/TinyCloud.dir/grpc_backend/BigTabler.cpp.o: ../grpc_backend/BigTabler.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tianli/upenn/cis505/TinyCloud/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/TinyCloud.dir/grpc_backend/BigTabler.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/TinyCloud.dir/grpc_backend/BigTabler.cpp.o -c /home/tianli/upenn/cis505/TinyCloud/grpc_backend/BigTabler.cpp

CMakeFiles/TinyCloud.dir/grpc_backend/BigTabler.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TinyCloud.dir/grpc_backend/BigTabler.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tianli/upenn/cis505/TinyCloud/grpc_backend/BigTabler.cpp > CMakeFiles/TinyCloud.dir/grpc_backend/BigTabler.cpp.i

CMakeFiles/TinyCloud.dir/grpc_backend/BigTabler.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TinyCloud.dir/grpc_backend/BigTabler.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tianli/upenn/cis505/TinyCloud/grpc_backend/BigTabler.cpp -o CMakeFiles/TinyCloud.dir/grpc_backend/BigTabler.cpp.s

CMakeFiles/TinyCloud.dir/grpc_backend/BigTabler.cpp.o.requires:

.PHONY : CMakeFiles/TinyCloud.dir/grpc_backend/BigTabler.cpp.o.requires

CMakeFiles/TinyCloud.dir/grpc_backend/BigTabler.cpp.o.provides: CMakeFiles/TinyCloud.dir/grpc_backend/BigTabler.cpp.o.requires
	$(MAKE) -f CMakeFiles/TinyCloud.dir/build.make CMakeFiles/TinyCloud.dir/grpc_backend/BigTabler.cpp.o.provides.build
.PHONY : CMakeFiles/TinyCloud.dir/grpc_backend/BigTabler.cpp.o.provides

CMakeFiles/TinyCloud.dir/grpc_backend/BigTabler.cpp.o.provides.build: CMakeFiles/TinyCloud.dir/grpc_backend/BigTabler.cpp.o


# Object files for target TinyCloud
TinyCloud_OBJECTS = \
"CMakeFiles/TinyCloud.dir/http_server.cpp.o" \
"CMakeFiles/TinyCloud.dir/grpc_backend/backend_server.cpp.o" \
"CMakeFiles/TinyCloud.dir/grpc_backend/filelist_wu.cpp.o" \
"CMakeFiles/TinyCloud.dir/grpc_backend/Indexer.cpp.o" \
"CMakeFiles/TinyCloud.dir/grpc_backend/file_wu.cpp.o" \
"CMakeFiles/TinyCloud.dir/grpc_backend/BigTabler.cpp.o"

# External object files for target TinyCloud
TinyCloud_EXTERNAL_OBJECTS =

TinyCloud: CMakeFiles/TinyCloud.dir/http_server.cpp.o
TinyCloud: CMakeFiles/TinyCloud.dir/grpc_backend/backend_server.cpp.o
TinyCloud: CMakeFiles/TinyCloud.dir/grpc_backend/filelist_wu.cpp.o
TinyCloud: CMakeFiles/TinyCloud.dir/grpc_backend/Indexer.cpp.o
TinyCloud: CMakeFiles/TinyCloud.dir/grpc_backend/file_wu.cpp.o
TinyCloud: CMakeFiles/TinyCloud.dir/grpc_backend/BigTabler.cpp.o
TinyCloud: CMakeFiles/TinyCloud.dir/build.make
TinyCloud: CMakeFiles/TinyCloud.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/tianli/upenn/cis505/TinyCloud/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Linking CXX executable TinyCloud"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/TinyCloud.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/TinyCloud.dir/build: TinyCloud

.PHONY : CMakeFiles/TinyCloud.dir/build

CMakeFiles/TinyCloud.dir/requires: CMakeFiles/TinyCloud.dir/http_server.cpp.o.requires
CMakeFiles/TinyCloud.dir/requires: CMakeFiles/TinyCloud.dir/grpc_backend/backend_server.cpp.o.requires
CMakeFiles/TinyCloud.dir/requires: CMakeFiles/TinyCloud.dir/grpc_backend/filelist_wu.cpp.o.requires
CMakeFiles/TinyCloud.dir/requires: CMakeFiles/TinyCloud.dir/grpc_backend/Indexer.cpp.o.requires
CMakeFiles/TinyCloud.dir/requires: CMakeFiles/TinyCloud.dir/grpc_backend/file_wu.cpp.o.requires
CMakeFiles/TinyCloud.dir/requires: CMakeFiles/TinyCloud.dir/grpc_backend/BigTabler.cpp.o.requires

.PHONY : CMakeFiles/TinyCloud.dir/requires

CMakeFiles/TinyCloud.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/TinyCloud.dir/cmake_clean.cmake
.PHONY : CMakeFiles/TinyCloud.dir/clean

CMakeFiles/TinyCloud.dir/depend:
	cd /home/tianli/upenn/cis505/TinyCloud/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/tianli/upenn/cis505/TinyCloud /home/tianli/upenn/cis505/TinyCloud /home/tianli/upenn/cis505/TinyCloud/cmake-build-debug /home/tianli/upenn/cis505/TinyCloud/cmake-build-debug /home/tianli/upenn/cis505/TinyCloud/cmake-build-debug/CMakeFiles/TinyCloud.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/TinyCloud.dir/depend

