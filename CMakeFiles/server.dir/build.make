# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

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
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/emanuele/Scrivania/WebServer

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/emanuele/Scrivania/WebServer

# Include any dependencies generated for this target.
include CMakeFiles/server.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/server.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/server.dir/flags.make

CMakeFiles/server.dir/lib/logging.c.o: CMakeFiles/server.dir/flags.make
CMakeFiles/server.dir/lib/logging.c.o: lib/logging.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/emanuele/Scrivania/WebServer/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/server.dir/lib/logging.c.o"
	/usr/bin/gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/server.dir/lib/logging.c.o   -c /home/emanuele/Scrivania/WebServer/lib/logging.c

CMakeFiles/server.dir/lib/logging.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/server.dir/lib/logging.c.i"
	/usr/bin/gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/emanuele/Scrivania/WebServer/lib/logging.c > CMakeFiles/server.dir/lib/logging.c.i

CMakeFiles/server.dir/lib/logging.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/server.dir/lib/logging.c.s"
	/usr/bin/gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/emanuele/Scrivania/WebServer/lib/logging.c -o CMakeFiles/server.dir/lib/logging.c.s

CMakeFiles/server.dir/lib/logging.c.o.requires:

.PHONY : CMakeFiles/server.dir/lib/logging.c.o.requires

CMakeFiles/server.dir/lib/logging.c.o.provides: CMakeFiles/server.dir/lib/logging.c.o.requires
	$(MAKE) -f CMakeFiles/server.dir/build.make CMakeFiles/server.dir/lib/logging.c.o.provides.build
.PHONY : CMakeFiles/server.dir/lib/logging.c.o.provides

CMakeFiles/server.dir/lib/logging.c.o.provides.build: CMakeFiles/server.dir/lib/logging.c.o


CMakeFiles/server.dir/lib/lru-cache.c.o: CMakeFiles/server.dir/flags.make
CMakeFiles/server.dir/lib/lru-cache.c.o: lib/lru-cache.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/emanuele/Scrivania/WebServer/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/server.dir/lib/lru-cache.c.o"
	/usr/bin/gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/server.dir/lib/lru-cache.c.o   -c /home/emanuele/Scrivania/WebServer/lib/lru-cache.c

CMakeFiles/server.dir/lib/lru-cache.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/server.dir/lib/lru-cache.c.i"
	/usr/bin/gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/emanuele/Scrivania/WebServer/lib/lru-cache.c > CMakeFiles/server.dir/lib/lru-cache.c.i

CMakeFiles/server.dir/lib/lru-cache.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/server.dir/lib/lru-cache.c.s"
	/usr/bin/gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/emanuele/Scrivania/WebServer/lib/lru-cache.c -o CMakeFiles/server.dir/lib/lru-cache.c.s

CMakeFiles/server.dir/lib/lru-cache.c.o.requires:

.PHONY : CMakeFiles/server.dir/lib/lru-cache.c.o.requires

CMakeFiles/server.dir/lib/lru-cache.c.o.provides: CMakeFiles/server.dir/lib/lru-cache.c.o.requires
	$(MAKE) -f CMakeFiles/server.dir/build.make CMakeFiles/server.dir/lib/lru-cache.c.o.provides.build
.PHONY : CMakeFiles/server.dir/lib/lru-cache.c.o.provides

CMakeFiles/server.dir/lib/lru-cache.c.o.provides.build: CMakeFiles/server.dir/lib/lru-cache.c.o


CMakeFiles/server.dir/lib/io.c.o: CMakeFiles/server.dir/flags.make
CMakeFiles/server.dir/lib/io.c.o: lib/io.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/emanuele/Scrivania/WebServer/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/server.dir/lib/io.c.o"
	/usr/bin/gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/server.dir/lib/io.c.o   -c /home/emanuele/Scrivania/WebServer/lib/io.c

CMakeFiles/server.dir/lib/io.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/server.dir/lib/io.c.i"
	/usr/bin/gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/emanuele/Scrivania/WebServer/lib/io.c > CMakeFiles/server.dir/lib/io.c.i

CMakeFiles/server.dir/lib/io.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/server.dir/lib/io.c.s"
	/usr/bin/gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/emanuele/Scrivania/WebServer/lib/io.c -o CMakeFiles/server.dir/lib/io.c.s

CMakeFiles/server.dir/lib/io.c.o.requires:

.PHONY : CMakeFiles/server.dir/lib/io.c.o.requires

CMakeFiles/server.dir/lib/io.c.o.provides: CMakeFiles/server.dir/lib/io.c.o.requires
	$(MAKE) -f CMakeFiles/server.dir/build.make CMakeFiles/server.dir/lib/io.c.o.provides.build
