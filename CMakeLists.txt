# ----- SETUP ----- #
cmake_minimum_required(VERSION 3.20)
project(EpicBoyEmulator C)

set(CMAKE_C_STANDARD 11)

# ----- DEFINE EXECUTABLE ----- #
add_executable(${PROJECT_NAME}
        #src files
        "src/src/emu.c" "src/src/bus.c" "src/src/cart.c" "src/src/cpu.c" "src/src/ppu.c" "src/src/timer.c" "src/main.c"
        "src/src/instructions.c" "src/src/cpuUtil.c" "src/src/cpuProc.c" "src/src/cpuFetch.c" "src/src/ram.c" "src/src/stack.c"
        "src/src/interrupt.c" "src/src/ui.c"

        "src/inc/emu.h" "src/inc/bus.h" "src/inc/cart.h" "src/inc/cpu.h" "src/inc/ppu.h" "src/inc/timer.h" "src/inc/common.h"
        "src/inc/instructs.h" "src/inc/ram.h" "src/inc/stack.h" "src/inc/interrupt.h" "src/inc/ui.h"
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