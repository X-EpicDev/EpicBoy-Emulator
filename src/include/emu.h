#ifndef EMU_H
#define EMU_H

#include "common.h"

typedef struct {
    bool paused;
    bool running;
    uint64_t ticks;
} emu_context;

int emu_run(int argc, char *argv[]);

emu_context *emu_get_context();

#endif //EMU_H