.PHONY : CMakeFiles/server.dir/lib/io.c.o.provides

CMakeFiles/server.dir/lib/io.c.o.provides.build: CMakeFiles/server.dir/lib/io.c.o


CMakeFiles/server.dir/server.c.o: CMakeFiles/server.dir/flags.make
CMakeFiles/server.dir/server.c.o: server.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/emanuele/Scrivania/WebServer/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object CMakeFiles/server.dir/server.c.o"
	/usr/bin/gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/server.dir/server.c.o   -c /home/emanuele/Scrivania/WebServer/server.c

CMakeFiles/server.dir/server.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/server.dir/server.c.i"
	/usr/bin/gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/emanuele/Scrivania/WebServer/server.c > CMakeFiles/server.dir/server.c.i

CMakeFiles/server.dir/server.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/server.dir/server.c.s"
	/usr/bin/gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/emanuele/Scrivania/WebServer/server.c -o CMakeFiles/server.dir/server.c.s

CMakeFiles/server.dir/server.c.o.requires:

.PHONY : CMakeFiles/server.dir/server.c.o.requires

CMakeFiles/server.dir/server.c.o.provides: CMakeFiles/server.dir/server.c.o.requires
	$(MAKE) -f CMakeFiles/server.dir/build.make CMakeFiles/server.dir/server.c.o.provides.build
.PHONY : CMakeFiles/server.dir/server.c.o.provides

CMakeFiles/server.dir/server.c.o.provides.build: CMakeFiles/server.dir/server.c.o


CMakeFiles/server.dir/lib/lock_fcntl.c.o: CMakeFiles/server.dir/flags.make
CMakeFiles/server.dir/lib/lock_fcntl.c.o: lib/lock_fcntl.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/emanuele/Scrivania/WebServer/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building C object CMakeFiles/server.dir/lib/lock_fcntl.c.o"
	/usr/bin/gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/server.dir/lib/lock_fcntl.c.o   -c /home/emanuele/Scrivania/WebServer/lib/lock_fcntl.c

CMakeFiles/server.dir/lib/lock_fcntl.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/server.dir/lib/lock_fcntl.c.i"
	/usr/bin/gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/emanuele/Scrivania/WebServer/lib/lock_fcntl.c > CMakeFiles/server.dir/lib/lock_fcntl.c.i

CMakeFiles/server.dir/lib/lock_fcntl.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/server.dir/lib/lock_fcntl.c.s"
	/usr/bin/gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/emanuele/Scrivania/WebServer/lib/lock_fcntl.c -o CMakeFiles/server.dir/lib/lock_fcntl.c.s

CMakeFiles/server.dir/lib/lock_fcntl.c.o.requires:

.PHONY : CMakeFiles/server.dir/lib/lock_fcntl.c.o.requires

CMakeFiles/server.dir/lib/lock_fcntl.c.o.provides: CMakeFiles/server.dir/lib/lock_fcntl.c.o.requires
	$(MAKE) -f CMakeFiles/server.dir/build.make CMakeFiles/server.dir/lib/lock_fcntl.c.o.provides.build
.PHONY : CMakeFiles/server.dir/lib/lock_fcntl.c.o.provides

CMakeFiles/server.dir/lib/lock_fcntl.c.o.provides.build: CMakeFiles/server.dir/lib/lock_fcntl.c.o


CMakeFiles/server.dir/sig_handler.c.o: CMakeFiles/server.dir/flags.make
CMakeFiles/server.dir/sig_handler.c.o: sig_handler.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/emanuele/Scrivania/WebServer/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building C object CMakeFiles/server.dir/sig_handler.c.o"
	/usr/bin/gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/server.dir/sig_handler.c.o   -c /home/emanuele/Scrivania/WebServer/sig_handler.c

CMakeFiles/server.dir/sig_handler.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/server.dir/sig_handler.c.i"
	/usr/bin/gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/emanuele/Scrivania/WebServer/sig_handler.c > CMakeFiles/server.dir/sig_handler.c.i

CMakeFiles/server.dir/sig_handler.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/server.dir/sig_handler.c.s"
	/usr/bin/gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/emanuele/Scrivania/WebServer/sig_handler.c -o CMakeFiles/server.dir/sig_handler.c.s

CMakeFiles/server.dir/sig_handler.c.o.requires:

.PHONY : CMakeFiles/server.dir/sig_handler.c.o.requires

CMakeFiles/server.dir/sig_handler.c.o.provides: CMakeFiles/server.dir/sig_handler.c.o.requires
	$(MAKE) -f CMakeFiles/server.dir/build.make CMakeFiles/server.dir/sig_handler.c.o.provides.build
.PHONY : CMakeFiles/server.dir/sig_handler.c.o.provides

