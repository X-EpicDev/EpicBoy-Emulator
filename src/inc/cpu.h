#ifndef CPU_H
#define CPU_H

#include "common.h"
#include "instructs.h"

//static void fetchData();

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

    bool interruptEnabled;
} CPUContext;

void cpuInit();
bool cpuStep();

typedef void (*InstructionProcess)(CPUContext*);
InstructionProcess instructionGetProcessor(instructionType type);

#define CPUFLAGZ BIT(ctx->regs.F, 7)
#define CPUFLAGC BIT(ctx->regs.F, 4)

uint16_t cpuReadReg(registerType rt);
void cpuSetReg(registerType rt, uint16_t value);

#endif //CPU_H
