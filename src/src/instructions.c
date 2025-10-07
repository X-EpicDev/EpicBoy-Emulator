#include "../inc/instructions.h"
#include "../inc/cpu.h"
#include "../inc/bus.h"

instruction instructions[0x100] = {
    //0x0X
    [0x00] = {NOP, IMPL},
    [0x01] = {LD, REG_D16, RegBC},
    [0x02] = {LD, MEMREG_REG, RegBC, RegA},
    [0x03] = {INC, REG, RegBC},
    [0x04] = {INC, REG, RegB},
    [0x05] = {DEC, REG, RegB},
    [0x06] = {LD, REG_D8, RegB},
    [0x07] = {RLCA},
    [0x08] = {LD, A16_REG, NONE, RegSP},
    [0x09] = {ADD, REG_REG, RegHL, RegBC},
    [0x0A] = {LD, REG_MEMREG, RegA, RegBC},
    [0x0B] = {DEC, REG, RegBC},
    [0x0C] = {INC, REG, RegC},
    [0x0D] = {DEC, REG, RegC},
    [0x0E] = {LD, REG_D8, RegC},
    [0x0F] = {RRCA},

    //0x1X
    [0x10] = {STOP},
    [0x11] = {LD, REG_D16, RegDE},
    [0x12] = {LD, MEMREG_REG, RegDE, RegA},
    [0x13] = {INC, REG, RegDE},
    [0x14] = {INC, REG, RegD},
    [0x15] = {DEC, REG, RegD},
    [0x16] = {LD, REG_D8, RegD},
    [0x17] = {RLA},
    [0x18] = {JR, D8},
    [0x19] = {ADD, REG_REG, RegHL, RegDE},
    [0x1A] = {LD, REG_MEMREG, RegA, RegDE},
    [0x1B] = {DEC, REG, RegDE},
    [0x1C] = {INC, REG, RegE},
    [0x1D] = {DEC, REG, RegE},
    [0x1E] = {LD, REG_D8, RegE},
    [0x1F] = {RRA},

    //0x2X
    [0x20] = {JR, D8, NONE, NONE, ConditionNZ},
    [0x21] = {LD, REG_D16, RegHL},
    [0x22] = {LD, HLI_REG, RegHL, RegA},
    [0x23] = {INC, REG, RegHL},
    [0x24] = {INC, REG, RegH},
    [0x25] = {DEC, REG, RegH},
    [0x26] = {LD, REG_D8, RegH},
    [0x27] = {DAA},
    [0x28] = {JR, D8, NONE, NONE, ConditionZ},
    [0x29] = {ADD, REG_REG, RegHL, RegHL},
    [0x2A] = {LD, REG_HLI, RegA, RegHL},
    [0x2B] = {DEC, REG, RegHL},
    [0x2C] = {INC, REG, RegL},
    [0x2D] = {DEC, REG, RegL},
    [0x2E] = {LD, REG_D8, RegL},
    [0x2F] = {CPL},

    //0x3X
    [0x30] = {JR, D8, NONE, NONE, ConditionNC},
    [0x31] = {LD, REG_D16, RegSP},
    [0x32] = {LD, HLD_REG, RegHL, RegA},
    [0x33] = {INC, REG, RegSP},
    [0x34] = {INC, MEMREG, RegHL},
    [0x35] = {DEC, MEMREG, RegHL},
    [0x36] = {LD, MEMREG_D8, RegHL},
    [0x37] = {SCF},
    [0x38] = {JR, D8, NONE, NONE, ConditionC},
    [0x39] = {ADD, REG_REG, RegHL, RegSP},
    [0x3A] = {LD, REG_HLD, RegA, RegHL},
    [0x3B] = {DEC, REG, RegSP},
    [0x3C] = {INC, REG, RegA},
    [0x3D] = {DEC, REG, RegA},
    [0x3E] = {LD, REG_D8, RegA},
    [0x3F] = {CCF},

    //0x4X
    [0x40] = {LD, REG_REG, RegB, RegB},
    [0x41] = {LD, REG_REG, RegB, RegC},
    [0x42] = {LD, REG_REG, RegB, RegD},
    [0x43] = {LD, REG_REG, RegB, RegE},
    [0x44] = {LD, REG_REG, RegB, RegH},
    [0x45] = {LD, REG_REG, RegB, RegL},
    [0x46] = {LD, REG_MEMREG, RegB, RegHL},
    [0x47] = {LD, REG_REG, RegB, RegA},
    [0x48] = {LD, REG_REG, RegC, RegB},
    [0x49] = {LD, REG_REG, RegC, RegC},
    [0x4A] = {LD, REG_REG, RegC, RegD},
    [0x4B] = {LD, REG_REG, RegC, RegE},
    [0x4C] = {LD, REG_REG, RegC, RegH},
    [0x4D] = {LD, REG_REG, RegC, RegL},
    [0x4E] = {LD, REG_MEMREG, RegC, RegHL},
    [0x4F] = {LD, REG_REG, RegC, RegA},

    //0x5X
    [0x50] = {LD, REG_REG,  RegD, RegB},
    [0x51] = {LD, REG_REG,  RegD, RegC},
    [0x52] = {LD, REG_REG,  RegD, RegD},
    [0x53] = {LD, REG_REG,  RegD, RegE},
    [0x54] = {LD, REG_REG,  RegD, RegH},
    [0x55] = {LD, REG_REG,  RegD, RegL},
    [0x56] = {LD, REG_MEMREG, RegD, RegHL},
    [0x57] = {LD, REG_REG,  RegD, RegA},
    [0x58] = {LD, REG_REG,  RegE, RegB},
    [0x59] = {LD, REG_REG,  RegE, RegC},
    [0x5A] = {LD, REG_REG,  RegE, RegD},
    [0x5B] = {LD, REG_REG,  RegE, RegE},
    [0x5C] = {LD, REG_REG,  RegE, RegH},
    [0x5D] = {LD, REG_REG,  RegE, RegL},
    [0x5E] = {LD, REG_MEMREG, RegE, RegHL},
    [0x5F] = {LD, REG_REG,  RegE, RegA},

    //0x6X
    [0x60] = {LD, REG_REG,  RegH, RegB},
    [0x61] = {LD, REG_REG,  RegH, RegC},
    [0x62] = {LD, REG_REG,  RegH, RegD},
    [0x63] = {LD, REG_REG,  RegH, RegE},
    [0x64] = {LD, REG_REG,  RegH, RegH},
    [0x65] = {LD, REG_REG,  RegH, RegL},
    [0x66] = {LD, REG_MEMREG, RegH, RegHL},
    [0x67] = {LD, REG_REG,  RegH, RegA},
    [0x68] = {LD, REG_REG,  RegL, RegB},
    [0x69] = {LD, REG_REG,  RegL, RegC},
    [0x6A] = {LD, REG_REG,  RegL, RegD},
    [0x6B] = {LD, REG_REG,  RegL, RegE},
    [0x6C] = {LD, REG_REG,  RegL, RegH},
    [0x6D] = {LD, REG_REG,  RegL, RegL},
    [0x6E] = {LD, REG_MEMREG, RegL, RegHL},
    [0x6F] = {LD, REG_REG,  RegL, RegA},

    //0x7X
    [0x70] = {LD, MEMREG_REG,  RegHL, RegB},
    [0x71] = {LD, MEMREG_REG,  RegHL, RegC},
    [0x72] = {LD, MEMREG_REG,  RegHL, RegD},
    [0x73] = {LD, MEMREG_REG,  RegHL, RegE},
    [0x74] = {LD, MEMREG_REG,  RegHL, RegH},
    [0x75] = {LD, MEMREG_REG,  RegHL, RegL},
    [0x76] = {HALT},
    [0x77] = {LD, MEMREG_REG,  RegHL, RegA},
    [0x78] = {LD, REG_REG,  RegA, RegB},
    [0x79] = {LD, REG_REG,  RegA, RegC},
    [0x7A] = {LD, REG_REG,  RegA, RegD},
    [0x7B] = {LD, REG_REG,  RegA, RegE},
    [0x7C] = {LD, REG_REG,  RegA, RegH},
    [0x7D] = {LD, REG_REG,  RegA, RegL},
    [0x7E] = {LD, REG_MEMREG, RegA, RegHL},
    [0x7F] = {LD, REG_REG,  RegA, RegA},

    //0x8X
    [0x80] = {ADD, REG_REG, RegA, RegB},
    [0x81] = {ADD, REG_REG, RegA, RegC},
    [0x82] = {ADD, REG_REG, RegA, RegD},
    [0x83] = {ADD, REG_REG, RegA, RegE},
    [0x84] = {ADD, REG_REG, RegA, RegH},
    [0x85] = {ADD, REG_REG, RegA, RegL},
    [0x86] = {ADD, REG_MEMREG, RegA, RegHL},
    [0x87] = {ADD, REG_REG, RegA, RegA},
    [0x88] = {ADC, REG_REG, RegA, RegB},
    [0x89] = {ADC, REG_REG, RegA, RegC},
    [0x8A] = {ADC, REG_REG, RegA, RegD},
    [0x8B] = {ADC, REG_REG, RegA, RegE},
    [0x8C] = {ADC, REG_REG, RegA, RegH},
    [0x8D] = {ADC, REG_REG, RegA, RegL},
    [0x8E] = {ADC, REG_MEMREG, RegA, RegHL},
    [0x8F] = {ADC, REG_REG, RegA, RegA},

    //0x9X
    [0x90] = {SUB, REG_REG, RegA, RegB},
    [0x91] = {SUB, REG_REG, RegA, RegC},
    [0x92] = {SUB, REG_REG, RegA, RegD},
    [0x93] = {SUB, REG_REG, RegA, RegE},
    [0x94] = {SUB, REG_REG, RegA, RegH},
    [0x95] = {SUB, REG_REG, RegA, RegL},
    [0x96] = {SUB, REG_MEMREG, RegA, RegHL},
    [0x97] = {SUB, REG_REG, RegA, RegA},
    [0x98] = {SBC, REG_REG, RegA, RegB},
    [0x99] = {SBC, REG_REG, RegA, RegC},
    [0x9A] = {SBC, REG_REG, RegA, RegD},
    [0x9B] = {SBC, REG_REG, RegA, RegE},
    [0x9C] = {SBC, REG_REG, RegA, RegH},
    [0x9D] = {SBC, REG_REG, RegA, RegL},
    [0x9E] = {SBC, REG_MEMREG, RegA, RegHL},
    [0x9F] = {SBC, REG_REG, RegA, RegA},

    //0xAX
    [0xA0] = {AND, REG_REG, RegA, RegB},
    [0xA1] = {AND, REG_REG, RegA, RegC},
    [0xA2] = {AND, REG_REG, RegA, RegD},
    [0xA3] = {AND, REG_REG, RegA, RegE},
    [0xA4] = {AND, REG_REG, RegA, RegH},
    [0xA5] = {AND, REG_REG, RegA, RegL},
    [0xA6] = {AND, REG_MEMREG, RegA, RegHL},
    [0xA7] = {AND, REG_REG, RegA, RegA},
    [0xA8] = {XOR, REG_REG, RegA, RegB},
    [0xA9] = {XOR, REG_REG, RegA, RegC},
    [0xAA] = {XOR, REG_REG, RegA, RegD},
    [0xAB] = {XOR, REG_REG, RegA, RegE},
    [0xAC] = {XOR, REG_REG, RegA, RegH},
    [0xAD] = {XOR, REG_REG, RegA, RegL},
    [0xAE] = {XOR, REG_MEMREG, RegA, RegHL},
    [0xAF] = {XOR, REG_REG, RegA, RegA},

    //0xBX
    [0xB0] = {OR, REG_REG, RegA, RegB},
    [0xB1] = {OR, REG_REG, RegA, RegC},
    [0xB2] = {OR, REG_REG, RegA, RegD},
    [0xB3] = {OR, REG_REG, RegA, RegE},
    [0xB4] = {OR, REG_REG, RegA, RegH},
    [0xB5] = {OR, REG_REG, RegA, RegL},
    [0xB6] = {OR, REG_MEMREG, RegA, RegHL},
    [0xB7] = {OR, REG_REG, RegA, RegA},
    [0xB8] = {CP, REG_REG, RegA, RegB},
    [0xB9] = {CP, REG_REG, RegA, RegC},
    [0xBA] = {CP, REG_REG, RegA, RegD},
    [0xBB] = {CP, REG_REG, RegA, RegE},
    [0xBC] = {CP, REG_REG, RegA, RegH},
    [0xBD] = {CP, REG_REG, RegA, RegL},
    [0xBE] = {CP, REG_MEMREG, RegA, RegHL},
    [0xBF] = {CP, REG_REG, RegA, RegA},

    [0xC0] = {RET, IMPL, NONE, NONE, ConditionNZ},
    [0xC1] = {POP, REG, RegBC},
    [0xC2] = {JP, D16, NONE, NONE, ConditionNZ},
    [0xC3] = {JP, D16},
    [0xC4] = {CALL, D16, NONE, NONE, ConditionNZ},
    [0xC5] = {PUSH, REG, RegBC},
    [0xC6] = {ADD, REG_D8, RegA},
    [0xC7] = {RST, IMPL, NONE, NONE, ConditionNone, 0x00},
    [0xC8] = {RET, IMPL, NONE, NONE, ConditionZ},
    [0xC9] = {RET},
    [0xCA] = {JP, D16, NONE, NONE, ConditionZ},
    [0xCB] = {CB, D8},
    [0xCC] = {CALL, D16, NONE, NONE, ConditionZ},
    [0xCD] = {CALL, D16},
    [0xCE] = {ADC, REG_D8, RegA},
    [0xCF] = {RST, IMPL, NONE, NONE, ConditionNone, 0x08},

    [0xD0] = {RET, IMPL, NONE, NONE, ConditionNC},
    [0xD1] = {POP, REG, RegDE},
    [0xD2] = {JP, D16, NONE, NONE, ConditionNC},
    [0xD4] = {CALL, D16, NONE, NONE, ConditionNC},
    [0xD5] = {PUSH, REG, RegDE},
    [0xD6] = {SUB, REG_D8, RegA},
    [0xD7] = {RST, IMPL, NONE, NONE, ConditionNone, 0x10},
    [0xD8] = {RET, IMPL, NONE, NONE, ConditionC},
    [0xD9] = {RETI},
    [0xDA] = {JP, D16, NONE, NONE, ConditionC},
    [0xDC] = {CALL, D16, NONE, NONE, ConditionC},
    [0xDE] = {SBC, REG_D8, RegA},
    [0xDF] = {RST, IMPL, NONE, NONE, ConditionNone, 0x18},

    //0xEX
    [0xE0] = {LDH, A8_REG, NONE, RegA},
    [0xE1] = {POP, REG, RegHL},
    [0xE2] = {LD, MEMREG_REG, RegC, RegA},
    [0xE5] = {PUSH, REG, RegHL},
    [0xE6] = {AND, REG_D8, RegA},
    [0xE7] = {RST, IMPL, NONE, NONE, ConditionNone, 0x20},
    [0xE8] = {ADD, REG_D8, RegSP},
    [0xE9] = {JP, REG, RegHL},
    [0xEA] = {LD, A16_REG, NONE, RegA},
    [0xEE] = {XOR, REG_D8, RegA},
    [0xEF] = {RST, IMPL, NONE, NONE, ConditionNone, 0x28},


    //0xFX
    [0xF0] = {LDH, REG_A8, RegA},
    [0xF1] = {POP, REG, RegAF},
    [0xF2] = {LD, REG_MEMREG, RegA, RegC},
    [0xF3] = {DI},
    [0xF5] = {PUSH, REG, RegAF},
    [0xF6] = {OR, REG_D8, RegA},
    [0xF7] = {RST, IMPL, NONE, NONE, ConditionNone, 0x30},
    [0xF8] = {LD, HL_SPR, RegHL, RegSP},
    [0xF9] = {LD, REG_REG, RegSP, RegHL},
    [0xFA] = {LD, REG_A16, RegA},
    [0xFB] = {EI},
    [0xFE] = {CP, REG_D8, RegA},
    [0xFF] = {RST, IMPL, NONE, NONE, ConditionNone, 0x38},
};