CMakeFiles/server.dir/sig_handler.c.o.provides.build: CMakeFiles/server.dir/sig_handler.c.o


CMakeFiles/server.dir/thread_job.c.o: CMakeFiles/server.dir/flags.make
CMakeFiles/server.dir/thread_job.c.o: thread_job.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/emanuele/Scrivania/WebServer/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building C object CMakeFiles/server.dir/thread_job.c.o"
	/usr/bin/gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/server.dir/thread_job.c.o   -c /home/emanuele/Scrivania/WebServer/thread_job.c

CMakeFiles/server.dir/thread_job.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/server.dir/thread_job.c.i"
	/usr/bin/gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/emanuele/Scrivania/WebServer/thread_job.c > CMakeFiles/server.dir/thread_job.c.i

CMakeFiles/server.dir/thread_job.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/server.dir/thread_job.c.s"
	/usr/bin/gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/emanuele/Scrivania/WebServer/thread_job.c -o CMakeFiles/server.dir/thread_job.c.s

CMakeFiles/server.dir/thread_job.c.o.requires:

.PHONY : CMakeFiles/server.dir/thread_job.c.o.requires

CMakeFiles/server.dir/thread_job.c.o.provides: CMakeFiles/server.dir/thread_job.c.o.requires
	$(MAKE) -f CMakeFiles/server.dir/build.make CMakeFiles/server.dir/thread_job.c.o.provides.build
.PHONY : CMakeFiles/server.dir/thread_job.c.o.provides

CMakeFiles/server.dir/thread_job.c.o.provides.build: CMakeFiles/server.dir/thread_job.c.o


CMakeFiles/server.dir/parser.c.o: CMakeFiles/server.dir/flags.make
CMakeFiles/server.dir/parser.c.o: parser.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/emanuele/Scrivania/WebServer/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building C object CMakeFiles/server.dir/parser.c.o"
	/usr/bin/gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/server.dir/parser.c.o   -c /home/emanuele/Scrivania/WebServer/parser.c

CMakeFiles/server.dir/parser.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/server.dir/parser.c.i"
	/usr/bin/gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/emanuele/Scrivania/WebServer/parser.c > CMakeFiles/server.dir/parser.c.i

CMakeFiles/server.dir/parser.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/server.dir/parser.c.s"
	/usr/bin/gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/emanuele/Scrivania/WebServer/parser.c -o CMakeFiles/server.dir/parser.c.s

CMakeFiles/server.dir/parser.c.o.requires:

.PHONY : CMakeFiles/server.dir/parser.c.o.requires

CMakeFiles/server.dir/parser.c.o.provides: CMakeFiles/server.dir/parser.c.o.requires
	$(MAKE) -f CMakeFiles/server.dir/build.make CMakeFiles/server.dir/parser.c.o.provides.build
.PHONY : CMakeFiles/server.dir/parser.c.o.provides

CMakeFiles/server.dir/parser.c.o.provides.build: CMakeFiles/server.dir/parser.c.o


CMakeFiles/server.dir/lib/ipc_sem.c.o: CMakeFiles/server.dir/flags.make
CMakeFiles/server.dir/lib/ipc_sem.c.o: lib/ipc_sem.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/emanuele/Scrivania/WebServer/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building C object CMakeFiles/server.dir/lib/ipc_sem.c.o"
	/usr/bin/gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/server.dir/lib/ipc_sem.c.o   -c /home/emanuele/Scrivania/WebServer/lib/ipc_sem.c

CMakeFiles/server.dir/lib/ipc_sem.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/server.dir/lib/ipc_sem.c.i"
	/usr/bin/gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/emanuele/Scrivania/WebServer/lib/ipc_sem.c > CMakeFiles/server.dir/lib/ipc_sem.c.i

CMakeFiles/server.dir/lib/ipc_sem.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/server.dir/lib/ipc_sem.c.s"
	/usr/bin/gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/emanuele/Scrivania/WebServer/lib/ipc_sem.c -o CMakeFiles/server.dir/lib/ipc_sem.c.s

CMakeFiles/server.dir/lib/ipc_sem.c.o.requires:

.PHONY : CMakeFiles/server.dir/lib/ipc_sem.c.o.requires

CMakeFiles/server.dir/lib/ipc_sem.c.o.provides: CMakeFiles/server.dir/lib/ipc_sem.c.o.requires
	$(MAKE) -f CMakeFiles/server.dir/build.make CMakeFiles/server.dir/lib/ipc_sem.c.o.provides.build
.PHONY : CMakeFiles/server.dir/lib/ipc_sem.c.o.provides

CMakeFiles/server.dir/lib/ipc_sem.c.o.provides.build: CMakeFiles/server.dir/lib/ipc_sem.c.o


