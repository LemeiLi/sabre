# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.15

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
CMAKE_SOURCE_DIR = /home/lijianhua/map_hdmcv/code

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/lijianhua/map_hdmcv/code/build

# Include any dependencies generated for this target.
include infra/CMakeFiles/infra.dir/depend.make

# Include the progress variables for this target.
include infra/CMakeFiles/infra.dir/progress.make

# Include the compile flags for this target's objects.
include infra/CMakeFiles/infra.dir/flags.make

infra/CMakeFiles/infra.dir/net/net.c.o: infra/CMakeFiles/infra.dir/flags.make
infra/CMakeFiles/infra.dir/net/net.c.o: ../infra/net/net.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lijianhua/map_hdmcv/code/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object infra/CMakeFiles/infra.dir/net/net.c.o"
	cd /home/lijianhua/map_hdmcv/code/build/infra && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/infra.dir/net/net.c.o   -c /home/lijianhua/map_hdmcv/code/infra/net/net.c

infra/CMakeFiles/infra.dir/net/net.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/infra.dir/net/net.c.i"
	cd /home/lijianhua/map_hdmcv/code/build/infra && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/lijianhua/map_hdmcv/code/infra/net/net.c > CMakeFiles/infra.dir/net/net.c.i

infra/CMakeFiles/infra.dir/net/net.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/infra.dir/net/net.c.s"
	cd /home/lijianhua/map_hdmcv/code/build/infra && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/lijianhua/map_hdmcv/code/infra/net/net.c -o CMakeFiles/infra.dir/net/net.c.s

infra/CMakeFiles/infra.dir/net/net_utils.c.o: infra/CMakeFiles/infra.dir/flags.make
infra/CMakeFiles/infra.dir/net/net_utils.c.o: ../infra/net/net_utils.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lijianhua/map_hdmcv/code/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object infra/CMakeFiles/infra.dir/net/net_utils.c.o"
	cd /home/lijianhua/map_hdmcv/code/build/infra && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/infra.dir/net/net_utils.c.o   -c /home/lijianhua/map_hdmcv/code/infra/net/net_utils.c

infra/CMakeFiles/infra.dir/net/net_utils.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/infra.dir/net/net_utils.c.i"
	cd /home/lijianhua/map_hdmcv/code/build/infra && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/lijianhua/map_hdmcv/code/infra/net/net_utils.c > CMakeFiles/infra.dir/net/net_utils.c.i

infra/CMakeFiles/infra.dir/net/net_utils.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/infra.dir/net/net_utils.c.s"
	cd /home/lijianhua/map_hdmcv/code/build/infra && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/lijianhua/map_hdmcv/code/infra/net/net_utils.c -o CMakeFiles/infra.dir/net/net_utils.c.s

infra/CMakeFiles/infra.dir/net/tcp_service.c.o: infra/CMakeFiles/infra.dir/flags.make
infra/CMakeFiles/infra.dir/net/tcp_service.c.o: ../infra/net/tcp_service.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lijianhua/map_hdmcv/code/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object infra/CMakeFiles/infra.dir/net/tcp_service.c.o"
	cd /home/lijianhua/map_hdmcv/code/build/infra && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/infra.dir/net/tcp_service.c.o   -c /home/lijianhua/map_hdmcv/code/infra/net/tcp_service.c

infra/CMakeFiles/infra.dir/net/tcp_service.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/infra.dir/net/tcp_service.c.i"
	cd /home/lijianhua/map_hdmcv/code/build/infra && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/lijianhua/map_hdmcv/code/infra/net/tcp_service.c > CMakeFiles/infra.dir/net/tcp_service.c.i

infra/CMakeFiles/infra.dir/net/tcp_service.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/infra.dir/net/tcp_service.c.s"
	cd /home/lijianhua/map_hdmcv/code/build/infra && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/lijianhua/map_hdmcv/code/infra/net/tcp_service.c -o CMakeFiles/infra.dir/net/tcp_service.c.s

infra/CMakeFiles/infra.dir/net/udp_raw.c.o: infra/CMakeFiles/infra.dir/flags.make
infra/CMakeFiles/infra.dir/net/udp_raw.c.o: ../infra/net/udp_raw.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lijianhua/map_hdmcv/code/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object infra/CMakeFiles/infra.dir/net/udp_raw.c.o"
	cd /home/lijianhua/map_hdmcv/code/build/infra && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/infra.dir/net/udp_raw.c.o   -c /home/lijianhua/map_hdmcv/code/infra/net/udp_raw.c

infra/CMakeFiles/infra.dir/net/udp_raw.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/infra.dir/net/udp_raw.c.i"
	cd /home/lijianhua/map_hdmcv/code/build/infra && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/lijianhua/map_hdmcv/code/infra/net/udp_raw.c > CMakeFiles/infra.dir/net/udp_raw.c.i

