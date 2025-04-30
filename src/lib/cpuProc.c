#include "../inc/cpu.h"
#include "../inc/emu.h"

//CHECK FOR CONDITIONS
static bool checkCondition(CPUContext *ctx) {
    bool Z = CPUFLAGZ;
    bool C = CPUFLAGC;

    switch (ctx->CurrentInstruction->condition) {
        case ConditionNone: return true;
        case ConditionC: return C;
        case ConditionNC: return !C;
        case ConditionZ: return Z;
        case ConditionNZ: return !Z;
    }

    return false;
}

//SET CPU FLAGS
void cpuSetFlags(CPUContext *ctx, char z, char n, char h, char c) {
    if (z != -1) {
        BITSET(ctx->regs.F, 7, z);
    }
    if (n != -1) {
        BITSET(ctx->regs.F, 6, n);
    }
    if (h != -1) {
        BITSET(ctx->regs.F, 5, h);
    }
    if (c != -1) {
        BITSET(ctx->regs.F, 4, c);
    }
}

//CPU INSTRUCTION PROCESSING
//NONE
static void ProcessNone(CPUContext *ctx) {
    printf("INVALID INSTRUCTION\n");
    exit(-7);
}

//NOP
static void ProcessNOP(CPUContext *ctx) {
    //Does Nothing
}

//JP
static void ProcessJP(CPUContext *ctx) {
    if (checkCondition(ctx)) {
        ctx->regs.PC = ctx-> fetchData;
        emuCycles(1);
    }
}

static void ProcessDI(CPUContext *ctx) {
    ctx->interruptEnabled = false;
}

static void ProcessXOR(CPUContext *ctx) {
    ctx->regs.A ^= ctx->fetchData & 0xFF;
    cpuSetFlags(ctx, ctx->regs.A,0,0,0);
}

//INSTRUCTIONS LIST
static InstructionProcess processors[] = {
    [NONE] = ProcessNone,
    [NOP] = ProcessNOP,
    [JP] = ProcessJP,
    [DI] = ProcessDI,
    [XOR] = ProcessXOR,
};

InstructionProcess instructionGetProcessor(instructionType type) {
    return processors[type];
}