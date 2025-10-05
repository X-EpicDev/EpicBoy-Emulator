#include "../inc/bus.h"
#include "../inc/cart.h"
#include "../inc/ram.h"
#include "../inc/cpu.h"

// MEMORY MAP
// 0x0000 - 0x3FFF : ROM BANK 0
// 0x4000 - 0x7FFF : ROM BANK 1
// 0x8000 - 0x97FF ; CHARACTER RAM
// 0x9800 - 0x9BFF : BG MAP 1
// 0x9C00 - 0x9FFF : BG MAP 2
// 0xA000 - 0xBFFF : CARTRIDGE RAM
// 0xC000 - 0xCFFF : RAM BANK 0
// 0xD000 - 0xDFFF : RAM BANK 1-7 (SWITCHABLE BUT ONLY USED BY GBC GAMES)
// 0xE000 - 0xFDFF : RESERVED (ECHO RAM)
// 0xFE00 - 0xFE9F : OBJECT ATTR MEMORY
// 0xFEA0 - 0xFEFF ; RESERVED (UNUSABLE)
// 0xFF00 - 0xFF7F : IO REGISTERS
// 0xFF80 - 0xFFFE : ZERO PAGE

uint8_t busRead(uint16_t address) {
    if (address < 0x8000) {
        //ROM DATA
        return cartRead(address);
    } else if (address < 0xA000) {
        //CHARACTER/MAP DATA
        //NO IMPLEMENTATION
        printf("Bus Read attempted at 0x%04X (NOT IMPLEMENTED)\n", address);
    } else if (address < 0xC000) {
        //CART RAM
        return cartRead(address);
    } else if (address < 0xE000) {
        //WORKING RAM
        return wramRead(address);
    } else if (address < 0xFE00) {
        //ECHO RAM (Copies 0xC000-0xDDFF)
        return wramRead(address - 0x2000);
    } else if (address < 0xFEA0) {
        //OAM
        //NO IMPLEMENTATION
        printf("Bus Read attempted at 0x%04X (NOT IMPLEMENTED)\n", address);
        return 0x0;
    } else if (address < 0xFF00) {
        //SYSTEM RESERVED
        return 0;
    } else if (address < 0xFF80) {
        //IO REGISTERS
        //NO IMPLEMENTATION
        printf("Bus Read attempted at 0x%04X (NOT IMPLEMENTED)\n", address);
        return 0x0;
    } else if (address == 0xFFFF) {
        //INTERRUPT ENABLE REGISTER
        //NO IMPLEMENTATION
        return cpuGetInterruptReg();
    }
    //NO IMPLEMENTATION
    return hramRead(address);
}

void busWrite(uint16_t address, uint8_t value) {
    if (address < 0x8000) {
        //ROM Data
        printf("Bus Write to ROM region at 0x%04X (SP=%04X)\n", address, cpuGetRegisters()->SP);
        cartWrite(address, value);
    } else if (address < 0xA000) {
        //Char/Map Data
        //NO IMPLEMENTATION
        printf("Bus Write attempted at 0x%04X with value 0x%02X (NOT IMPLEMENTED)\n", address, value);
    } else if (address < 0xC000) {
        //CART-RAM
        cartWrite(address, value);
    } else if (address < 0xE000) {
        //WRAM
        wramWrite(address, value);
    } else if (address < 0xFE00) {
        //ECHO RAM (mirror of 0xC000-0xDDFF)
        wramWrite(address - 0x2000, value);
    } else if (address < 0xFEA0) {
        //OAM
        //NO IMPLEMENTATION
        printf("Bus Write attempted at 0x%04X with value 0x%02X (NOT IMPLEMENTED)\n", address, value);
    } else if (address < 0xFF00) {
        //SYSTEM RESERVED
    } else if (address < 0xFF80) {
        //IO Registers
        //NO IMPLEMENTATION
        printf("Bus Write attempted at 0x%04X with value 0x%02X (NOT IMPLEMENTED)\n", address, value);
    } else if (address == 0xFFFF) {
        // INTERRUPT ENABLE REGISTER
        cpuSetInterruptRegister(value);
    } else {
        hramWrite(address, value);
    }
}

uint16_t busRead16(uint16_t address) {
    uint16_t lo = busRead(address);
    uint16_t hi = busRead(address + 1);
    return lo | hi << 8;
}

void busWrite16(uint16_t address, uint16_t value) {
    busWrite(address, value & 0xFF);
    busWrite(address + 1, (value >> 8) & 0xFF);
}