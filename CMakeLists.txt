# ----- SETUP ----- #
cmake_minimum_required(VERSION 3.20)
project(ITD102_GB_Emulator C)

set(CMAKE_C_STANDARD 11)


# ----- DEFINE EXECUTABLE ----- #
add_executable(ITD102_GB_Emulator
        #src files
        "src/lib/emu.c" "src/lib/bus.c" "src/lib/cart.c" "src/lib/cpu.c" "src/lib/ppu.c" "src/lib/timer.c" "src/main.c"
        #header files
        "src/inc/emu.h" "src/inc/bus.h" "src/inc/cart.h" "src/inc/cpu.h" "src/inc/ppu.h" "src/inc/timer.h" "src/inc/common.h"
        src/inc/instructs.h
        src/lib/instructions.c
        src/lib/cpuUtil.c
        src/lib/cpuProc.c
)

# ----- ADD SDL3 TO PROJECT ----- #
set(SDL3_DIR "deps/SDL3")
add_subdirectory(${SDL3_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE "${SDL3_DIR}/include")
target_link_libraries(${PROJECT_NAME} SDL3::SDL3)

# ----- ADD SDL3 TFF TO PROJECT ----- #
set(SDL3_TFF_DIR "deps/SDL3TFF")
add_subdirectory(${SDL3_TFF_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE "${SDL3_TFF_DIR}/include")
target_link_libraries(${PROJECT_NAME} SDL3_ttf::SDL3_ttf)