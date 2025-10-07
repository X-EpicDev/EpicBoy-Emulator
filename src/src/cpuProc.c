#include "../inc/cpu.h"
#include "../inc/emu.h"
#include "../inc/bus.h"
#include "../inc/stack.h"

//Processes CPU Instructions

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

static void ProcessNONE(CPUContext *ctx) {
    printf("INVALID INSTRUCTION [0x%02X]\n",ctx->currentOPCode);
    exit(-7);
}

static void ProcessNOP(CPUContext *ctx) {

}

registerType regTypeLookup[] = {
    RegB,
    RegC,
    RegD,
    RegE,
    RegH,
    RegL,
    RegHL,
    RegA
};

registerType decodeRegister(uint8_t reg) {
    if (reg > 0b111) {
        return RegNone;
    }

    return regTypeLookup[reg];
}

static void ProcessCB(CPUContext *ctx) {
    uint8_t op = ctx->fetchedData;
    registerType reg = decodeRegister(op & 0b111);
    uint8_t bit = (op >> 3) & 0b111;
    uint8_t bitOP = (op >> 6) & 0b11;
    uint8_t registerValue = cpuReadReg8(reg);

    emuCycles(1);

    if (reg == RegHL) {
        emuCycles(2);
    }

    switch (bitOP) {
        case 1:
            // BIT
            cpuSetFlags(ctx, !(registerValue & (1 << bit)), 0, 1, -1);
            return;

        case 2:
            // RST
            registerValue &= ~(1 << bit);
            cpuSetReg8(reg, registerValue);
            return;

        case 3:
            // SET
            registerValue |= (1 << bit);
            cpuSetReg8(reg, registerValue);
            return;
    }

    bool flagC = CPUFLAGC;

    switch (bit) {
        case 0: {
            // RLC
            bool setC = false;
            uint8_t result = (registerValue << 1) & 0xFF;

            if ((registerValue & (1 << 7)) != 0) {
                result |= 1;
                setC = true;
            }

            cpuSetReg8(reg, result);
            cpuSetFlags(ctx, result == 0, false, false, setC);
        } return;

        case 1: {
            // RRC
            uint8_t old = registerValue;
            registerValue >>= 1;
            registerValue |= (old << 7);

            cpuSetReg8(reg, registerValue);
            cpuSetFlags(ctx, !registerValue, false, false, old & 1);
        } return;

        case 2: {
            // RL
            uint8_t old = registerValue;
            registerValue <<= 1;
            registerValue |= flagC;

            cpuSetReg8(reg, registerValue);
            cpuSetFlags(ctx, !registerValue, false, false, !!(old & 0x80));
        } return;

        case 3: {
            // RR
            uint8_t old = registerValue;
            registerValue >>= 1;

            registerValue |= (flagC << 7);

            cpuSetReg8(reg, registerValue);
            cpuSetFlags(ctx, !registerValue, false, false, old & 1);
        } return;

        case 4: {
            // SLA
            uint8_t old = registerValue;
            registerValue <<= 1;

            cpuSetReg8(reg, registerValue);
            cpuSetFlags(ctx, !registerValue, false, false, !!(old & 0x80));
            } return;

        case 5: {
            // SRA
            uint8_t u = (int8_t)registerValue >> 1;
            cpuSetReg8(reg, u);
            cpuSetFlags(ctx, !u, 0, 0, registerValue & 1);
        } return;

        case 6: {
            // SWAP
            registerValue = ((registerValue & 0xF0) >> 4) | ((registerValue & 0xF) << 4);
            cpuSetReg8(reg, registerValue);
            cpuSetFlags(ctx, registerValue == 0, false, false, false);
        } return;

        case 7: {
            // SRL
            uint8_t u = registerValue >> 1;
            cpuSetReg8(reg, u);
            cpuSetFlags(ctx, !u, 0, 0, registerValue & 1);
        } return;
    }

    fprintf(stderr, "ERROR: INVALID CB: %02X\n", op);
    NOIMPL
}

static void ProcessRLCA(CPUContext *ctx) {
    uint8_t u = ctx->regs.A;
    bool c = (u >> 7) & 1;
    u = (u << 1) | c;
    ctx->regs.A = u;

    cpuSetFlags(ctx, 0,0,0,c);
}

static void ProcessRRCA(CPUContext *ctx) {
    uint8_t b = ctx->regs.A & 1;
    ctx->regs.A >>= 1;
    ctx->regs.A |= (b << 7);

    cpuSetFlags(ctx, 0,0,0,b);
}


static void ProcessRLA(CPUContext *ctx) {
    uint8_t u = ctx->regs.A;
    uint8_t cf = CPUFLAGC;
    uint8_t c = (u >> 7) & 1;

    ctx->regs.A = (u << 1) | cf;
    cpuSetFlags(ctx, 0, 0, 0, c);
}

