#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#include "../inc/emu.h"
#include "../inc/cart.h"
#include "../inc/cpu.h"
#include "../inc/ui.h"



static emuContext ctx;

emuContext *emuGetContext() {
    return &ctx;
}

void *cpuRun(void *p) {
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
            return 0;
        }

        ctx.ticks++;
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

    while (!ctx.die) {
        usleep(1000);
        uiHandleEvents();
    }


    return 0;
}

void emuCycles(int cpuCycles) {
    //Not IMPL
}