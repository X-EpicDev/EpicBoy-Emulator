# ----- SETUP ----- #
cmake_minimum_required(VERSION 3.30)
project(ITD102_GB_Emulator C)

set(CMAKE_C_STANDARD 11)

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/deps")

# ----- DEFINE EXECUTABLE ----- #
add_executable(ITD102_GB_Emulator ${SRC_DIR}/main.c)

# ----- IMPORT SDL ----- #
set(SDL_DIR "${LIB_DIR}/SDL")
add_subdirectory("${SDL_DIR}")
target_include_directories(${PROJECT_NAME} PRIVATE "${SDL_DIR}/include")
target_link_libraries(${PROJECT_NAME} "sdl" "${SDL_LIBRARIES}")

