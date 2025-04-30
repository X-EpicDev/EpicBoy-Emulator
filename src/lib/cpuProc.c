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

//NOP
static void ProcessNOP(CPUContext *ctx) {
    //Does Nothing
}

//CPU INSTRUCTION PROCESSING
//NONE
static void ProcessNone(CPUContext *ctx) {
    printf("INVALID INSTRUCTION");
    exit(-7);
}

//JP
static void ProcessJP(CPUContext *ctx) {
    if (checkCondition(ctx)) {
        ctx->regs.PC = ctx-> fetchData;
        emuCycles(1);
    }
}

//INSTRUCTIONS LIST
static InstructionProcess processors[] = {
    [NONE] = ProcessNone,
    [NOP] = ProcessNOP,
    [JP] = ProcessJP,
};

InstructionProcess instructionGetProcessor(instructionType type) {
    return processors[type];
}

