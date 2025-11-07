#include "../inc/ppu.h"

static ppuContext ctx;

void ppuInit() {

}

void ppuTick() {

}

void ppuOamWrite(uint16_t address, uint8_t value) {
    if (address >= 0xFE00) {
        address -= 0xFE00;
    }

    uint8_t *p = (uint8_t *)ctx.oam_ram;
    p[address] = value;
}

uint8_t ppuOamRead(uint16_t address) {
    if (address >= 0xFE00) {
        address -= 0xFE00;
    }

    uint8_t *p = (uint8_t *)ctx.oam_ram;
    return p[address];
}

void ppuVramWrite(uint16_t address, uint8_t value) {
    ctx.vram[address - 0x8000] = value;
}

uint8_t ppuVramRead(uint16_t address) {
    return ctx.vram[address - 0x8000];
}