CMakeFiles/server.dir/http_response.c.o: CMakeFiles/server.dir/flags.make
CMakeFiles/server.dir/http_response.c.o: http_response.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/emanuele/Scrivania/WebServer/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building C object CMakeFiles/server.dir/http_response.c.o"
	/usr/bin/gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/server.dir/http_response.c.o   -c /home/emanuele/Scrivania/WebServer/http_response.c

CMakeFiles/server.dir/http_response.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/server.dir/http_response.c.i"
	/usr/bin/gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/emanuele/Scrivania/WebServer/http_response.c > CMakeFiles/server.dir/http_response.c.i

CMakeFiles/server.dir/http_response.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/server.dir/http_response.c.s"
	/usr/bin/gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/emanuele/Scrivania/WebServer/http_response.c -o CMakeFiles/server.dir/http_response.c.s

CMakeFiles/server.dir/http_response.c.o.requires:

.PHONY : CMakeFiles/server.dir/http_response.c.o.requires

CMakeFiles/server.dir/http_response.c.o.provides: CMakeFiles/server.dir/http_response.c.o.requires
	$(MAKE) -f CMakeFiles/server.dir/build.make CMakeFiles/server.dir/http_response.c.o.provides.build
.PHONY : CMakeFiles/server.dir/http_response.c.o.provides

CMakeFiles/server.dir/http_response.c.o.provides.build: CMakeFiles/server.dir/http_response.c.o


# Object files for target server
server_OBJECTS = \
"CMakeFiles/server.dir/lib/logging.c.o" \
"CMakeFiles/server.dir/lib/lru-cache.c.o" \
"CMakeFiles/server.dir/lib/io.c.o" \
"CMakeFiles/server.dir/server.c.o" \
"CMakeFiles/server.dir/lib/lock_fcntl.c.o" \
"CMakeFiles/server.dir/sig_handler.c.o" \
"CMakeFiles/server.dir/thread_job.c.o" \
"CMakeFiles/server.dir/parser.c.o" \
"CMakeFiles/server.dir/lib/ipc_sem.c.o" \
"CMakeFiles/server.dir/http_response.c.o"

# External object files for target server
server_EXTERNAL_OBJECTS =

server: CMakeFiles/server.dir/lib/logging.c.o
server: CMakeFiles/server.dir/lib/lru-cache.c.o
server: CMakeFiles/server.dir/lib/io.c.o
server: CMakeFiles/server.dir/server.c.o
server: CMakeFiles/server.dir/lib/lock_fcntl.c.o
server: CMakeFiles/server.dir/sig_handler.c.o
server: CMakeFiles/server.dir/thread_job.c.o
server: CMakeFiles/server.dir/parser.c.o
server: CMakeFiles/server.dir/lib/ipc_sem.c.o
server: CMakeFiles/server.dir/http_response.c.o
server: CMakeFiles/server.dir/build.make
server: CMakeFiles/server.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/emanuele/Scrivania/WebServer/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Linking C executable server"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/server.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/server.dir/build: server

.PHONY : CMakeFiles/server.dir/build

CMakeFiles/server.dir/requires: CMakeFiles/server.dir/lib/logging.c.o.requires
CMakeFiles/server.dir/requires: CMakeFiles/server.dir/lib/lru-cache.c.o.requires
CMakeFiles/server.dir/requires: CMakeFiles/server.dir/lib/io.c.o.requires
CMakeFiles/server.dir/requires: CMakeFiles/server.dir/server.c.o.requires
CMakeFiles/server.dir/requires: CMakeFiles/server.dir/lib/lock_fcntl.c.o.requires
CMakeFiles/server.dir/requires: CMakeFiles/server.dir/sig_handler.c.o.requires
CMakeFiles/server.dir/requires: CMakeFiles/server.dir/thread_job.c.o.requires
CMakeFiles/server.dir/requires: CMakeFiles/server.dir/parser.c.o.requires
CMakeFiles/server.dir/requires: CMakeFiles/server.dir/lib/ipc_sem.c.o.requires
CMakeFiles/server.dir/requires: CMakeFiles/server.dir/http_response.c.o.requires

.PHONY : CMakeFiles/server.dir/requires

CMakeFiles/server.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/server.dir/cmake_clean.cmake
.PHONY : CMakeFiles/server.dir/clean

CMakeFiles/server.dir/depend:
	cd /home/emanuele/Scrivania/WebServer && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/emanuele/Scrivania/WebServer /home/emanuele/Scrivania/WebServer /home/emanuele/Scrivania/WebServer /home/emanuele/Scrivania/WebServer /home/emanuele/Scrivania/WebServer/CMakeFiles/server.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/server.dir/depend
