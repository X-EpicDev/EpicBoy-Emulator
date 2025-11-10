#include "../inc/ppu.h"
#include "../inc/lcd.h"
#include "../inc/ppuSM.h"
#include <string.h>

void pipelineFifoReset();
void pipelineProcess();

static ppuContext ctx;

ppuContext *ppuGetContext() {
    return &ctx;
}

void ppuInit() {
    ctx.currentFrame = 0;
    ctx.lineTicks = 0;
    ctx.videoBuffer = malloc(yResolution * xResolution * sizeof(32));

    //pipeline
    ctx.pfc.lineX = 0;
    ctx.pfc.pushedX = 0;
    ctx.pfc.fetchX = 0;
    ctx.pfc.pixelFifo.size = 0;
    ctx.pfc.pixelFifo.head = ctx.pfc.pixelFifo.tail = NULL;
    ctx.pfc.currentFetchState = FS_TILE;

    lcdInit();
    LCDS_MODE_SET(MODE_OAM);

    memset(ctx.oamRam, 0, sizeof(ctx.oamRam));
    memset(ctx.videoBuffer, 0, yResolution * xResolution * sizeof(uint32_t));
}

void ppuTick() {
    ctx.lineTicks++;

    switch (LCDS_MODE) {
        case MODE_OAM:
            ppuModeOAM();
            break;
        case MODE_XFER:
            ppuModeXFER();
            break;
        case MODE_VBLANK:
            ppuModeVBLANK();
            break;
        case MODE_HBLANK:
            ppuModeHBLANK();
            break;
    }
}

void ppuOamWrite(uint16_t address, uint8_t value) {
    if (address >= 0xFE00) {
        address -= 0xFE00;
    }

    uint8_t *p = (uint8_t *)ctx.oamRam;
    p[address] = value;
}

uint8_t ppuOamRead(uint16_t address) {
    if (address >= 0xFE00) {
        address -= 0xFE00;
    }

    uint8_t *p = (uint8_t *)ctx.oamRam;
    return p[address];
}

void ppuVramWrite(uint16_t address, uint8_t value) {
    ctx.vram[address - 0x8000] = value;
}

uint8_t ppuVramRead(uint16_t address) {
    return ctx.vram[address - 0x8000];
}