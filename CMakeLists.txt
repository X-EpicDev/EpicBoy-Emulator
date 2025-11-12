# ----- SETUP ----- #
cmake_minimum_required(VERSION 3.20)
project(EpicBoyEmulator C)

set(CMAKE_C_STANDARD 11)

# ----- DEFINE EXECUTABLE ----- #
add_executable(${PROJECT_NAME}
        #main
        "src/main.c"

        #src files
        "src/src/bus.c" "src/src/cart.c" "src/src/cpu.c" "src/src/cpuFetch.c" "src/src/cpuProc.c" "src/src/cpuUtil.c"
        "src/src/dbg.c" "src/src/dma.c" "src/src/emu.c" "src/src/gamepad.c" "src/src/instructions.c"
        "src/src/interrupts.c" "src/src/io.c" "src/src/lcd.c" "src/src/ppu.c" "src/src/ppuPipeline.c" "src/src/ppuSM.c"
        "src/src/ram.c" "src/src/stack.c" "src/src/timer.c" "src/src/ui.c"

        #include files
        "src/inc/bus.h" "src/inc/cart.h" "src/inc/common.h" "src/inc/cpu.h" "src/inc/dbg.h" "src/inc/dma.h"
        "src/inc/emu.h" "src/inc/gamepad.h" "src/inc/instructions.h" "src/inc/interrupts.h" "src/inc/io.h"
        "src/inc/lcd.h" "src/inc/ppu.h" "src/inc/ppuSM.h" "src/inc/ram.h" "src/inc/stack.h" "src/inc/timer.h"
        "src/inc/ui.h"
)

# ----- ADD SDL2 TO PROJECT ----- #
set(SDL2_DIR "deps/SDL2")
add_subdirectory(${SDL2_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE "${SDL2_DIR}/include")
target_link_libraries(${PROJECT_NAME} SDL2::SDL2)

# ----- ADD SDL2 TFF TO PROJECT ----- #
set(SDL2_TFF_DIR "deps/SDL2TFF")
add_subdirectory(${SDL2_TFF_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE "${SDL2_TFF_DIR}/include")
target_link_libraries(${PROJECT_NAME} SDL2_ttf::SDL2_ttf)