static void ProcessSTOP(CPUContext *ctx) {
    fprintf(stderr, "CPU STOPPING\n");
    NOIMPL
}

static void ProcessDAA(CPUContext *ctx) {
    uint8_t u = 0;
    int fc = 0;

    if (CPUFLAGH || (!CPUFLAGN && (ctx->regs.A & 0xF) > 9)) {
        u = 6;
    }

    if (CPUFLAGC || (!CPUFLAGN && ctx->regs.A > 0x99)) {
        u |= 0x60;
        fc = 1;
    }

    ctx->regs.A += CPUFLAGN ? -u : u;

    cpuSetFlags(ctx, ctx->regs.A == 0, -1, 0, fc);
}

static void ProcessCPL(CPUContext *ctx) {
    ctx->regs.A = ~ctx->regs.A;
    cpuSetFlags(ctx, -1, 1, 1, -1);
}

static void ProcessSCF(CPUContext *ctx) {
    cpuSetFlags(ctx, -1, 0, 0, 1);
}

static void ProcessCCF(CPUContext *ctx) {
    cpuSetFlags(ctx, -1, 0, 0, CPUFLAGC ^ 1);
}

static void ProcessHALT(CPUContext *ctx) {
    ctx->halted = true;
}

static void ProcessRRA(CPUContext *ctx) {
    uint8_t carry = CPUFLAGC;
    uint8_t new_c = ctx->regs.A & 1;

    ctx->regs.A >>= 1;
    ctx->regs.A |= (carry << 7);

    cpuSetFlags(ctx, 0, 0, 0, new_c);
}

static void ProcessAND(CPUContext *ctx) {
    ctx->regs.A &= ctx->fetchedData;
    cpuSetFlags(ctx, ctx->regs.A == 0, 0, 1, 0);
}

static void ProcessXOR(CPUContext *ctx) {
    ctx->regs.A ^= ctx->fetchedData & 0xFF;
    cpuSetFlags(ctx, ctx->regs.A == 0, 0, 0, 0);
}

static void ProcessOR(CPUContext *ctx) {
    ctx->regs.A |= ctx->fetchedData & 0xFF;
    cpuSetFlags(ctx, ctx->regs.A == 0, 0, 0, 0);
}

static void ProcessCP(CPUContext *ctx) {
    int n = (int)ctx->regs.A - (int)ctx->fetchedData;

    cpuSetFlags(ctx, n == 0, 1,
        ((int)ctx->regs.A & 0x0F) - ((int)ctx->fetchedData & 0x0F) < 0, n < 0);
}

static void ProcessDI(CPUContext *ctx) {
    ctx->interruptMasterEnabled = false;
}

static void ProcessEI(CPUContext *ctx) {
    ctx->enablingIME = true;
}

static bool is16Bit(registerType rt) {
    return rt >= RegAF;
}

static void ProcessLD(CPUContext *ctx) {
    if (ctx->destinationIsMemory) {
        //LD (BC), A for instance...

        if (is16Bit(ctx->currentInstruction->reg2)) {
            //if 16 bit register...
            emuCycles(1);
            busWrite16(ctx->memoryDestination, ctx->fetchedData);
        } else {
            busWrite(ctx->memoryDestination, ctx->fetchedData);
        }

        emuCycles(1);

        return;
    }

    if (ctx->currentInstruction->mode == HL_SPR) {
        uint8_t hflag = (cpuReadReg(ctx->currentInstruction->reg2) & 0xF) +
            (ctx->fetchedData & 0xF) >= 0x10;

        uint8_t cflag = (cpuReadReg(ctx->currentInstruction->reg2) & 0xFF) +
            (ctx->fetchedData & 0xFF) >= 0x100;

        cpuSetFlags(ctx, 0, 0, hflag, cflag);
        cpuSetReg(ctx->currentInstruction->reg1,
            cpuReadReg(ctx->currentInstruction->reg2) + (char)ctx->fetchedData);

        return;
    }

    cpuSetReg(ctx->currentInstruction->reg1, ctx->fetchedData);
}

static void ProcessLDH(CPUContext *ctx) {
    if (ctx->currentInstruction->reg1 == RegA) {
        cpuSetReg(ctx->currentInstruction->reg1, busRead(0xFF00 | ctx->fetchedData));
    } else {
        busWrite(ctx->memoryDestination, ctx->regs.A);
    }

    emuCycles(1);
}


