#include "../inc/instructs.h"
#include "../inc/cpu.h"

instruction instructions[0x100] = {
    [0x00] = {NOP, IMPL},
};

instruction *instructionByOpCode(uint8_t opcode) {
    if (instructions[opcode].type = NONE) {
        return NULL;
    }

    return &instructions[opcode];
}