infra/CMakeFiles/infra.dir/net/udp_raw.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/infra.dir/net/udp_raw.c.s"
	cd /home/lijianhua/map_hdmcv/code/build/infra && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/lijianhua/map_hdmcv/code/infra/net/udp_raw.c -o CMakeFiles/infra.dir/net/udp_raw.c.s

infra/CMakeFiles/infra.dir/net/udp_service.c.o: infra/CMakeFiles/infra.dir/flags.make
infra/CMakeFiles/infra.dir/net/udp_service.c.o: ../infra/net/udp_service.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lijianhua/map_hdmcv/code/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building C object infra/CMakeFiles/infra.dir/net/udp_service.c.o"
	cd /home/lijianhua/map_hdmcv/code/build/infra && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/infra.dir/net/udp_service.c.o   -c /home/lijianhua/map_hdmcv/code/infra/net/udp_service.c

infra/CMakeFiles/infra.dir/net/udp_service.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/infra.dir/net/udp_service.c.i"
	cd /home/lijianhua/map_hdmcv/code/build/infra && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/lijianhua/map_hdmcv/code/infra/net/udp_service.c > CMakeFiles/infra.dir/net/udp_service.c.i

infra/CMakeFiles/infra.dir/net/udp_service.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/infra.dir/net/udp_service.c.s"
	cd /home/lijianhua/map_hdmcv/code/build/infra && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/lijianhua/map_hdmcv/code/infra/net/udp_service.c -o CMakeFiles/infra.dir/net/udp_service.c.s

infra/CMakeFiles/infra.dir/stream/persistence.c.o: infra/CMakeFiles/infra.dir/flags.make
infra/CMakeFiles/infra.dir/stream/persistence.c.o: ../infra/stream/persistence.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lijianhua/map_hdmcv/code/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building C object infra/CMakeFiles/infra.dir/stream/persistence.c.o"
	cd /home/lijianhua/map_hdmcv/code/build/infra && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/infra.dir/stream/persistence.c.o   -c /home/lijianhua/map_hdmcv/code/infra/stream/persistence.c

infra/CMakeFiles/infra.dir/stream/persistence.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/infra.dir/stream/persistence.c.i"
	cd /home/lijianhua/map_hdmcv/code/build/infra && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/lijianhua/map_hdmcv/code/infra/stream/persistence.c > CMakeFiles/infra.dir/stream/persistence.c.i

infra/CMakeFiles/infra.dir/stream/persistence.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/infra.dir/stream/persistence.c.s"
	cd /home/lijianhua/map_hdmcv/code/build/infra && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/lijianhua/map_hdmcv/code/infra/stream/persistence.c -o CMakeFiles/infra.dir/stream/persistence.c.s

infra/CMakeFiles/infra.dir/stream/stream.c.o: infra/CMakeFiles/infra.dir/flags.make
infra/CMakeFiles/infra.dir/stream/stream.c.o: ../infra/stream/stream.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lijianhua/map_hdmcv/code/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building C object infra/CMakeFiles/infra.dir/stream/stream.c.o"
	cd /home/lijianhua/map_hdmcv/code/build/infra && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/infra.dir/stream/stream.c.o   -c /home/lijianhua/map_hdmcv/code/infra/stream/stream.c

infra/CMakeFiles/infra.dir/stream/stream.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/infra.dir/stream/stream.c.i"
	cd /home/lijianhua/map_hdmcv/code/build/infra && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/lijianhua/map_hdmcv/code/infra/stream/stream.c > CMakeFiles/infra.dir/stream/stream.c.i

infra/CMakeFiles/infra.dir/stream/stream.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/infra.dir/stream/stream.c.s"
	cd /home/lijianhua/map_hdmcv/code/build/infra && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/lijianhua/map_hdmcv/code/infra/stream/stream.c -o CMakeFiles/infra.dir/stream/stream.c.s

infra/CMakeFiles/infra.dir/link/datalink.c.o: infra/CMakeFiles/infra.dir/flags.make
infra/CMakeFiles/infra.dir/link/datalink.c.o: ../infra/link/datalink.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lijianhua/map_hdmcv/code/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building C object infra/CMakeFiles/infra.dir/link/datalink.c.o"
	cd /home/lijianhua/map_hdmcv/code/build/infra && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/infra.dir/link/datalink.c.o   -c /home/lijianhua/map_hdmcv/code/infra/link/datalink.c

infra/CMakeFiles/infra.dir/link/datalink.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/infra.dir/link/datalink.c.i"
	cd /home/lijianhua/map_hdmcv/code/build/infra && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/lijianhua/map_hdmcv/code/infra/link/datalink.c > CMakeFiles/infra.dir/link/datalink.c.i

