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
include dal/CMakeFiles/dal.dir/depend.make

# Include the progress variables for this target.
include dal/CMakeFiles/dal.dir/progress.make

# Include the compile flags for this target's objects.
include dal/CMakeFiles/dal.dir/flags.make

dal/CMakeFiles/dal.dir/cv22_4cam.c.o: dal/CMakeFiles/dal.dir/flags.make
dal/CMakeFiles/dal.dir/cv22_4cam.c.o: ../dal/cv22_4cam.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lijianhua/map_hdmcv/code/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object dal/CMakeFiles/dal.dir/cv22_4cam.c.o"
	cd /home/lijianhua/map_hdmcv/code/build/dal && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/dal.dir/cv22_4cam.c.o   -c /home/lijianhua/map_hdmcv/code/dal/cv22_4cam.c

dal/CMakeFiles/dal.dir/cv22_4cam.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/dal.dir/cv22_4cam.c.i"
	cd /home/lijianhua/map_hdmcv/code/build/dal && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/lijianhua/map_hdmcv/code/dal/cv22_4cam.c > CMakeFiles/dal.dir/cv22_4cam.c.i

dal/CMakeFiles/dal.dir/cv22_4cam.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/dal.dir/cv22_4cam.c.s"
	cd /home/lijianhua/map_hdmcv/code/build/dal && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/lijianhua/map_hdmcv/code/dal/cv22_4cam.c -o CMakeFiles/dal.dir/cv22_4cam.c.s

dal/CMakeFiles/dal.dir/cv22_data.c.o: dal/CMakeFiles/dal.dir/flags.make
dal/CMakeFiles/dal.dir/cv22_data.c.o: ../dal/cv22_data.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lijianhua/map_hdmcv/code/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object dal/CMakeFiles/dal.dir/cv22_data.c.o"
	cd /home/lijianhua/map_hdmcv/code/build/dal && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/dal.dir/cv22_data.c.o   -c /home/lijianhua/map_hdmcv/code/dal/cv22_data.c

dal/CMakeFiles/dal.dir/cv22_data.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/dal.dir/cv22_data.c.i"
	cd /home/lijianhua/map_hdmcv/code/build/dal && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/lijianhua/map_hdmcv/code/dal/cv22_data.c > CMakeFiles/dal.dir/cv22_data.c.i

dal/CMakeFiles/dal.dir/cv22_data.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/dal.dir/cv22_data.c.s"
	cd /home/lijianhua/map_hdmcv/code/build/dal && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/lijianhua/map_hdmcv/code/dal/cv22_data.c -o CMakeFiles/dal.dir/cv22_data.c.s

dal/CMakeFiles/dal.dir/dal_image.c.o: dal/CMakeFiles/dal.dir/flags.make
dal/CMakeFiles/dal.dir/dal_image.c.o: ../dal/dal_image.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lijianhua/map_hdmcv/code/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object dal/CMakeFiles/dal.dir/dal_image.c.o"
	cd /home/lijianhua/map_hdmcv/code/build/dal && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/dal.dir/dal_image.c.o   -c /home/lijianhua/map_hdmcv/code/dal/dal_image.c

dal/CMakeFiles/dal.dir/dal_image.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/dal.dir/dal_image.c.i"
	cd /home/lijianhua/map_hdmcv/code/build/dal && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/lijianhua/map_hdmcv/code/dal/dal_image.c > CMakeFiles/dal.dir/dal_image.c.i

dal/CMakeFiles/dal.dir/dal_image.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/dal.dir/dal_image.c.s"
	cd /home/lijianhua/map_hdmcv/code/build/dal && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/lijianhua/map_hdmcv/code/dal/dal_image.c -o CMakeFiles/dal.dir/dal_image.c.s

dal/CMakeFiles/dal.dir/dal_insgps.c.o: dal/CMakeFiles/dal.dir/flags.make
dal/CMakeFiles/dal.dir/dal_insgps.c.o: ../dal/dal_insgps.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lijianhua/map_hdmcv/code/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object dal/CMakeFiles/dal.dir/dal_insgps.c.o"
	cd /home/lijianhua/map_hdmcv/code/build/dal && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/dal.dir/dal_insgps.c.o   -c /home/lijianhua/map_hdmcv/code/dal/dal_insgps.c

dal/CMakeFiles/dal.dir/dal_insgps.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/dal.dir/dal_insgps.c.i"
	cd /home/lijianhua/map_hdmcv/code/build/dal && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/lijianhua/map_hdmcv/code/dal/dal_insgps.c > CMakeFiles/dal.dir/dal_insgps.c.i

