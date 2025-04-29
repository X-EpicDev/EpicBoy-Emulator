#ifndef CPU_H
#define CPU_H

#include "common.h"
#include "instructs.h"

typedef struct {
    uint8_t A;
    uint8_t B;
    uint8_t C;
    uint8_t D;
    uint8_t E;
    uint8_t F;
    uint8_t H;
    uint8_t L;
    uint16_t PC;
    uint16_t SP;
} Registers;

typedef struct {
    Registers regs;

    //currently fetched data
    uint16_t fetchData;
    uint16_t memoryDestination;
    bool destinationIsMemory;
    uint8_t currentOPCode;
    instruction *CurrentInstruction;

    bool halted;
    bool stepping;
} CPUContext;

void cpuInit();
bool cpuStep();

typedef void (*IN_PROC)(CPUContext *);

IN_PROC inst_get_processor(instructionType type);

#define CPU_FLAG_Z BIT(ctx->regs.f, 7)
#define CPU_FLAG_C BIT(ctx->regs.f, 4)

uint16_t cpu_read_reg(registerType rt);

#endif //CPU_H
