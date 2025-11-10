#include "../inc/lcd.h"
#include "../inc/ppu.h"
#include "../inc/dma.h"

static lcdContext ctx;

static unsigned long coloursDefault[4] = {0xFFFFFFFF, 0xFFAAAAAA, 0xFF555555, 0xFF000000};

void lcdInit() {
    ctx.lcdc = 0x91;
    ctx.scrollX = 0;
    ctx.scrollY = 0;
    ctx.ly = 0;
    ctx.lyCompare = 0;
    ctx.bgPalette = 0xFC;
    ctx.objPalette[0] = 0xFF;
    ctx.objPalette[1] = 0xFF;
    ctx.winY = 0;
    ctx.winX = 0;


    for (int i=0; i<4; i++) {
        ctx.bgColours[i] = coloursDefault[i];
        ctx.sp1Colours[i] = coloursDefault[i];
        ctx.sp2Colours[i] = coloursDefault[i];
    }
}

lcdContext *lcdGetContext() {
    return &ctx;
}

uint8_t lcdRead(uint16_t address) {
    uint8_t offset = (address - 0xFF40);
    uint8_t *p = (uint8_t *)&ctx;

    return p[offset];
}

void updatePalette(uint8_t paletteData, uint8_t palette) {
    uint32_t *pColours = ctx.bgColours;

    switch(palette) {
        case 1:
            pColours = ctx.sp1Colours;
            break;
        case 2:
            pColours = ctx.sp2Colours;
            break;
    }

    pColours[0] = coloursDefault[paletteData & 0b11];
    pColours[1] = coloursDefault[(paletteData >> 2) & 0b11];
    pColours[2] = coloursDefault[(paletteData >> 4) & 0b11];
    pColours[3] = coloursDefault[(paletteData >> 6) & 0b11];
}

void lcdWrite(uint16_t address, uint8_t value) {
    uint8_t offset = (address - 0xFF40);
    uint8_t *p = (uint8_t *)&ctx;
    p[offset] = value;

    if (offset == 6) {
        dmaStart(value);
    }

    if (address == 0xFF47) {
        updatePalette(value, 0);
    } else if (address == 0xFF48) {
        updatePalette(value & 0b11111100, 1);
    } else if (address == 0xFF49) {
        updatePalette(value & 0b11111100, 1);
    }
}