dal/CMakeFiles/dal.dir/dal_insgps.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/dal.dir/dal_insgps.c.s"
	cd /home/lijianhua/map_hdmcv/code/build/dal && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/lijianhua/map_hdmcv/code/dal/dal_insgps.c -o CMakeFiles/dal.dir/dal_insgps.c.s

dal/CMakeFiles/dal.dir/dal_lidar.c.o: dal/CMakeFiles/dal.dir/flags.make
dal/CMakeFiles/dal.dir/dal_lidar.c.o: ../dal/dal_lidar.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lijianhua/map_hdmcv/code/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building C object dal/CMakeFiles/dal.dir/dal_lidar.c.o"
	cd /home/lijianhua/map_hdmcv/code/build/dal && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/dal.dir/dal_lidar.c.o   -c /home/lijianhua/map_hdmcv/code/dal/dal_lidar.c

dal/CMakeFiles/dal.dir/dal_lidar.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/dal.dir/dal_lidar.c.i"
	cd /home/lijianhua/map_hdmcv/code/build/dal && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/lijianhua/map_hdmcv/code/dal/dal_lidar.c > CMakeFiles/dal.dir/dal_lidar.c.i

dal/CMakeFiles/dal.dir/dal_lidar.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/dal.dir/dal_lidar.c.s"
	cd /home/lijianhua/map_hdmcv/code/build/dal && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/lijianhua/map_hdmcv/code/dal/dal_lidar.c -o CMakeFiles/dal.dir/dal_lidar.c.s

dal/CMakeFiles/dal.dir/dal_object.c.o: dal/CMakeFiles/dal.dir/flags.make
dal/CMakeFiles/dal.dir/dal_object.c.o: ../dal/dal_object.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lijianhua/map_hdmcv/code/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building C object dal/CMakeFiles/dal.dir/dal_object.c.o"
	cd /home/lijianhua/map_hdmcv/code/build/dal && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/dal.dir/dal_object.c.o   -c /home/lijianhua/map_hdmcv/code/dal/dal_object.c

dal/CMakeFiles/dal.dir/dal_object.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/dal.dir/dal_object.c.i"
	cd /home/lijianhua/map_hdmcv/code/build/dal && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/lijianhua/map_hdmcv/code/dal/dal_object.c > CMakeFiles/dal.dir/dal_object.c.i

dal/CMakeFiles/dal.dir/dal_object.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/dal.dir/dal_object.c.s"
	cd /home/lijianhua/map_hdmcv/code/build/dal && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/lijianhua/map_hdmcv/code/dal/dal_object.c -o CMakeFiles/dal.dir/dal_object.c.s

dal/CMakeFiles/dal.dir/dal_sensors.c.o: dal/CMakeFiles/dal.dir/flags.make
dal/CMakeFiles/dal.dir/dal_sensors.c.o: ../dal/dal_sensors.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lijianhua/map_hdmcv/code/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building C object dal/CMakeFiles/dal.dir/dal_sensors.c.o"
	cd /home/lijianhua/map_hdmcv/code/build/dal && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/dal.dir/dal_sensors.c.o   -c /home/lijianhua/map_hdmcv/code/dal/dal_sensors.c

dal/CMakeFiles/dal.dir/dal_sensors.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/dal.dir/dal_sensors.c.i"
	cd /home/lijianhua/map_hdmcv/code/build/dal && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/lijianhua/map_hdmcv/code/dal/dal_sensors.c > CMakeFiles/dal.dir/dal_sensors.c.i

dal/CMakeFiles/dal.dir/dal_sensors.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/dal.dir/dal_sensors.c.s"
	cd /home/lijianhua/map_hdmcv/code/build/dal && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/lijianhua/map_hdmcv/code/dal/dal_sensors.c -o CMakeFiles/dal.dir/dal_sensors.c.s

dal/CMakeFiles/dal.dir/vlp32c.c.o: dal/CMakeFiles/dal.dir/flags.make
dal/CMakeFiles/dal.dir/vlp32c.c.o: ../dal/vlp32c.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lijianhua/map_hdmcv/code/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building C object dal/CMakeFiles/dal.dir/vlp32c.c.o"
	cd /home/lijianhua/map_hdmcv/code/build/dal && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/dal.dir/vlp32c.c.o   -c /home/lijianhua/map_hdmcv/code/dal/vlp32c.c

