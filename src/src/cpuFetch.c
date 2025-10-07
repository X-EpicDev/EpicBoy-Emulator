#include "../inc/bus.h"
#include "../inc/cpu.h"
#include "../inc/emu.h"

extern CPUContext ctx;

void fetchData() {
    ctx.memoryDestination = 0;
    ctx.destinationIsMemory = false;

    if (ctx.currentInstruction == NULL) {
        return;
    }

    switch(ctx.currentInstruction->mode) {
        case IMPL: return;

        case REG:
            ctx.fetchedData = cpuReadReg(ctx.currentInstruction->reg1);
            return;

        case REG_REG:
            ctx.fetchedData = cpuReadReg(ctx.currentInstruction->reg2);
            return;

        case REG_D8:
            ctx.fetchedData = busRead(ctx.regs.PC);
            emuCycles(1);
            ctx.regs.PC++;
            return;

        case REG_D16:
        case D16: {
            uint16_t lo = busRead(ctx.regs.PC);
            emuCycles(1);

            uint16_t hi = busRead(ctx.regs.PC + 1);
            emuCycles(1);

            ctx.fetchedData = lo | (hi << 8);

            ctx.regs.PC += 2;

            return;
        }

        case MEMREG_REG:
            ctx.fetchedData = cpuReadReg(ctx.currentInstruction->reg2);
            ctx.memoryDestination = cpuReadReg(ctx.currentInstruction->reg1);
            ctx.destinationIsMemory = true;

            if (ctx.currentInstruction->reg1 == RegC) {
                ctx.memoryDestination |= 0xFF00;
            }

            return;

        case REG_MEMREG: {
            uint16_t address = cpuReadReg(ctx.currentInstruction->reg2);

            if (ctx.currentInstruction->reg2 == RegC) {
                address |= 0xFF00;
            }

            ctx.fetchedData = busRead(address);
            emuCycles(1);

        } return;

        case REG_HLI:
            ctx.fetchedData = busRead(cpuReadReg(ctx.currentInstruction->reg2));
            emuCycles(1);
            cpuSetReg(RegHL, cpuReadReg(RegHL) + 1);
            return;

        case REG_HLD:
            ctx.fetchedData = busRead(cpuReadReg(ctx.currentInstruction->reg2));
            emuCycles(1);
            cpuSetReg(RegHL, cpuReadReg(RegHL) - 1);
            return;

        case HLI_REG:
            ctx.fetchedData = cpuReadReg(ctx.currentInstruction->reg2);
            ctx.memoryDestination = cpuReadReg(ctx.currentInstruction->reg1);
            ctx.destinationIsMemory = true;
            cpuSetReg(RegHL, cpuReadReg(RegHL) + 1);
            return;

        case HLD_REG:
            ctx.fetchedData = cpuReadReg(ctx.currentInstruction->reg2);
            ctx.memoryDestination = cpuReadReg(ctx.currentInstruction->reg1);
            ctx.destinationIsMemory = true;
            cpuSetReg(RegHL, cpuReadReg(RegHL) - 1);
            return;

        case REG_A8:
            ctx.fetchedData = busRead(ctx.regs.PC);
            emuCycles(1);
            ctx.regs.PC++;
            return;

        case A8_REG:
            ctx.memoryDestination = busRead(ctx.regs.PC) | 0xFF00;
            ctx.destinationIsMemory = true;
            emuCycles(1);
            ctx.regs.PC++;
            return;

        case HL_SPR:
            ctx.fetchedData = busRead(ctx.regs.PC);
            emuCycles(1);
            ctx.regs.PC++;
            return;

        case D8:
            ctx.fetchedData = busRead(ctx.regs.PC);
            emuCycles(1);
            ctx.regs.PC++;
            return;

        case A16_REG:
        case D16_REG: {
            uint16_t lo = busRead(ctx.regs.PC);
            emuCycles(1);

            uint16_t hi = busRead(ctx.regs.PC + 1);
            emuCycles(1);

            ctx.memoryDestination = lo | (hi << 8);
            ctx.destinationIsMemory = true;

            ctx.regs.PC += 2;
            ctx.fetchedData = cpuReadReg(ctx.currentInstruction->reg2);

        } return;

        case MEMREG_D8:
            ctx.fetchedData = busRead(ctx.regs.PC);
            emuCycles(1);
            ctx.regs.PC++;
            ctx.memoryDestination = cpuReadReg(ctx.currentInstruction->reg1);
            ctx.destinationIsMemory = true;
            return;

        case MEMREG:
            ctx.memoryDestination = cpuReadReg(ctx.currentInstruction->reg1);
            ctx.destinationIsMemory = true;
            ctx.fetchedData = busRead(cpuReadReg(ctx.currentInstruction->reg1));
            emuCycles(1);
            return;

        case REG_A16: {
            uint16_t lo = busRead(ctx.regs.PC);
            emuCycles(1);

            uint16_t hi = busRead(ctx.regs.PC + 1);
            emuCycles(1);

            uint16_t address = lo | (hi << 8);

            ctx.regs.PC += 2;
            ctx.fetchedData = busRead(address);
            emuCycles(1);

            return;
        }

        default:
            printf("Unknown Addressing Mode %d (%02X)\n", ctx.currentInstruction->mode, ctx.currentOPCode);
            exit(-7);
            return;
    }
}