instruction *instructionByOpCode(uint8_t opcode) {
    return &instructions[opcode];
}

char *instructionLookup[] = {
    "<NONE>",
    "NOP",
    "LD",
    "INC",
    "DEC",
    "RLCA",
    "ADD",
    "RRCA",
    "STOP",
    "RLA",
    "JR",
    "RRA",
    "DAA",
    "CPL",
    "SCF",
    "CCF",
    "HALT",
    "ADC",
    "SUB",
    "SBC",
    "AND",
    "XOR",
    "OR",
    "CP",
    "POP",
    "JP",
    "PUSH",
    "RET",
    "CB",
    "CALL",
    "RETI",
    "LDH",
    "JPHL",
    "DI",
    "EI",
    "RST",
    "IN_ERR",
    "IN_RLC",
    "IN_RRC",
    "IN_RL",
    "IN_RR",
    "IN_SLA",
    "IN_SRA",
    "IN_SWAP",
    "IN_SRL",
    "IN_BIT",
    "IN_RES",
    "IN_SET"
};

char *instructionName(instructionType type) {
    return instructionLookup[type];
}

static char *regLookup[] = {
    "<NONE>",
    "A",
    "F",
    "B",
    "C",
    "D",
    "E",
    "H",
    "L",
    "AF",
    "BC",
    "DE",
    "HL",
    "SP",
    "PC"
};

