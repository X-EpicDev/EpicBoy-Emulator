# ----- SETUP ----- #
cmake_minimum_required(VERSION 3.20)
project(ITD102_GB_Emulator C)

set(CMAKE_C_STANDARD 11)

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/deps")

# ----- DEFINE EXECUTABLE ----- #
add_executable(ITD102_GB_Emulator "${SRC_DIR}/main.c"
        src/lib/emu.c
        src/include/emu.h
        src/include/common.h
        src/lib/bus.c
        src/include/bus.h
        src/lib/cart.c
        src/include/cart.h
        src/lib/cpu.c
        src/include/cpu.h
        src/lib/ppu.c
        src/include/ppu.h
        src/lib/timer.c
        src/include/timer.h
)

# ----- ADD SDL3 TO PROJECT ----- #
set(SDL3_DIR "${LIB_DIR}/SDL3")
add_subdirectory(${SDL3_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE "${SDL3_DIR}/include")
target_link_libraries(${PROJECT_NAME} SDL3::SDL3)

# ----- ADD SDL3 TFF TO PROJECT ----- #
set(SDL3_TFF_DIR "${LIB_DIR}/SDL3TFF")
add_subdirectory(${SDL3_TFF_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE "${SDL3_TFF_DIR}/include")
target_link_libraries(${PROJECT_NAME} SDL3_ttf::SDL3_ttf)