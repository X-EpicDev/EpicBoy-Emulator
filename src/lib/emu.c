#include <stdio.h>
#include "../inc/emu.h"
#include "../inc/cart.h"
#include "../inc/cpu.h"
#include "SDL3/SDL.h"
#include "SDL3_ttf/SDL_ttf.h"

static emuContext ctx;

emuContext *emu_get_context() {
    return &ctx;
}

void delay(uint32_t ms) {
    SDL_Delay(ms);
}

int emuRun(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: emu <romfile>\n");
        return -1;
    }

    if (!cartLoad(argv[1])) {
        printf("Failed to load ROM: %s\n", argv[1]);
        return -2;
    }

    printf("ROM Loaded..\n");

    SDL_Init(SDL_INIT_VIDEO);
    printf("SDL INIT\n");
    TTF_Init();
    printf("TTF INIT\n");

    cpuInit();

    ctx.running = true;
    ctx.paused = false;
    ctx.ticks = 0;

    while (ctx.running) {
        if (ctx.paused) {
            delay(10);
            continue;
        }

        if (!cpuStep()) {
            printf("CPU Stopped\n");
            return -3;
        }

        ctx.ticks++;
    }

    return 0;
}

void emuCycles(int cpuCycles) {
    //Not IMPL
}