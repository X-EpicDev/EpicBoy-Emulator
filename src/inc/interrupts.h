#ifndef INTERRUPT_H
#define INTERRUPT_H

#include "cpu.h"

typedef enum {
    Interrupt_VBLANK = 1,
    Interrupt_LCDSTAT = 2,
    Interrupt_TIMER = 4,
    Interrupt_SERIAL = 8,
    Interrupt_JOYPAD = 16,
} interruptType;

void cpuRequestInterrupt(interruptType type);

void cpuHandleInterrupts(CPUContext *ctx);

#endif //INTERRUPT_H