void instructionToString(CPUContext *ctx, char *str) {
    instruction *inst = ctx->currentInstruction;
    sprintf(str, "%s ", instructionName(inst->type));

    switch(inst->mode) {
        case IMPL:
            return;

        case REG_D16:
        case REG_A16:
            sprintf(str, "%s %s,$%04X", instructionName(inst->type),
                regLookup[inst->reg1], ctx->fetchedData);
            return;

        case REG:
            sprintf(str, "%s %s", instructionName(inst->type),
                regLookup[inst->reg1]);
            return;

        case REG_REG:
            sprintf(str, "%s %s,%s", instructionName(inst->type),
                regLookup[inst->reg1], regLookup[inst->reg2]);
            return;

        case MEMREG_REG:
            sprintf(str, "%s (%s),%s", instructionName(inst->type),
                regLookup[inst->reg1], regLookup[inst->reg2]);
            return;

        case MEMREG:
            sprintf(str, "%s (%s)", instructionName(inst->type),
                regLookup[inst->reg1]);
            return;

        case REG_MEMREG:
            sprintf(str, "%s %s,(%s)", instructionName(inst->type),
                regLookup[inst->reg1], regLookup[inst->reg2]);
            return;

        case REG_D8:
        case REG_A8:
            sprintf(str, "%s %s,$%02X", instructionName(inst->type),
                regLookup[inst->reg1], ctx->fetchedData & 0xFF);
            return;

        case REG_HLI:
            sprintf(str, "%s %s,(%s+)", instructionName(inst->type),
                regLookup[inst->reg1], regLookup[inst->reg2]);
            return;

        case REG_HLD:
            sprintf(str, "%s %s,(%s-)", instructionName(inst->type),
                regLookup[inst->reg1], regLookup[inst->reg2]);
            return;

        case HLI_REG:
            sprintf(str, "%s (%s+),%s", instructionName(inst->type),
                regLookup[inst->reg1], regLookup[inst->reg2]);
            return;

        case HLD_REG:
            sprintf(str, "%s (%s-),%s", instructionName(inst->type),
                regLookup[inst->reg1], regLookup[inst->reg2]);
            return;

        case A8_REG:
            sprintf(str, "%s $%02X,%s", instructionName(inst->type),
                busRead(ctx->regs.PC - 1), regLookup[inst->reg2]);

            return;

        case HL_SPR:
            sprintf(str, "%s (%s),SP+%d", instructionName(inst->type),
                regLookup[inst->reg1], ctx->fetchedData & 0xFF);
            return;

        case D8:
            sprintf(str, "%s $%02X", instructionName(inst->type),
                ctx->fetchedData & 0xFF);
            return;

        case D16:
            sprintf(str, "%s $%04X", instructionName(inst->type),
                ctx->fetchedData);
            return;

        case MEMREG_D8:
            sprintf(str, "%s (%s),$%02X", instructionName(inst->type),
                regLookup[inst->reg1], ctx->fetchedData & 0xFF);
            return;

        case A16_REG:
            sprintf(str, "%s ($%04X),%s", instructionName(inst->type),
                ctx->fetchedData, regLookup[inst->reg2]);
            return;

        default:
            fprintf(stderr, "INVALID AM: %d\n", inst->mode);
            NOIMPL
    }
}