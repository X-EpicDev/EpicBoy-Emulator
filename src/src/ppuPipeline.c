#include "../inc/ppu.h"
#include "../inc/lcd.h"
#include "../inc/bus.h"

void pixelFifoPush(uint32_t value) {
    fifo_entry *next = malloc(sizeof(fifo_entry));
    next->next = NULL;
    next->value = value;

    if (!ppuGetContext()->pfc.pixelFifo.head) {
        //first entry
        ppuGetContext()->pfc.pixelFifo.head = ppuGetContext()->pfc.pixelFifo.tail = next;
    } else {
        ppuGetContext()->pfc.pixelFifo.tail->next = next;
        ppuGetContext()->pfc.pixelFifo.tail = next;
    }

    ppuGetContext()->pfc.pixelFifo.size++;
}

uint32_t pixelFifoPop() {
    if (ppuGetContext()->pfc.pixelFifo.size <= 0) {
        fprintf(stderr, "ERR IN PIXEL FIFO!\n");
        exit(-8);
    }

    fifo_entry *popped = ppuGetContext()->pfc.pixelFifo.head;
    ppuGetContext()->pfc.pixelFifo.head = popped->next;
    ppuGetContext()->pfc.pixelFifo.size--;

    uint32_t value = popped->value;
    free(popped);

    return value;
}

bool pipelineFifoAdd() {
    if (ppuGetContext()->pfc.pixelFifo.size > 8) {
        //FIFO IS FULL
        return false;
    }

    int x = ppuGetContext()->pfc.fetchX - (8 - (lcdGetContext()->scrollX % 8));

    for (int i = 0; i < 8; i++) {
        int bit = 7 - i;
        uint8_t hi = !!(ppuGetContext()->pfc.bgwFetchData[1] & (1 << bit));
        uint8_t lo = !!(ppuGetContext()->pfc.bgwFetchData[2] & (1 << bit)) << 1;
        uint32_t colour = lcdGetContext()->bgColours[hi | lo];

        if (x >= 0) {
            pixelFifoPush(colour);
            ppuGetContext()->pfc.fifoX++;
        }
    }

    return true;
}

void pipelineFetch() {
    switch (ppuGetContext()->pfc.currentFetchState) {
        case FS_TILE: {
            if (LCDC_BGW_ENABLE) {
                ppuGetContext()->pfc.bgwFetchData[0] = busRead(LCDC_BG_MAP_AREA +
                    (ppuGetContext()->pfc.mapX/8) +
                    (((ppuGetContext()->pfc.mapY/8)) * 32));

                if (LCDC_BGW_DATA_AREA == 0x8800) {
                    ppuGetContext()->pfc.bgwFetchData[0] += 128;
                }
            }

            ppuGetContext()->pfc.currentFetchState = FS_DATA0;
            ppuGetContext()->pfc.fetchX += 8;
        } break;

        case FS_DATA0: {
            ppuGetContext()->pfc.bgwFetchData[1] = busRead(LCDC_BGW_DATA_AREA +
                (ppuGetContext()->pfc.bgwFetchData[0] * 16) +
                ppuGetContext()->pfc.tileY);

            ppuGetContext()->pfc.currentFetchState = FS_DATA1;
        } break;

        case FS_DATA1: {
            ppuGetContext()->pfc.bgwFetchData[2] = busRead(LCDC_BGW_DATA_AREA +
                (ppuGetContext()->pfc.bgwFetchData[0] * 16) +
                ppuGetContext()->pfc.tileY + 1);

            ppuGetContext()->pfc.currentFetchState = FS_IDLE;

        } break;

        case FS_IDLE: {
            ppuGetContext()->pfc.currentFetchState = FS_PUSH;
        } break;

        case FS_PUSH: {
            if (pipelineFifoAdd()) {
                ppuGetContext()->pfc.currentFetchState = FS_TILE;
            }

        } break;

    }
}

void pipelinePushPixel() {
    if (ppuGetContext()->pfc.pixelFifo.size > 8) {
        uint32_t pixelData = pixelFifoPop();

        if (ppuGetContext()->pfc.lineX >= (lcdGetContext()->scrollX % 8)) {
            ppuGetContext()->videoBuffer[ppuGetContext()->pfc.pushedX +
                (lcdGetContext()->ly * xResolution)] = pixelData;

            ppuGetContext()->pfc.pushedX++;
        }

        ppuGetContext()->pfc.lineX++;
    }
}

void pipelineProcess() {
    ppuGetContext()->pfc.mapY = (lcdGetContext()->ly + lcdGetContext()->scrollY);
    ppuGetContext()->pfc.mapX = (ppuGetContext()->pfc.fetchX + lcdGetContext()->scrollX);
    ppuGetContext()->pfc.tileY = ((lcdGetContext()->ly + lcdGetContext()->scrollY) % 8) * 2;

    if (!(ppuGetContext()->lineTicks & 1)) {
        pipelineFetch();
    }

    pipelinePushPixel();
}

void pipelineFifoReset() {
    while (ppuGetContext()->pfc.pixelFifo.size) {
        pixelFifoPop();
    }

    ppuGetContext()->pfc.pixelFifo.head = 0;
}