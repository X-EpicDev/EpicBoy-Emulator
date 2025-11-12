<a href="https://www.fontspace.com/category/pixel"><img src="https://see.fontimg.com/api/rf5/JpBgm/MDIxNjc2MDU1NTllNDQzYzhhNDU0ZWIyYWQxMGZmYWYudHRm/VGhlIEVwaWNCb3kgRW11bGF0b3I/raster-forge-regular.png?r=fs&h=30&w=1000&fg=801717&bg=FFFFFF&tb=1&s=30" alt="Pixel fonts"></a>

<pre>
The EpicBoy Emulator is a general GameBoy emulator written in Pure C, and was created 
as a way for me to learn Emulator Development

# Progress

Overall Progress
[======    ] 60% (For better accuracy look at each section below)

CPU
[==========] 100% Unprefixed CPU OP-Codes
[==========] 100% Prefixed CPU OP-Codes

Memory/Bus
[==========] 100% Cartridge Read (ROM ONLY)
[==========] 100% Echo RAM (0xE000-0xFDFF)
[==========] 100% WRAM/HRAM Access
[==========] 100% Interrupt Register (IE/IF)
[          ] 0%   Cartridge Write
[          ] 0%   IO Registers (0xFF00-0xFF7F)
[          ] 0%   MBC1/MBC2/MBC3/MBC5 Support

PPU/GPU
[==========] 100% Sprite Attribute Memory/OAM (0xFE00-0xFE9F)
[==========] 100% VRAM Access
[==========] 100% Tile Data (0x8000-0x97FF)
[==========] 100% BG Map 1 (0x9800-0x98FF)
[==========] 100% BG Map 2 (0x9C00-0x9FFF)
[          ] 0%   Palette/Colours
[          ] 0%   LCD Control/Status

Timers / Sound / Input
[==========] 100% Timer Registers (DIV, TIMA, TMA, TAC)
[=====     ] 50%  Serial / Link Cable Support
[          ] 0%   Joypad Input
[          ] 0%   Sound Registers

Other Core / Future
[==========] 100% DMA Transfer
[==========] 100% Halt / Stop CPU states
[          ] 0%   GameBoy Boot ROM
[          ] 0%   Double-speed mode / GBC support
[          ] 0%   RTC (MBC3 timer)

# Credits

- X_EpicDev (Everything)
</pre>