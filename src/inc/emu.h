#ifndef EMU_H
#define EMU_H

#include "common.h"

typedef struct {
    bool paused;
    bool running;
    uint64_t ticks;
} emuContext;

int emuRun(int argc, char *argv[]);

emuContext *emuGetContext();

void emuCycles(int cpuCycles);

#endif //EMU_H