static bool checkCondition(CPUContext *ctx) {
    bool z = CPUFLAGZ;
    bool c = CPUFLAGC;

    switch(ctx->currentInstruction->condition) {
        case ConditionNone: return true;
        case ConditionC: return c;
        case ConditionNC: return !c;
        case ConditionZ: return z;
        case ConditionNZ: return !z;
    }

    return false;
}

static void goToAddress(CPUContext *ctx, uint16_t addr, bool pushpc) {
    if (checkCondition(ctx)) {
        if (pushpc) {
            emuCycles(2);
            stackPush16(ctx->regs.PC);
        }

        ctx->regs.PC = addr;
        emuCycles(1);
    }
}

static void ProcessJP(CPUContext *ctx) {
    goToAddress(ctx, ctx->fetchedData, false);
}

static void ProcessJR(CPUContext *ctx) {
    int8_t rel = (char)(ctx->fetchedData & 0xFF);
    uint16_t addr = ctx->regs.PC + rel;

    goToAddress(ctx, addr, false);
}

static void ProcessCALL(CPUContext *ctx) {
    goToAddress(ctx, ctx->fetchedData, true);
}

static void ProcessRST(CPUContext *ctx) {
    goToAddress(ctx, ctx->currentInstruction->parameter, true);
}

static void ProcessRET(CPUContext *ctx) {
    if (ctx->currentInstruction->condition != ConditionNone) {
        emuCycles(1);
    }

    if (checkCondition(ctx)) {
        uint16_t lo = stackPop();
        emuCycles(1);
        uint16_t hi = stackPop();
        emuCycles(1);

        uint16_t n = (hi << 8) | lo;
        ctx->regs.PC = n;

        emuCycles(1);
    }
}

static void ProcessRETI(CPUContext *ctx) {
    ctx->interruptMasterEnabled = true;
    ProcessRET(ctx);
}

static void ProcessPOP(CPUContext *ctx) {
    uint16_t lo = stackPop();
    emuCycles(1);
    uint16_t hi = stackPop();
    emuCycles(1);

    uint16_t n = (hi << 8) | lo;

    cpuSetReg(ctx->currentInstruction->reg1, n);

    if (ctx->currentInstruction->reg1 == RegAF) {
        cpuSetReg(ctx->currentInstruction->reg1, n & 0xFFF0);
    }
}

static void ProcessPUSH(CPUContext *ctx) {
    uint16_t hi = (cpuReadReg(ctx->currentInstruction->reg1) >> 8) & 0xFF;
    emuCycles(1);
    stackPush(hi);

    uint16_t lo = cpuReadReg(ctx->currentInstruction->reg1) & 0xFF;
    emuCycles(1);
    stackPush(lo);

    emuCycles(1);
}

static void ProcessINC(CPUContext *ctx) {
    uint16_t val = cpuReadReg(ctx->currentInstruction->reg1) + 1;

    if (is16Bit(ctx->currentInstruction->reg1)) {
        emuCycles(1);
    }

    if (ctx->currentInstruction->reg1 == RegHL && ctx->currentInstruction->mode == MEMREG) {
        val = busRead(cpuReadReg(RegHL)) + 1;
        val &= 0xFF;
        busWrite(cpuReadReg(RegHL), val);
    } else {
        cpuSetReg(ctx->currentInstruction->reg1, val);
        val = cpuReadReg(ctx->currentInstruction->reg1);
    }

    if ((ctx->currentOPCode & 0x03) == 0x03) {
        return;
    }

    cpuSetFlags(ctx, val == 0, 0, (val & 0x0F) == 0, -1);
}

static void ProcessDEC(CPUContext *ctx) {
    uint16_t val = cpuReadReg(ctx->currentInstruction->reg1) - 1;

    if (is16Bit(ctx->currentInstruction->reg1)) {
        emuCycles(1);
    }

    if (ctx->currentInstruction->reg1 == RegHL && ctx->currentInstruction->mode == MEMREG) {
        val = busRead(cpuReadReg(RegHL)) - 1;
        busWrite(cpuReadReg(RegHL), val);
    } else {
        cpuSetReg(ctx->currentInstruction->reg1, val);
        val = cpuReadReg(ctx->currentInstruction->reg1);
    }

    if ((ctx->currentOPCode & 0x0B) == 0x0B) {
        return;
    }

    cpuSetFlags(ctx, val == 0, 1, (val & 0x0F) == 0x0F, -1);
}

static void ProcessSUB(CPUContext *ctx) {
    uint16_t val = cpuReadReg(ctx->currentInstruction->reg1) - ctx->fetchedData;

    int z = val == 0;
    int h = ((int)cpuReadReg(ctx->currentInstruction->reg1) & 0xF) - ((int)ctx->fetchedData & 0xF) < 0;
    int c = ((int)cpuReadReg(ctx->currentInstruction->reg1)) - ((int)ctx->fetchedData) < 0;

    cpuSetReg(ctx->currentInstruction->reg1, val);
    cpuSetFlags(ctx, z, 1, h, c);
}

