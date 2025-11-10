#include "../inc/ppu.h"
#include "../inc/lcd.h"
#include "../inc/cpu.h"
#include "../inc/interrupts.h"

void incrementLY() {
    lcdGetContext()->ly++;

    if (lcdGetContext()->ly == lcdGetContext()->lyCompare) {
        LCDS_LYC_SET(1);

        if (LCDS_STAT_INT(SS_LYC)) {
            cpuRequestInterrupt(Interrupt_LCDSTAT);
        }
    } else {
        LCDS_LYC_SET(0);
    }
}

void ppuModeOAM() {
    if (ppuGetContext()->lineTicks >= 80) {
        LCDS_MODE_SET(MODE_XFER);

        ppuGetContext()->pfc.currentFetchState = FS_TILE;
        ppuGetContext()->pfc.lineX = 0;
        ppuGetContext()->pfc.fetchX = 0;
        ppuGetContext()->pfc.pushedX = 0;
        ppuGetContext()->pfc.fifoX = 0;
    }
}

void ppuModeXFER() {
    pipelineProcess();

    if (ppuGetContext()->pfc.pushedX >= xResolution) {
        pipelineFifoReset();

        LCDS_MODE_SET(MODE_HBLANK);

        if (LCDS_STAT_INT(SS_HBLANK)) {
            cpuRequestInterrupt(Interrupt_LCDSTAT);
        }
    }
}

void ppuModeVBLANK() {
    if (ppuGetContext()->lineTicks >= TicksPerLine) {
        incrementLY();

        if (lcdGetContext()->ly >= linesPerFrame) {
            LCDS_MODE_SET(MODE_OAM);
            lcdGetContext()->ly = 0;
        }

        ppuGetContext()->lineTicks = 0;
    }
}

static uint32_t targetFrameTime = 1000/60;
static long previousFrameTime = 0;
static long startTimer = 0;
static long frameCount = 0;

void ppuModeHBLANK() {
    if (ppuGetContext()->lineTicks >= TicksPerLine) {
        incrementLY();

        if (lcdGetContext()->ly >= yResolution) {
            LCDS_MODE_SET(MODE_VBLANK);

            cpuRequestInterrupt(Interrupt_VBLANK);

            if (LCDS_STAT_INT(SS_VBLANK)) {
                cpuRequestInterrupt(Interrupt_LCDSTAT);
            }

            ppuGetContext()->currentFrame++;

            //calculate FPS
            uint32_t end = getTicks();
            uint32_t frameTime = end - previousFrameTime;

            if (frameTime < targetFrameTime) {
                delay((targetFrameTime - frameTime));
            }

            if (end - startTimer >= 1000) {
                uint32_t fps = frameCount;
                startTimer = end;
                frameCount = 0;

                printf("FPS: %d\n", fps);
            }

            frameCount++;
            previousFrameTime = getTicks();

        } else {
            LCDS_MODE_SET(MODE_OAM);
        }

        ppuGetContext()->lineTicks = 0;
    }
}