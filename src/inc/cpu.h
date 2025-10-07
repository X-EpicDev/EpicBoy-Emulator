#ifndef CPU_H
#define CPU_H

#include "common.h"
#include "instructions.h"

typedef struct {
    uint8_t A;
    uint8_t F;
    uint8_t B;
    uint8_t C;
    uint8_t D;
    uint8_t E;
    uint8_t H;
    uint8_t L;
    uint16_t PC;
    uint16_t SP;
} CPURegisters;

typedef struct {
    CPURegisters regs;

    //currently fetched data
    uint16_t fetchedData;
    uint16_t memoryDestination;
    bool destinationIsMemory;
    uint8_t currentOPCode;
    instruction *currentInstruction;

    bool halted;
    bool stepping;

    bool interruptMasterEnabled;
    bool enablingIME;
    uint8_t interruptEnableRegister;
    uint8_t interruptFlags;

} CPUContext;

CPURegisters *cpuGetRegisters();

void cpuInit();
bool cpuStep();

typedef void (*InstructionProcess)(CPUContext *);

InstructionProcess instructionGetProcessor(instructionType type);

#define CPUFLAGZ BIT(ctx->regs.F, 7)
#define CPUFLAGN BIT(ctx->regs.F, 6)
#define CPUFLAGH BIT(ctx->regs.F, 5)
#define CPUFLAGC BIT(ctx->regs.F, 4)

uint16_t cpuReadReg(registerType rt);
void cpuSetReg(registerType rt, uint16_t value);

uint16_t cpuGetInterruptReg();
void cpuSetInterruptRegister(uint8_t n);

uint8_t cpuReadReg8(registerType rt);
void cpuSetReg8(registerType rt, uint8_t value);

uint8_t cpuGetInterruptFlags();
void cpuSetInterruptFlags(uint8_t value);

void instructionToString(CPUContext *ctx, char *str);

#endif //CPU_H