static void ProcessSBC(CPUContext *ctx) {
    uint8_t val = ctx->fetchedData + CPUFLAGC;

    int z = cpuReadReg(ctx->currentInstruction->reg1) - val == 0;

    int h = ((int)cpuReadReg(ctx->currentInstruction->reg1) & 0xF)
        - ((int)ctx->fetchedData & 0xF) - ((int)CPUFLAGC) < 0;
    int c = ((int)cpuReadReg(ctx->currentInstruction->reg1))
        - ((int)ctx->fetchedData) - ((int)CPUFLAGC) < 0;

    cpuSetReg(ctx->currentInstruction->reg1, cpuReadReg(ctx->currentInstruction->reg1) - val);
    cpuSetFlags(ctx, z, 1, h, c);
}

static void ProcessADC(CPUContext *ctx) {
    uint16_t u = ctx->fetchedData;
    uint16_t a = ctx->regs.A;
    uint16_t c = CPUFLAGC;

    ctx->regs.A = (a + u + c) & 0xFF;

    cpuSetFlags(ctx, ctx->regs.A == 0, 0,
        (a & 0xF) + (u & 0xF) + c > 0xF,
        a + u + c > 0xFF);
}

static void ProcessADD(CPUContext *ctx) {
    uint32_t val = cpuReadReg(ctx->currentInstruction->reg1) + ctx->fetchedData;

    bool is_16bit = is16Bit(ctx->currentInstruction->reg1);

    if (is_16bit) {
        emuCycles(1);
    }

    if (ctx->currentInstruction->reg1 == RegSP) {
        val = cpuReadReg(ctx->currentInstruction->reg1) + (char)ctx->fetchedData;
    }

    int z = (val & 0xFF) == 0;
    int h = (cpuReadReg(ctx->currentInstruction->reg1) & 0xF) + (ctx->fetchedData & 0xF) >= 0x10;
    int c = (int)(cpuReadReg(ctx->currentInstruction->reg1) & 0xFF) + (int)(ctx->fetchedData & 0xFF) >= 0x100;

    if (is_16bit) {
        z = -1;
        h = (cpuReadReg(ctx->currentInstruction->reg1) & 0xFFF) + (ctx->fetchedData & 0xFFF) >= 0x1000;
        uint32_t n = ((uint32_t)cpuReadReg(ctx->currentInstruction->reg1)) + ((uint32_t)ctx->fetchedData);
        c = n >= 0x10000;
    }

    if (ctx->currentInstruction->reg1 == RegSP) {
        z = 0;
        h = (cpuReadReg(ctx->currentInstruction->reg1) & 0xF) + (ctx->fetchedData & 0xF) >= 0x10;
        c = (int)(cpuReadReg(ctx->currentInstruction->reg1) & 0xFF) + (int)(ctx->fetchedData & 0xFF) >= 0x100;
    }

    cpuSetReg(ctx->currentInstruction->reg1, val & 0xFFFF);
    cpuSetFlags(ctx, z, 0, h, c);
}

static InstructionProcess processors[] = {
    [NONE] = ProcessNONE,
    [NOP] = ProcessNOP,
    [LD] = ProcessLD,
    [LDH] = ProcessLDH,
    [JP] = ProcessJP,
    [DI] = ProcessDI,
    [POP] = ProcessPOP,
    [PUSH] = ProcessPUSH,
    [JR] = ProcessJR,
    [CALL] = ProcessCALL,
    [RET] = ProcessRET,
    [RST] = ProcessRST,
    [DEC] = ProcessDEC,
    [INC] = ProcessINC,
    [ADD] = ProcessADD,
    [ADC] = ProcessADC,
    [SUB] = ProcessSUB,
    [SBC] = ProcessSBC,
    [AND] = ProcessAND,
    [XOR] = ProcessXOR,
    [OR] = ProcessOR,
    [CP] = ProcessCP,
    [CB] = ProcessCB,
    [RRCA] = ProcessRRCA,
    [RLCA] = ProcessRLCA,
    [RRA] = ProcessRRA,
    [RLA] = ProcessRLA,
    [STOP] = ProcessSTOP,
    [HALT] = ProcessHALT,
    [DAA] = ProcessDAA,
    [CPL] = ProcessCPL,
    [SCF] = ProcessSCF,
    [CCF] = ProcessCCF,
    [EI] = ProcessEI,
    [RETI] = ProcessRETI,
};

InstructionProcess instructionGetProcessor(instructionType type) {
    return processors[type];
}