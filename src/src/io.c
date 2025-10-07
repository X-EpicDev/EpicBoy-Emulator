#include "../inc/io.h"
#include "../inc/timer.h"
#include "../inc/cpu.h"

static char SerialData[2];

uint8_t ioRead(uint16_t address) {
    if (address == 0xFF01) {
        return SerialData[0];
    }

    if (address == 0xFF02) {
        return SerialData[1];
    }

    if (BETWEEN(address, 0xFF04, 0xFF07)) {
        return timerRead(address);
    }

    if (address == 0xFF0F) {
        return cpuGetInterruptFlags();
    }

    printf("Unsupported Bus Read attempted at 0x%04X\n", address);
    return 0;
}

void ioWrite(uint16_t address, uint8_t value) {
    if (address == 0xFF01) {
        SerialData[0] = value;
        return;
    }

    if (address == 0xFF02) {
        SerialData[1] = value;
        return;
    }

    if (BETWEEN(address, 0xFF04, 0xFF07)) {
        timerWrite(address, value);
        return;
    }

    if (address == 0xFF0F) {
        cpuSetInterruptFlags(value);
        return;
    }

    printf("Unsupported Bus Write attempted at 0x%04X with value 0x%02X (NOT IMPLEMENTED)\n", address, value);
}