#include "../inc/bus.h"
#include "../inc/cpu.h"
#include "../inc/emu.h"

extern CPUContext ctx;

//fetch current instruction
void fetchData() {
    ctx.memoryDestination = 0;
    ctx.destinationIsMemory = false;

    if (ctx.CurrentInstruction == NULL) {
        return;
    }

    //address modes switch
    switch(ctx.CurrentInstruction->mode) {

        case IMPL:
            return;

        case REG:
            ctx.fetchData = cpuReadReg(ctx.CurrentInstruction->reg1);
            return;

        case REG_REG:
            ctx.fetchData = cpuReadReg(ctx.CurrentInstruction->reg2);
            return;

        case REG_N8:
            ctx.fetchData = busRead(ctx.regs.PC);
            emuCycles(1);
            ctx.regs.PC++;
            return;

        case REG_E8: {
            uint8_t raw = busRead(ctx.regs.PC);
            emuCycles(1);

            ctx.fetchData = (int8_t)raw;

            ctx.regs.PC++;
            return;
        }

        case REG_N16: {
            uint16_t lo = busRead(ctx.regs.PC);
            emuCycles(1);

            uint16_t hi = busRead(ctx.regs.PC + 1);
            emuCycles(1);

            uint16_t value = lo | (hi << 8);
            ctx.regs.PC += 2;

            // Write directly into the target register pair
            cpuSetReg(ctx.CurrentInstruction->reg1, value);

            return;
        }

        case A16:
        case N16: {
            uint16_t lo = busRead(ctx.regs.PC);
            emuCycles(1);

            uint16_t hi = busRead(ctx.regs.PC + 1);
            emuCycles(1);

            ctx.fetchData = lo | (hi << 8);
            ctx.regs.PC += 2;

            return;
        }

        case E8: {
            uint8_t raw = busRead(ctx.regs.PC);
            emuCycles(1);

            ctx.fetchData = (int8_t)raw;
            ctx.regs.PC++;

            return;
        }

        case MEMREG_REG:
            ctx.fetchData = cpuReadReg(ctx.CurrentInstruction->reg2);
            ctx.memoryDestination = cpuReadReg(ctx.CurrentInstruction->reg1);
            ctx.destinationIsMemory = true;

            if (ctx.CurrentInstruction->reg1 == RegC) {
                ctx.memoryDestination |= 0xFF00;
            } return;

        case REG_MEMREG: {
            uint16_t address = cpuReadReg(ctx.CurrentInstruction->reg2);

            if (ctx.CurrentInstruction->reg2 == RegC) {
                address |= 0xFF00;
            }

            ctx.fetchData = busRead(address);
            emuCycles(1);
        } return;

        case REG_HLI:
            ctx.fetchData = busRead(cpuReadReg(RegHL));
            emuCycles(1);
            cpuSetReg(RegHL, cpuReadReg(RegHL) + 1);
            return;

        case REG_HLD:
            ctx.fetchData = busRead(cpuReadReg(RegHL));
            emuCycles(1);
            cpuSetReg(RegHL, cpuReadReg(RegHL) - 1);
            return;

        case HLI_REG:
            ctx.fetchData = cpuReadReg(ctx.CurrentInstruction->reg2);
            ctx.memoryDestination = cpuReadReg(ctx.CurrentInstruction->reg1);
            ctx.destinationIsMemory = true;
            cpuSetReg(RegHL, cpuReadReg(RegHL) + 1);
            return;

        case HLD_REG:
            ctx.fetchData = cpuReadReg(ctx.CurrentInstruction->reg2);
            ctx.memoryDestination = cpuReadReg(ctx.CurrentInstruction->reg1);
            ctx.destinationIsMemory = true;
            cpuSetReg(RegHL, cpuReadReg(RegHL) - 1);
            return;

        case REG_A8:
            ctx.fetchData = busRead(ctx.regs.PC);
            emuCycles(1);
            ctx.regs.PC++;
            return;

        case A8_REG:
            ctx.memoryDestination = busRead(ctx.regs.PC) | 0xFF00;
            ctx.destinationIsMemory = true;
            emuCycles(1);
            ctx.regs.PC++;
            return;

        case HL_SPR: {
            uint8_t raw = busRead(ctx.regs.PC);
            emuCycles(1);
            ctx.regs.PC++;

            // Store as signed offset for executor
            ctx.fetchData = (int8_t)raw;
            return;
        }

        case N8:
            ctx.fetchData = busRead(ctx.regs.PC);
            emuCycles(1);
            ctx.regs.PC++;
            return;

        case A16_REG: {
            uint16_t lo = busRead(ctx.regs.PC);
            emuCycles(1);

            uint16_t hi = busRead(ctx.regs.PC + 1);
            emuCycles(1);

            ctx.memoryDestination = lo | (hi << 8);
            ctx.destinationIsMemory = true;

            ctx.regs.PC += 2;

            ctx.fetchData = cpuReadReg(ctx.CurrentInstruction->reg2);
            return;
        }
        case N16_REG: {
            uint16_t lo = busRead(ctx.regs.PC);
            emuCycles(1);

            uint16_t hi = busRead(ctx.regs.PC + 1);
            emuCycles(1);

            uint16_t value = lo | (hi << 8);

            ctx.regs.PC += 2;

            cpuSetReg(ctx.CurrentInstruction->reg1, value);
        } return;

        case MEMREG_N8:
            ctx.fetchData = busRead(ctx.regs.PC);
            emuCycles(1);
            ctx.regs.PC++;
            ctx.memoryDestination = cpuReadReg(ctx.CurrentInstruction->reg1);
            ctx.destinationIsMemory = true;
            return;

        case MEMREG:
            ctx.memoryDestination = cpuReadReg(ctx.CurrentInstruction->reg1);
            ctx.destinationIsMemory = true;
            emuCycles(1);
            return;

        case REG_A16: {
            uint16_t lo = busRead(ctx.regs.PC);
            emuCycles(1);

            uint16_t hi = busRead(ctx.regs.PC + 1);
            emuCycles(1);

            uint16_t address = lo | (hi << 8);

            ctx.regs.PC += 2;
            ctx.fetchData = busRead(address);
            emuCycles(1);

            return;
        }

        default:
            printf("Unknown Addressing Mode %d (%02X)\n", ctx.CurrentInstruction->mode, ctx.currentOPCode);
            exit(-7);
    }
}