infra/CMakeFiles/infra.dir/link/datalink.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/infra.dir/link/datalink.c.s"
	cd /home/lijianhua/map_hdmcv/code/build/infra && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/lijianhua/map_hdmcv/code/infra/link/datalink.c -o CMakeFiles/infra.dir/link/datalink.c.s

infra/CMakeFiles/infra.dir/link/fifo.c.o: infra/CMakeFiles/infra.dir/flags.make
infra/CMakeFiles/infra.dir/link/fifo.c.o: ../infra/link/fifo.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lijianhua/map_hdmcv/code/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building C object infra/CMakeFiles/infra.dir/link/fifo.c.o"
	cd /home/lijianhua/map_hdmcv/code/build/infra && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/infra.dir/link/fifo.c.o   -c /home/lijianhua/map_hdmcv/code/infra/link/fifo.c

infra/CMakeFiles/infra.dir/link/fifo.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/infra.dir/link/fifo.c.i"
	cd /home/lijianhua/map_hdmcv/code/build/infra && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/lijianhua/map_hdmcv/code/infra/link/fifo.c > CMakeFiles/infra.dir/link/fifo.c.i

infra/CMakeFiles/infra.dir/link/fifo.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/infra.dir/link/fifo.c.s"
	cd /home/lijianhua/map_hdmcv/code/build/infra && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/lijianhua/map_hdmcv/code/infra/link/fifo.c -o CMakeFiles/infra.dir/link/fifo.c.s

infra/CMakeFiles/infra.dir/pack/datapack.c.o: infra/CMakeFiles/infra.dir/flags.make
infra/CMakeFiles/infra.dir/pack/datapack.c.o: ../infra/pack/datapack.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lijianhua/map_hdmcv/code/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building C object infra/CMakeFiles/infra.dir/pack/datapack.c.o"
	cd /home/lijianhua/map_hdmcv/code/build/infra && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/infra.dir/pack/datapack.c.o   -c /home/lijianhua/map_hdmcv/code/infra/pack/datapack.c

infra/CMakeFiles/infra.dir/pack/datapack.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/infra.dir/pack/datapack.c.i"
	cd /home/lijianhua/map_hdmcv/code/build/infra && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/lijianhua/map_hdmcv/code/infra/pack/datapack.c > CMakeFiles/infra.dir/pack/datapack.c.i

infra/CMakeFiles/infra.dir/pack/datapack.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/infra.dir/pack/datapack.c.s"
	cd /home/lijianhua/map_hdmcv/code/build/infra && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/lijianhua/map_hdmcv/code/infra/pack/datapack.c -o CMakeFiles/infra.dir/pack/datapack.c.s

infra/CMakeFiles/infra.dir/utils/exception.c.o: infra/CMakeFiles/infra.dir/flags.make
infra/CMakeFiles/infra.dir/utils/exception.c.o: ../infra/utils/exception.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lijianhua/map_hdmcv/code/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building C object infra/CMakeFiles/infra.dir/utils/exception.c.o"
	cd /home/lijianhua/map_hdmcv/code/build/infra && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/infra.dir/utils/exception.c.o   -c /home/lijianhua/map_hdmcv/code/infra/utils/exception.c

infra/CMakeFiles/infra.dir/utils/exception.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/infra.dir/utils/exception.c.i"
	cd /home/lijianhua/map_hdmcv/code/build/infra && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/lijianhua/map_hdmcv/code/infra/utils/exception.c > CMakeFiles/infra.dir/utils/exception.c.i

infra/CMakeFiles/infra.dir/utils/exception.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/infra.dir/utils/exception.c.s"
	cd /home/lijianhua/map_hdmcv/code/build/infra && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/lijianhua/map_hdmcv/code/infra/utils/exception.c -o CMakeFiles/infra.dir/utils/exception.c.s

infra/CMakeFiles/infra.dir/utils/ringbuffer.c.o: infra/CMakeFiles/infra.dir/flags.make
infra/CMakeFiles/infra.dir/utils/ringbuffer.c.o: ../infra/utils/ringbuffer.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lijianhua/map_hdmcv/code/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Building C object infra/CMakeFiles/infra.dir/utils/ringbuffer.c.o"
	cd /home/lijianhua/map_hdmcv/code/build/infra && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/infra.dir/utils/ringbuffer.c.o   -c /home/lijianhua/map_hdmcv/code/infra/utils/ringbuffer.c

infra/CMakeFiles/infra.dir/utils/ringbuffer.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/infra.dir/utils/ringbuffer.c.i"
	cd /home/lijianhua/map_hdmcv/code/build/infra && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/lijianhua/map_hdmcv/code/infra/utils/ringbuffer.c > CMakeFiles/infra.dir/utils/ringbuffer.c.i

