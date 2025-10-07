#ifndef PPU_H
#define PPU_H

#include "common.h"

static const int16_t linesPerFrame = 154;
static const int16_t TicksPerLine = 456;
static const int16_t yResolution = 144;
static const int16_t xResolution = 160;

typedef enum {
    Tile,
    DATA0,
    DATA1,
    IDLE,
    PUSH,
} fetchState;

typedef struct fifo_entry {
    struct fifo_entry *next;
    uint32_t value; //32-Bit Color Value
} fifo_entry;

typedef struct {
    fifo_entry *head;
    fifo_entry *tail;
    uint32_t size;
} fifo;

typedef struct {
    fetchState currentFetchState;
    fifo pixelFifo;
    uint8_t lineX;
    uint8_t pushedX;
    uint8_t fetchX;
    uint8_t bgwFetchData[3];
    uint8_t fetchEntryData[6]; //oam data..
    uint8_t mapY;
    uint8_t mapX;
    uint8_t tileY;
    uint8_t fifoX;
} pixelFifoContext;

typedef struct {
    uint8_t y;
    uint8_t x;
    uint8_t tile;

    uint8_t f_cgb_pn : 3;
    uint8_t f_cgb_vram_bank : 1;
    uint8_t f_pn : 1;
    uint8_t f_x_flip : 1;
    uint8_t f_y_flip : 1;
    uint8_t f_bgp : 1;
} oamEntry;

 // Bit7   BG and Window over OBJ (0=No, 1=BG and Window colors 1-3 over the OBJ)
 // Bit6   Y flip          (0=Normal, 1=Vertically mirrored)
 // Bit5   X flip          (0=Normal, 1=Horizontally mirrored)
 // Bit4   Palette number  **Non CGB Mode Only** (0=OBP0, 1=OBP1)
 // Bit3   Tile VRAM-Bank  **CGB Mode Only**     (0=Bank 0, 1=Bank 1)
 // Bit2-0 Palette number  **CGB Mode Only**     (OBP0-7)

typedef struct oamLineEntry {
    oamEntry entry;
    struct oamLineEntry *next;
} oamLineEntry;

typedef struct {
    oamEntry oam_ram[40];
    uint8_t vram[0x2000];

    pixelFifoContext pfc;

    uint8_t line_sprite_count; //0 to 10 sprites.
    oamLineEntry *line_sprites; //linked list of current sprites on line.
    oamLineEntry line_entry_array[10]; //memory to use for list.

    uint8_t fetched_entry_count;
    oamEntry fetched_entries[3]; //entries fetched during pipeline.
    uint8_t window_line;

    uint32_t current_frame;
    uint32_t line_ticks;
    uint32_t *video_buffer;
} ppuContext;

void ppuInit();
void ppuTick();

void ppuOamWrite(uint16_t address, uint8_t value);
uint8_t ppuOamRead(uint16_t address);

void ppuVramWrite(uint16_t address, uint8_t value);
uint8_t ppuVramRead(uint16_t address);

ppuContext *ppuGetContext();

#endif //PPU_H
