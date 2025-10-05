#ifndef INTERRUPT_H
#define INTERRUPT_H

#include "cpu.h"

typedef enum {
    InterruptVBLANK = 1,
    InterruptLCDSTAT = 2,
    InterruptTIMER = 4,
    InterruptSERIAL = 8,
    InterruptJOYPAD = 16,
} interruptType;

void cpuRequestInterrupt(interruptType type);

void cpuHandleInterrupt(CPUContext *ctx);

#endif //INTERRUPT_H