dal/CMakeFiles/dal.dir/vlp32c.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/dal.dir/vlp32c.c.i"
	cd /home/lijianhua/map_hdmcv/code/build/dal && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/lijianhua/map_hdmcv/code/dal/vlp32c.c > CMakeFiles/dal.dir/vlp32c.c.i

dal/CMakeFiles/dal.dir/vlp32c.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/dal.dir/vlp32c.c.s"
	cd /home/lijianhua/map_hdmcv/code/build/dal && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/lijianhua/map_hdmcv/code/dal/vlp32c.c -o CMakeFiles/dal.dir/vlp32c.c.s

dal/CMakeFiles/dal.dir/xwgi7660.c.o: dal/CMakeFiles/dal.dir/flags.make
dal/CMakeFiles/dal.dir/xwgi7660.c.o: ../dal/xwgi7660.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lijianhua/map_hdmcv/code/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building C object dal/CMakeFiles/dal.dir/xwgi7660.c.o"
	cd /home/lijianhua/map_hdmcv/code/build/dal && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/dal.dir/xwgi7660.c.o   -c /home/lijianhua/map_hdmcv/code/dal/xwgi7660.c

dal/CMakeFiles/dal.dir/xwgi7660.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/dal.dir/xwgi7660.c.i"
	cd /home/lijianhua/map_hdmcv/code/build/dal && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/lijianhua/map_hdmcv/code/dal/xwgi7660.c > CMakeFiles/dal.dir/xwgi7660.c.i

dal/CMakeFiles/dal.dir/xwgi7660.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/dal.dir/xwgi7660.c.s"
	cd /home/lijianhua/map_hdmcv/code/build/dal && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/lijianhua/map_hdmcv/code/dal/xwgi7660.c -o CMakeFiles/dal.dir/xwgi7660.c.s

# Object files for target dal
dal_OBJECTS = \
"CMakeFiles/dal.dir/cv22_4cam.c.o" \
"CMakeFiles/dal.dir/cv22_data.c.o" \
"CMakeFiles/dal.dir/dal_image.c.o" \
"CMakeFiles/dal.dir/dal_insgps.c.o" \
"CMakeFiles/dal.dir/dal_lidar.c.o" \
"CMakeFiles/dal.dir/dal_object.c.o" \
"CMakeFiles/dal.dir/dal_sensors.c.o" \
"CMakeFiles/dal.dir/vlp32c.c.o" \
"CMakeFiles/dal.dir/xwgi7660.c.o"

# External object files for target dal
dal_EXTERNAL_OBJECTS =

dal/libdal.so: dal/CMakeFiles/dal.dir/cv22_4cam.c.o
dal/libdal.so: dal/CMakeFiles/dal.dir/cv22_data.c.o
dal/libdal.so: dal/CMakeFiles/dal.dir/dal_image.c.o
dal/libdal.so: dal/CMakeFiles/dal.dir/dal_insgps.c.o
dal/libdal.so: dal/CMakeFiles/dal.dir/dal_lidar.c.o
dal/libdal.so: dal/CMakeFiles/dal.dir/dal_object.c.o
dal/libdal.so: dal/CMakeFiles/dal.dir/dal_sensors.c.o
dal/libdal.so: dal/CMakeFiles/dal.dir/vlp32c.c.o
dal/libdal.so: dal/CMakeFiles/dal.dir/xwgi7660.c.o
dal/libdal.so: dal/CMakeFiles/dal.dir/build.make
dal/libdal.so: infra/libinfra.so
dal/libdal.so: dal/CMakeFiles/dal.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/lijianhua/map_hdmcv/code/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Linking C shared library libdal.so"
	cd /home/lijianhua/map_hdmcv/code/build/dal && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/dal.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
dal/CMakeFiles/dal.dir/build: dal/libdal.so

.PHONY : dal/CMakeFiles/dal.dir/build

dal/CMakeFiles/dal.dir/clean:
	cd /home/lijianhua/map_hdmcv/code/build/dal && $(CMAKE_COMMAND) -P CMakeFiles/dal.dir/cmake_clean.cmake
.PHONY : dal/CMakeFiles/dal.dir/clean

dal/CMakeFiles/dal.dir/depend:
	cd /home/lijianhua/map_hdmcv/code/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/lijianhua/map_hdmcv/code /home/lijianhua/map_hdmcv/code/dal /home/lijianhua/map_hdmcv/code/build /home/lijianhua/map_hdmcv/code/build/dal /home/lijianhua/map_hdmcv/code/build/dal/CMakeFiles/dal.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : dal/CMakeFiles/dal.dir/depend
