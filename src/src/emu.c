#include "../inc/emu.h"
#include "../inc/cart.h"
#include "../inc/cpu.h"
#include "../inc/timer.h"
#include "../inc/ui.h"
#include "../inc/dma.h"

//TODO ADD WIN32 ALTERNATIVE TO PTHREAD
#include <stdio.h>
#include <pthread.h>

#include "../inc/ppu.h"


//
// Emu components:
//
// |Cart|
// |CPU|
// |Address Bus|
// |PPU|
// |Timer|
//
//

static emuContext ctx;

emuContext *emuGetContext() {
    return &ctx;
}

void *cpuRun(void *p) {
    timerInit();
    cpuInit();
    ppuInit();

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
            return 0;
        }
    }

    return 0;
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

    uiInit();

    pthread_t t1;

    if (pthread_create(&t1, NULL, cpuRun, NULL)) {
        fprintf(stderr, "Failed to create CPU thread\n");
        return -1;
    }

    uint32_t previousFrame = 0;

    while (!ctx.die) {
        usleep(1000);
        uiHandleEvents();

        if (previousFrame != ppuGetContext()->currentFrame) {
            uiUpdate();
        }

        previousFrame = ppuGetContext()->currentFrame;
    }

    return 0;
}

void emuCycles(int cpuCycles) {

    for (int i = 0; i < cpuCycles; i++) {
        for (int n=0; n<4; n++) {
            ctx.ticks++;
            timerTick();
            ppuTick();
        }

        dmaTick();
    }
}