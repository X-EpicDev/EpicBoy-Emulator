#include "../inc/instructs.h"
#include "../inc/cpu.h"

instruction instructions[0x100] = {
    [0x00] = {NOP, IMPL},
    [0x01] = {LD, REG_N16, RegBC},
    [0x02] = {LD, MEMREG_REG, RegBC, RegA},
    //0x03
    //0x04
    [0x05] = {DEC, REG, RegB},
    [0x06] = {LD, REG_N8, RegB},
    //0x07
    [0x08] = {LD, A16_REG, NONE, RegSP},
    //0x09
    [0x0A] = {LD, REG_MEMREG, RegA, RegBC},
    //0x0B
    //0x0C
    //0x0D
    [0x0E] = {LD, REG_N8, RegC},
    //0x0F

    //0x10
    [0x11] = {LD, REG_N16, RegDE},
    [0x12] = {LD, MEMREG_REG, RegDE, RegA},
    //0x13
    //0x14
    [0x15] = {DEC, REG, RegD},
    [0x16] = {LD, REG_N8, RegD},
    //0x17
    [0x18] = {JR, E8},
    //0x19
    [0x1A] = {LD, REG_MEMREG, RegA, RegDE},
    //0x1B
    //0x1C
    //0x1D
    [0x1E] = {LD, REG_N8, RegE},
    //0x1F

    [0x20] = {JR, E8, NONE, NONE, ConditionNZ},
    [0x21] = {LD, REG_N16, RegHL},
    [0x22] = {LD, HLI_REG, RegHL, RegA},
    //0x23
    //0x24
    [0x25] = {DEC, REG, RegH},
    [0x26] = {LD, REG_N8, RegH},
    //0x27
    [0x28] = {JR, E8, NONE, NONE, ConditionZ},
    //0x29
    [0x2A] = {LD, REG_HLI, RegA, RegHL},
    //0x2B
    //0x2C
    //0x2D
    [0x2E] = {LD, REG_N8, RegL},
    //0x2F

    [0x30] = {JR, E8, NONE, NONE, ConditionNC},
    [0x31] = {LD, REG_N16, RegSP},
    [0x32] = {LD, HLD_REG, RegHL, RegA},
    //0x33
    //0x34
    [0x35] = {DEC, REG, RegHL},
    [0x36] = {LD, MEMREG_N8, RegHL},
    //0x37
    [0x38] = {JR, E8, NONE, NONE, ConditionC},
    //0x39
    [0x3A] = {LD, REG_HLD, RegA, RegHL},
    //0x3B
    //0x3C
    //0x3D
    [0x3E] = {LD, REG_N8, RegA},
    //0x3F

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

    //0x80
    //0x81
    //0x82
    //0x83
    //0x84
    //0x85
    //0x86
    //0x87
    //0x88
    //0x89
    //0x8A
    //0x8B
    //0x8C
    //0x8D
    //0x8E
    //0x8F

    //0x90
    //0x91
    //0x92
    //0x93
    //0x94
    //0x95
    //0x96
    //0x97
    //0x98
    //0x99
    //0x9A
    //0x9B
    //0x9C
    //0x9D
    //0x9E
    //0x9F

    //0xA0
    //0xA1
    //0xA2
    //0xA3
    //0xA4
    //0xA5
    //0xA6
    //0xA7
    [0xA8] = {XOR, REG_REG, RegA, RegB},
    [0xA9] = {XOR, REG_REG, RegA, RegC},
    [0xAA] = {XOR, REG_REG, RegA, RegD},
    [0xAB] = {XOR, REG_REG, RegA, RegE},
    [0xAC] = {XOR, REG_REG, RegA, RegH},
    [0xAD] = {XOR, REG_REG, RegA, RegL},
    [0xAE] = {XOR, REG_MEMREG, RegA, RegHL},
    [0xAF] = {XOR, REG_REG, RegA, RegA},

    //0xB0
    //0xB1
    //0xB2
    //0xB3
    //0xB4
    //0xB5
    //0xB6
    //0xB7
    //0xB8
    //0xB9
    //0xBA
    //0xBB
    //0xBC
    //0xBD
    //0xBE
    //0xBF

    [0xC0] = {RET, IMPL, NONE, NONE, ConditionNZ},
    [0xC1] = {POP, REG, RegBC},
    [0xC2] = {JP, A16, NONE, NONE, ConditionNZ},
    [0xC3] = {JP, A16},
    [0xC4] = {CALL, A16, NONE, NONE, ConditionNZ},
    [0xC5] = {PUSH, REG, RegBC},
    [0xC7] = {RST, IMPL, NONE, NONE, ConditionNone, 0x00},
    //0xC7
    [0xC8] = {RET, IMPL, NONE, NONE, ConditionZ},
    [0xC9] = {RET},
    [0xCA] = {JP, A16, NONE, NONE, ConditionZ},
    //0xCB
    [0xCC] = {CALL, A16, NONE, NONE, ConditionZ},
    [0xCD] = {CALL, A16},
    //0xCE
    [0xCF] = {RST, IMPL, NONE, NONE, ConditionNone, 0x08},

    [0xD0] = {RET, IMPL, NONE, NONE, ConditionNC},
    [0xD1] = {POP, REG, RegDE},
    [0xD2] = {JP, A16, NONE, NONE, ConditionNC},

    [0xD4] = {CALL, A16, NONE, NONE, ConditionNC},
    [0xD5] = {PUSH, REG, RegDE},
    //0xD6
    [0xD7] = {RST, IMPL, NONE, NONE, ConditionNone, 0x10},
    [0xD8] = {RET, IMPL, NONE, NONE, ConditionC},
    [0xD9] = {RETI},
    [0xDA] = {JP, A16, NONE, NONE, ConditionC},

    [0xDC] = {CALL, A16, NONE, NONE, ConditionZ},

    //0xDE
    [0xDF] = {RST, IMPL, NONE, NONE, ConditionNone, 0x18},

    [0xE0] = {LDH, A8_REG, NONE, RegA},
    [0xE1] = {POP, REG, RegHL},
    [0xE2] = {LDH, MEMREG_REG, RegC, RegA},

    [0xE5] = {PUSH, REG, RegHL},
    //0xE6
    [0xE7] = {RST, IMPL, NONE, NONE, ConditionNone, 0x20},
    //0xE8
    [0xE9] = {JP, REG, RegHL},
    [0xEA] = {LD, A16_REG, NONE, RegA},

    [0xEE] = {XOR, REG_N8, RegA},
    [0xEF] = {RST, IMPL, NONE, NONE, ConditionNone, 0x28},

    [0xF0] = {LDH, REG_A8, RegA, NONE},
    [0xF1] = {POP, REG, RegAF},
    [0xF2] = {LDH, REG_MEMREG, RegA, RegC},
    [0xF3] = {DI},
    [0xC5] = {PUSH, REG, RegAF},
    //0xF6
    [0xF7] = {RST, IMPL, NONE, NONE, ConditionNone, 0x30},
    //0xF8
    //0xF9
    [0xFA] = {LD, REG_A16, RegA},
    //0xFB

    //0xFE
    [0xFF] = {RST, IMPL, NONE, NONE, ConditionNone, 0x38},
};

instruction *instructionByOpCode(uint8_t opcode) {
    return &instructions[opcode];
}

char *InstructionLookup[] = {
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
    "CALL",
    "RETI",
    "LDH",
    "JPHL",
    "DI",
    "EI",
    "RST",
    "CB",
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

char *instructionName(instructionType t) {
    return InstructionLookup[t];
}