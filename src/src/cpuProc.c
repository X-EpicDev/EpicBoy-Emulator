#include "../inc/bus.h"
#include "../inc/cpu.h"
#include "../inc/emu.h"
#include "../inc/stack.h"

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

//CPU INSTRUCTION PROCESSING
//NONE
static void ProcessNone(CPUContext *ctx) {
    printf("INVALID INSTRUCTION [0x%02X]\n",ctx->currentOPCode);
    exit(-7);
}

//NOP
static void ProcessNOP(CPUContext *ctx) {
    //Does Nothing
}

static void ProcessLD(CPUContext *ctx) {
    if (ctx->destinationIsMemory) {

        if (ctx->CurrentInstruction->reg2 >= RegAF) {
            busWrite16(ctx->memoryDestination, ctx->fetchData);
        } else {
            busWrite(ctx->memoryDestination, ctx->fetchData);
        }

        return;
    }

    if (ctx->CurrentInstruction->mode == HL_SPR) {
        uint8_t hflag = (cpuReadReg(ctx->CurrentInstruction->reg2) & 0xF) + (ctx->fetchData & 0xF) >= 0x10;

        uint8_t cflag = cpuReadReg(ctx->CurrentInstruction->reg2) & 0xFF + (ctx->fetchData & 0xF) >= 0x100;

        cpuSetFlags(ctx, 0, 0, hflag, cflag);
        cpuSetReg(ctx->CurrentInstruction->reg1, cpuReadReg(ctx->CurrentInstruction->reg2) + (char)ctx->fetchData);

        return;
    }

    cpuSetReg(ctx->CurrentInstruction->reg1, ctx->fetchData);
}

static void ProcessLDH(CPUContext *ctx) {
    if (ctx->CurrentInstruction->reg1 == RegA) {
        cpuSetReg(ctx->CurrentInstruction->reg1, busRead(0xFF00 | ctx->fetchData));
    } else {
        busWrite(0xFF00 | ctx->fetchData, ctx->regs.A);
    }

    emuCycles(1);
}

static void goToAddress(CPUContext *ctx, uint16_t address, bool pushPC) {
    if (checkCondition(ctx)) {
        if (pushPC) {
            emuCycles(2);
            stackPush16(ctx->regs.PC);
        }

        ctx->regs.PC = address;
        emuCycles(1);
    }
}

static void ProcessJP(CPUContext *ctx) {
    goToAddress(ctx, ctx->fetchData, false);
}

static void ProcessJR(CPUContext *ctx) {
    int8_t rel = (int8_t)(ctx->fetchData & 0xFF);
    uint16_t address = ctx->regs.PC + rel;
    goToAddress(ctx, address, false);
}

static void ProcessCALL(CPUContext *ctx) {
    goToAddress(ctx, ctx->fetchData, true);
}

static void ProcessRST(CPUContext *ctx) {
    goToAddress(ctx, ctx->CurrentInstruction->param, true);
}

static void ProcessRET(CPUContext *ctx) {
    if (ctx->CurrentInstruction->mode != ConditionNone) {
        emuCycles(1);
    }

    if (checkCondition(ctx)) {
        uint16_t lo = stackPop();
        emuCycles(1);
        uint16_t hi = stackPop();
        emuCycles(1);

        uint16_t n = (hi << 8) | lo;
        ctx->regs.PC = n;
    }
}

static void ProcessRETI(CPUContext *ctx) {
    ctx->interruptEnabled = true;
    ProcessRET(ctx);
}

static void ProcessDI(CPUContext *ctx) {
    ctx->interruptEnabled = false;
}

static void ProcessXOR(CPUContext *ctx) {
    printf("A=0x%02X, fetchData=0x%02X\n", ctx->regs.A, ctx->fetchData);

    ctx->regs.A ^= ctx->fetchData;
    cpuSetFlags(ctx, ctx->regs.A == 0,0,0,0);
}

static void ProcessPOP(CPUContext *ctx) {
    uint16_t lo = stackPop();
    emuCycles(1);
    uint16_t hi = stackPop();
    emuCycles(1);

    uint16_t n = (hi << 8) | lo;

    cpuSetReg(ctx->CurrentInstruction->reg1, n);

    if (ctx->CurrentInstruction->reg1 == RegAF) {
        cpuSetReg(ctx->CurrentInstruction->reg1, n & 0xFFF0);
    }
}

static void ProcessPUSH(CPUContext *ctx) {
    uint16_t hi = (cpuReadReg(ctx->CurrentInstruction->reg1) >> 8) & 0xFF;
    emuCycles(1);
    stackPush(hi);

    uint16_t lo = cpuReadReg(ctx->CurrentInstruction->reg2) & 0xFF;
    emuCycles(1);
    stackPush(lo);

    emuCycles(1);
}

//INSTRUCTIONS LIST
static InstructionProcess processors[] = {
    [NONE] = ProcessNone,
    [NOP] = ProcessNOP,
    [LD] = ProcessLD,
    [LDH] = ProcessLDH,
    [JP] = ProcessJP,
    [DI] = ProcessDI,
    [XOR] = ProcessXOR,
    [POP] = ProcessPOP,
    [PUSH] = ProcessPUSH,
    [JR] = ProcessJR,
    [CALL] = ProcessCALL,
    [RST] = ProcessRST,
    [RETI] = ProcessRETI,
    [RET] = ProcessRET,
};

InstructionProcess instructionGetProcessor(instructionType type) {
    return processors[type];
}