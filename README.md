<a href="https://www.fontspace.com/category/pixel"><img src="https://see.fontimg.com/api/rf5/JpBgm/MDIxNjc2MDU1NTllNDQzYzhhNDU0ZWIyYWQxMGZmYWYudHRm/VGhlIEVwaWNCb3kgRW11bGF0b3I/raster-forge-regular.png?r=fs&h=30&w=1000&fg=801717&bg=FFFFFF&tb=1&s=30" alt="Pixel fonts"></a>

<pre>
The EpicBoy Emulator is a general GameBoy emulator written in Pure C, and was created 
as a way for me to learn Emulator Development (I should have just made a Chip8)

# Progress

Overall Progress (DMG)
[========  ] 87% (For better accuracy look at each section below)

CPU
[==========] 100% Unprefixed CPU OP-Codes
[==========] 100% Prefixed CPU OP-Codes

Memory/Bus
[==========] 100% Cartridge Read (ROM ONLY)
[==========] 100% Echo RAM (0xE000-0xFDFF)
[==========] 100% WRAM/HRAM Access
[==========] 100% Interrupt Register (IE/IF)
[==========] 100% Cartridge Write
[==========] 100% IO Registers (0xFF00-0xFF7F)
[===       ] 33%  Working IO Registers (0xFF00-0xFF7F)
[=         ] 12%  All Rom Types Support

DMG PPU/GPU
[==========] 100% Sprite Attribute Memory/OAM (0xFE00-0xFE9F)
[==========] 100% VRAM Access
[==========] 100% Tile Data (0x8000-0x97FF)
[==========] 100% BG Map 1 (0x9800-0x98FF)
[==========] 100% BG Map 2 (0x9C00-0x9FFF)
[==========] 100% Palette/Colours
[==========] 100% LCD Control/Status

Timers / Sound / Input
[==========] 100% Timer Registers (DIV, TIMA, TMA, TAC)
[==========] 100% Joypad Input
[========= ] 90%  Serial Support
[          ] 0%   Sound Registers/APU

Other Core / Future
[==========] 100% DMA Transfer
[==========] 100% Halt / Stop CPU states
[          ] 0%   GameBoy Boot ROM
[          ] 0%   Double-speed mode
[          ] 0%   RTC (MBC3 timer)

# Credits

- X_EpicDev (Everything)
</pre>