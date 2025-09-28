#include "../inc/bus.h"
#include "../inc/cpu.h"
#include "../inc/emu.h"
#include "../inc/stack.h"

//CHECK IF 16BIT
static bool is16Bit(registerType rt) {
    return rt >= RegAF;
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

//CHECK CURRENT CONDITIONS (FLAGS)
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

//CPU INSTRUCTION PROCESSING
static void ProcessNone(CPUContext *ctx) {
    printf("INVALID INSTRUCTION [0x%02X]\n",ctx->currentOPCode);
    exit(-7);
}

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

static void ProcessJP(CPUContext *ctx) {
    goToAddress(ctx, ctx->fetchData, false);
}

static void ProcessJR(CPUContext *ctx) {
    int8_t rel = (int8_t)(ctx->fetchData & 0xFF);
    uint16_t address = ctx->regs.PC + rel;
    goToAddress(ctx, address, false);
}

static void ProcessCALL(CPUContext *ctx) {
    if (checkCondition(ctx)) {
        uint16_t returnAddress = ctx->regs.PC + 3;
        stackPush16(returnAddress);
        emuCycles(2);

        ctx->regs.PC = ctx->fetchData;
        emuCycles(1);
    }
}

static void ProcessRST(CPUContext *ctx) {
    stackPush16(ctx->regs.PC +1);
    ctx->regs.PC = ctx->CurrentInstruction->param;
    emuCycles(1);
}

static void ProcessRET(CPUContext *ctx) {
    if (ctx->CurrentInstruction->mode != ConditionNone) {
        emuCycles(1);
    }

    if (checkCondition(ctx)) {
        ctx->regs.PC = stackPop16();
        emuCycles(2);
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

static void ProcessINC(CPUContext *ctx) {
    uint16_t value = cpuReadReg(ctx->CurrentInstruction->reg1) + 1;

    if (is16Bit(ctx->CurrentInstruction->reg1)) {
        emuCycles(1);
    }

    if (ctx->CurrentInstruction->reg1 == RegHL && ctx->CurrentInstruction->mode == MEMREG) {
        value = busRead(cpuReadReg(RegHL)) + 1;
        value &= 0xFF;
        busWrite(cpuReadReg(RegHL), value);
    } else {
        cpuSetReg(ctx->CurrentInstruction->reg1, value);
        value = cpuReadReg(ctx->CurrentInstruction->reg1);
    }

    if ((ctx->currentOPCode & 0x03) == 0x03) {
        return;
    }
    cpuSetFlags(ctx, value == 0,0, (value, 0x0F) == 0, -1);
}

static void ProcessDEC(CPUContext *ctx) {
    if (is16Bit(ctx->CurrentInstruction->reg1)) {

        uint16_t value = cpuReadReg(ctx->CurrentInstruction->reg1) - 1;
        cpuSetReg(ctx->CurrentInstruction->reg1, value);
        emuCycles(1);
    } else if (ctx->CurrentInstruction->reg1 == RegHL && ctx->CurrentInstruction->mode == MEMREG) {
        uint8_t value = busRead(cpuReadReg(RegHL));
        uint8_t result = value -1;
        busWrite(cpuReadReg(RegHL), result);

        cpuSetFlags(ctx, result == 0,1, (value & 0x0F) == 0x00, -1);
        emuCycles(3);
    } else {
        uint8_t value = cpuReadReg(ctx->CurrentInstruction->reg1);
        uint8_t result = value - 1;
        cpuSetReg(ctx->CurrentInstruction->reg1, result);
        cpuSetFlags(ctx, result == 0, 1, (value & 0x0F) == 0x00, -1);
        emuCycles(1);
    }
}

static void ProcessADD(CPUContext *ctx) {
    uint32_t value = cpuReadReg(ctx->CurrentInstruction->reg1) + ctx->fetchData;

    bool isIt16Bit = is16Bit(ctx->CurrentInstruction->reg1);

    if (isIt16Bit) {
        emuCycles(1);
    }

    if (ctx->CurrentInstruction->reg1 == RegSP) {
        value = cpuReadReg(ctx->CurrentInstruction->reg1) + ctx->fetchData;
    }

    int z = (value & 0xFF) == 0;
    int h = (cpuReadReg(ctx->CurrentInstruction->reg1) & 0xF) + (ctx->fetchData & 0xF) >= 0x10;
    int c = (int)(cpuReadReg(ctx->CurrentInstruction->reg1) & 0xFF) + (int)(ctx->fetchData & 0xFF) >= 0x100;

    if (isIt16Bit) {
        z = -1;
        h = (cpuReadReg(ctx->CurrentInstruction->reg1) & 0xFFF) + (ctx->fetchData & 0xFFF) >= 0x1000;
        uint32_t n = ((uint32_t)cpuReadReg(ctx->CurrentInstruction->reg1)) + ((uint32_t)ctx->fetchData);
        c = n >= 0x10000;
    }

    if (ctx->CurrentInstruction->reg1 == RegSP) {
        z = 0;
        h = (cpuReadReg(ctx->CurrentInstruction->reg1) & 0xF) + (ctx->fetchData & 0xF) >= 0x10;
        c = (int)(cpuReadReg(ctx->CurrentInstruction->reg1) & 0xFF) + (int)(ctx->fetchData & 0xFF) >= 0x100;
    }

    cpuSetReg(ctx->CurrentInstruction->reg1, value & 0xFFFF);
    cpuSetFlags(ctx,z,0,h,c);
}

static void ProcessSUB(CPUContext *ctx) {
    uint16_t value = cpuReadReg(ctx->CurrentInstruction->reg1) - ctx->fetchData;

    int z = value == 0;
    int h = ((int)cpuReadReg(ctx->CurrentInstruction->reg1) & 0xF) - ((int)ctx->fetchData & 0xF) < 0;
    int c = ((int)cpuReadReg(ctx->CurrentInstruction->reg1)) - ((int)ctx->fetchData) < 0;

    cpuSetReg(ctx->CurrentInstruction->reg1, value);
    cpuSetFlags(ctx, z, 1, h, c);
}

static void ProcessADC(CPUContext *ctx) {
    uint16_t u = ctx->fetchData;
    uint16_t a = ctx->regs.A;
    uint16_t c = CPUFLAGC;

    ctx->regs.A = (a + u + c) & 0xFF;

    cpuSetFlags(ctx, ctx->regs.A == 0,0,(a & 0xF) + (u & 0xF) + c > 0xF, a+u+c > 0xFF);
}

static void ProcessSBC(CPUContext *ctx) {
    uint8_t value = ctx->fetchData + CPUFLAGC;

    int z = cpuReadReg(ctx->CurrentInstruction->reg1) - value == 0;
    int h = ((int)cpuReadReg(ctx->CurrentInstruction->reg1) & 0xF) - ((int)ctx->fetchData & 0xF) - ((int)CPUFLAGC) < 0;
    int c = ((int)cpuReadReg(ctx->CurrentInstruction->reg1)) - ((int)ctx->fetchData) -((int)CPUFLAGC) < 0;

    cpuSetReg(ctx->CurrentInstruction->reg1, cpuReadReg(ctx->CurrentInstruction->reg1) - value);
    cpuSetFlags(ctx, z, 1, h, c);
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
    [INC] = ProcessINC,
    [DEC] = ProcessDEC,
    [SUB] = ProcessSUB,
    [SBC] = ProcessSBC,
    [ADC] = ProcessADC,
    [ADD] = ProcessADD,
};

InstructionProcess instructionGetProcessor(instructionType type) {
    return processors[type];
}