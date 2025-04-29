#include "../inc/bus.h""
#include "../inc/cart.h"

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

// uint8_t busRead(uint16_t address) {
//     if (address < 0x8000) {
//         return cartRead(address);
//     }
//     NOIMPL
// }

uint8_t busRead(uint16_t address) {
    if (address < 0x8000) {
        return cartRead(address);
    }
    //not implemented
}

void busWrite(uint16_t address, uint8_t value) {
    if (address < 0x8000) {
        //ROM DATA
        cartWrite(address, value);
        return;
    }
    //not implemented
}