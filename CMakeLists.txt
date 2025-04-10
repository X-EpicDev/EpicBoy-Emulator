# ----- SETUP ----- #
cmake_minimum_required(VERSION 3.30)
project(ITD102_GB_Emulator C)

set(CMAKE_C_STANDARD 11)

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(DEP_DIR "${CMAKE_CURRENT_SOURCE_DIR}/deps")

# ----- DEFINE EXECUTABLE ----- #
add_executable(ITD102_GB_Emulator ${SRC_DIR}/main.c)

# ----- IMPORT SDL ----- #

