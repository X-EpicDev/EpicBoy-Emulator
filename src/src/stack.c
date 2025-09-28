#include "../inc/stack.h"
#include "../inc/bus.h"
#include "../inc/cpu.h"

void stackPush(uint8_t data) {
    cpuGetRegisters()->SP--;
    busWrite(cpuGetRegisters()->SP, data);
}

void stackPush16(uint16_t data) {
    stackPush((data >> 8) & 0xFF);
    stackPush(data & 0xFF);
}

uint8_t stackPop() {
    uint8_t value = busRead(cpuGetRegisters()->SP);
    cpuGetRegisters()->SP++;
    return value;
}

uint16_t stackPop16() {
    uint16_t lo = stackPop();
    uint16_t hi = stackPop();

    return (hi << 8) | lo;
}