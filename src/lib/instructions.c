#include "../inc/instructs.h"
#include "../inc/cpu.h"

instruction instructions[0x100] = {
    [0x00] = {NOP, IMPL},
    [0x01] = {LD, REG_D16, RegBC},

    [0xC3] = {JP, D16},
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

char *instructionName(instructionType t) {
    return InstructionLookup[t];
}