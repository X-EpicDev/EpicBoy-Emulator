#include "../inc/ram.h"

typedef struct {
    uint8_t wram[0x2000];
    uint8_t hram[0x80];
} ramContext;

static ramContext ram;

uint8_t wramRead(uint16_t address) {
    address -= 0xC000;

    if (address >= 0x2000) {
        printf("INVALID WRAM ADDRESS: %08X\n", address + 0xC000);
        exit(-1);
    }

    return ram.wram[address];
}

void wramWrite(uint16_t address, uint8_t data) {
    address -= 0xC000;

    ram.wram[address] = data;
}

uint8_t hramRead(uint16_t address) {
    address -= 0xFF80;

    return ram.hram[address];
}

void hramWrite(uint16_t address, uint8_t data) {
    address -= 0xFF80;

    ram.hram[address] = data;
}