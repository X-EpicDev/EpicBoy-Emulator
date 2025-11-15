#include "../inc/io.h"
#include "../inc/dma.h"
#include "../inc/lcd.h"
#include "../inc/timer.h"
#include "../inc/cpu.h"
#include "../inc/gamepad.h"

static char serial_data[2];

u8 io_read(u16 address) {
    if (address == 0xFF00) {
        //DMG Joypad Input
        return gamepad_get_output();
    }

    if (BETWEEN(address, 0xFF01, 0xFF02)) {
        //DMG Serial Transfer
        if (address == 0xFF01) return serial_data[0];
        if (address == 0xFF02) return serial_data[1];
    }

    if (BETWEEN(address, 0xFF04, 0xFF07)) {
        //DMG Timer and Divider
        return timer_read(address);
    }

    if (address == 0xFF0F) {
        //DMG Interrupts
        return cpu_get_int_flags();
    }

    if (BETWEEN(address, 0xFF10, 0xFF26)) {
        //DMG Audio - NOIMPL
        return 0;
    }

    if (BETWEEN(address, 0xFF10, 0xFF26)) {
        //DMG Wave Pattern - NOIMPL
        return 0;
    }

    if (BETWEEN(address, 0xFF40, 0xFF4B)) {
        //DMG LCD Control, Status, Position, Scrolling, and Palettes
        return lcd_read(address);
    }

    if (BETWEEN(address, 0xFF4C, 0xFF4D)) {
        //CGB KEY0 + KEY1 - NOIMPL
        return 0;
    }

    if (address == 0xFF4F) {
        //CGB VRAM Bank Select - NOIMPL
        return 0;
    }

    if (address == 0xFF50) {
        //DMG BOOT ROM Mapping Control - NOIMPL
        return 0;
    }

    if (BETWEEN(address, 0xFF51, 0xFF6B)) {
        //CGB VRAM DMA - NOIMPL
        return 0;
    }

    if (address == 0xFF56) {
        //CGB IR Port - NOIMPL
        return 0;
    }

    if (BETWEEN(address, 0xFF68, 0xFF6B)) {
        //CGB BG/OBJ Palettes - NOIMPL
        return 0;
    }

    if (address == 0xFF6C) {
        //CGB Object Priority Mode - NOIMPL
        return 0;
    }

    if (address == 0xFF70) {
        //CGB WRAM Bank Select - NOIMPL
        return 0;
    }

    printf("UNSUPPORTED bus_read(%04X)\n", address);
    return 0;
}

void io_write(u16 address, u8 value) {
    if (address == 0xFF00) {
        //DMG Joypad Input
        gamepad_set_sel(value);
        return;
    }

    if (BETWEEN(address, 0xFF01, 0xFF02)) {
        //DMG Serial Transfer
        if (address == 0xFF01) serial_data[0] = value;
        else if (address == 0xFF02) serial_data[1] = value;
        return;
    }

    if (BETWEEN(address, 0xFF04, 0xFF07)) {
        //DMG Timer and Divider
        timer_write(address, value);
        return;
    }

    if (address == 0xFF0F) {
        //DMG Interrupts
        cpu_set_int_flags(value);
        return;
    }

    if (BETWEEN(address, 0xFF10, 0xFF26)) {
        //DMG Audio - NOIMPL
        return;
    }

    if (BETWEEN(address, 0xFF30, 0xFF3F)) {
        //DMG Wave Pattern - NOIMPL
    }

    if (BETWEEN(address, 0xFF40, 0xFF4B)) {
        //DMG LCD Control, Status, Position, Scrolling, and Palettes
        lcd_write(address, value);
        return;
    }

    if (BETWEEN(address, 0xFF4C, 0xFF4D)) {
        //CGB KEY0 + KEY1 - NOIMPL
        return;
    }

    if (address == 0xFF4F) {
        //CGB VRAM Bank Select - NOIMPL
        return;
    }

    if (address == 0xFF50) {
        //DMG BOOT ROM Mapping Control - NOIMPL
        return;
    }

    if (BETWEEN(address, 0xFF51, 0xFF6B)) {
        //CGB VRAM DMA - NOIMPL
        return;
    }

    if (address == 0xFF56) {
        //CGB IR Port - NOIMPL
        return;
    }

    if (BETWEEN(address, 0xFF68, 0xFF6B)) {
        //CGB BG/OBJ Palettes - NOIMPL
        return;
    }

    if (address == 0xFF6C) {
        //CGB Object Priority Mode - NOIMPL
        return;
    }

    if (address == 0xFF70) {
        //CGB WRAM Bank Select - NOIMPL
        return;
    }

    printf("UNSUPPORTED bus_write(%04X)\n", address);
}
