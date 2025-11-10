#include "../inc/dma.h"
#include "../inc/ppu.h"
#include "../inc/bus.h"

typedef struct {
    bool active;
    uint8_t byte;
    uint8_t value;
    uint8_t startDelay;
} dmaContext;

static dmaContext ctx;

void dmaStart(uint8_t start) {
    ctx.active = true;
    ctx.byte = 0;
    ctx.startDelay = 2;
    ctx.value = start;
}

void dmaTick() {
    if (!ctx.active) {
        return;
    }

    if (ctx.startDelay) {
        ctx.startDelay--;
        return;
    }

    ppuOamWrite(ctx.byte, busRead((ctx.value * 0x100) + ctx.byte));

    ctx.byte++;

    ctx.active = ctx.byte < 0xA0;
}

bool dmaTransferring() {
    return ctx.active;
}