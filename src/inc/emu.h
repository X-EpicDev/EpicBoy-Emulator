#ifndef EMU_H
#define EMU_H

#include "common.h"

typedef struct {
    bool paused;
    bool running;
    uint64_t ticks;
} emuContext;

int emuRun(int argc, char *argv[]);

emuContext *emu_get_context();

void emuCycles(int cpuCycles);

#endif //EMU_H