infra/CMakeFiles/infra.dir/utils/ringbuffer.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/infra.dir/utils/ringbuffer.c.s"
	cd /home/lijianhua/map_hdmcv/code/build/infra && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/lijianhua/map_hdmcv/code/infra/utils/ringbuffer.c -o CMakeFiles/infra.dir/utils/ringbuffer.c.s

infra/CMakeFiles/infra.dir/utils/utils.c.o: infra/CMakeFiles/infra.dir/flags.make
infra/CMakeFiles/infra.dir/utils/utils.c.o: ../infra/utils/utils.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lijianhua/map_hdmcv/code/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_13) "Building C object infra/CMakeFiles/infra.dir/utils/utils.c.o"
	cd /home/lijianhua/map_hdmcv/code/build/infra && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/infra.dir/utils/utils.c.o   -c /home/lijianhua/map_hdmcv/code/infra/utils/utils.c

infra/CMakeFiles/infra.dir/utils/utils.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/infra.dir/utils/utils.c.i"
	cd /home/lijianhua/map_hdmcv/code/build/infra && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/lijianhua/map_hdmcv/code/infra/utils/utils.c > CMakeFiles/infra.dir/utils/utils.c.i

infra/CMakeFiles/infra.dir/utils/utils.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/infra.dir/utils/utils.c.s"
	cd /home/lijianhua/map_hdmcv/code/build/infra && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/lijianhua/map_hdmcv/code/infra/utils/utils.c -o CMakeFiles/infra.dir/utils/utils.c.s

# Object files for target infra
infra_OBJECTS = \
"CMakeFiles/infra.dir/net/net.c.o" \
"CMakeFiles/infra.dir/net/net_utils.c.o" \
"CMakeFiles/infra.dir/net/tcp_service.c.o" \
"CMakeFiles/infra.dir/net/udp_raw.c.o" \
"CMakeFiles/infra.dir/net/udp_service.c.o" \
"CMakeFiles/infra.dir/stream/persistence.c.o" \
"CMakeFiles/infra.dir/stream/stream.c.o" \
"CMakeFiles/infra.dir/link/datalink.c.o" \
"CMakeFiles/infra.dir/link/fifo.c.o" \
"CMakeFiles/infra.dir/pack/datapack.c.o" \
"CMakeFiles/infra.dir/utils/exception.c.o" \
"CMakeFiles/infra.dir/utils/ringbuffer.c.o" \
"CMakeFiles/infra.dir/utils/utils.c.o"

# External object files for target infra
infra_EXTERNAL_OBJECTS =

infra/libinfra.so: infra/CMakeFiles/infra.dir/net/net.c.o
infra/libinfra.so: infra/CMakeFiles/infra.dir/net/net_utils.c.o
infra/libinfra.so: infra/CMakeFiles/infra.dir/net/tcp_service.c.o
infra/libinfra.so: infra/CMakeFiles/infra.dir/net/udp_raw.c.o
infra/libinfra.so: infra/CMakeFiles/infra.dir/net/udp_service.c.o
infra/libinfra.so: infra/CMakeFiles/infra.dir/stream/persistence.c.o
infra/libinfra.so: infra/CMakeFiles/infra.dir/stream/stream.c.o
infra/libinfra.so: infra/CMakeFiles/infra.dir/link/datalink.c.o
infra/libinfra.so: infra/CMakeFiles/infra.dir/link/fifo.c.o
infra/libinfra.so: infra/CMakeFiles/infra.dir/pack/datapack.c.o
infra/libinfra.so: infra/CMakeFiles/infra.dir/utils/exception.c.o
infra/libinfra.so: infra/CMakeFiles/infra.dir/utils/ringbuffer.c.o
infra/libinfra.so: infra/CMakeFiles/infra.dir/utils/utils.c.o
infra/libinfra.so: infra/CMakeFiles/infra.dir/build.make
infra/libinfra.so: infra/CMakeFiles/infra.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/lijianhua/map_hdmcv/code/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_14) "Linking C shared library libinfra.so"
	cd /home/lijianhua/map_hdmcv/code/build/infra && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/infra.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
infra/CMakeFiles/infra.dir/build: infra/libinfra.so

.PHONY : infra/CMakeFiles/infra.dir/build

infra/CMakeFiles/infra.dir/clean:
	cd /home/lijianhua/map_hdmcv/code/build/infra && $(CMAKE_COMMAND) -P CMakeFiles/infra.dir/cmake_clean.cmake
.PHONY : infra/CMakeFiles/infra.dir/clean

infra/CMakeFiles/infra.dir/depend:
	cd /home/lijianhua/map_hdmcv/code/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/lijianhua/map_hdmcv/code /home/lijianhua/map_hdmcv/code/infra /home/lijianhua/map_hdmcv/code/build /home/lijianhua/map_hdmcv/code/build/infra /home/lijianhua/map_hdmcv/code/build/infra/CMakeFiles/infra.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : infra/CMakeFiles/infra.dir/depend

