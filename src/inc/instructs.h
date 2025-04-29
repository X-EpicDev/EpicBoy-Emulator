#ifndef INSTRUCTS_H
#define INSTRUCTS_H
#include <stdint.h>

//Address Modes
typedef enum {
    IMPL,       //Implied Address Mode (Does Nothing)
    REG_D16,    //Loads 16Bit Immediate Value Into Register
    REG_REG,    //Loads Data From 1 Register Into Another
    MEMREG_REG, //Loads Data From A Register Into Memory
    REG,        //Used In Single Register Operations
    REG_D8,     //Loads 8Bit Immediate Value Into Register
    REG_MEMREG, //Loads Data From Memory Into A Register
    REG_HLI,    //Loads Data From Memory Address HL Is Pointing To Into A Register - Increments After
    REG_HLD,    //Loads Data From Memory Address HL Is Pointing To Into A Register - Decrements After
    HLI_REG,    //Loads Data From Register Into A Memory Address That HL Is Pointing To - Increments After
    HLD_REG,    //Loads Data From Register Into A Memory Address That HL Is Pointing To - Decrements After
    REG_A8,     //Loads An Immediate Memory Address Into A Register
    A8_REG,     //Loads A Register into an Immediate Memory Address
    HL_SPR,     //Uses HL As a Pointer To Sprite Memory
    D16,        //16Bit Immediate Value
    D8,         //8Bit Immediate Value
    D16_REG,    //Loads A Register Into A 16Bit Memory Address
    MEMREG_D8,  //Loads An Immediate Memory Address With 8Bit Data From A Register
    MEMREG,     //Memory Address
    A16_MEMREG, //Loads A Memory Address With An Immediate Memory Address
    REG_A16     //Loads Register With 16Bit Immediate Memory Address
} addressMode;

//Register Types
typedef enum {
    RegNone,//NULL Register
    RegA,   //Register A
    RegB,   //Register B
    RegC,   //Register C
    RegD,   //Register D
    RegE,   //Register E
    RegF,   //Register F
    RegH,   //Register H
    RegL,   //Register L
    RegPC,  //Program Counter
    RegSP   //Stack Pointer
} registerType;

//Instruction Types
typedef enum {
    NONE,   //Ignore - This Is Not An ASM Instruction
    NOP,    //Does Nothing - (1 CPU Cycle)
    LD,     //Loads Values In Memory And Registers (and Between) - (1-7 CPU Cycles)
    INC,    //Increments Register or Address By 1 - (1-6 CPU Cycles)
    DEC,    //Decrements Register or Address By 1 - (1-6 CPU Cycles)
    RLCA,   //Rotates Contents Of Register Left Through Carry Flag - (4 CPU Cycles)
    ADD,    //Adds The Contents Of Register To Another - (1-4 CPU Cycles)
    RRCA,   //Rotates Contents Of Register Right Through Carry Flag - (4 CPU Cycles)
    STOP,   //Halts CPU Execution Until Reboot (4 CPU Cycles)
    RLA,    //Rotates Contents Of Register Left Through Carry Flag, Affecting The Carry Flag - (4 CPU Cycles)
    JR,     //Jump To Specific Address - (3 CPU Cycles)
    RRA,    //Rotates Contents Of Register Right Through Carry Flag, Affecting The Carry Flag - (4 CPU Cycles)
    DAA,    //Adjusts Register After a BCD (Binary-Coded Decimal) Operation To Ensure The Result Is Valid - (4 CPU Cycles)
    CPL,    //Flips All Bits In Register - (4 CPU Cycles)
    SCF,    //Set Carry Flag To 1, Does Not Affect Any Other Flags - (4 CPU Cycles)
    CCF,    //Toggles Carry Flag - (4 CPU Cycles)
    HALT,   //Halts CPU Execution Until Reboot - (4 CPU Cycles)
    ADC,    //Adds Value + Carry Flag To Register - (1-4 CPU Cycles)
    SUB,    //Subtracts A Value From A Register - (1-4 CPU Cycles)
    SBC,    //Subtracts Value + Carry Flag To Register - (1-4 CPU Cycles)
    AND,    //Performs Bitwise AND Operation Between 2 Values - (1-4 CPU Cycles)
    XOR,    //Performs Bitwise XOR Operation Between 2 Values - (1-4 CPU Cycles)
    OR,     //Performs Bitwise OR Operation Between 2 Values - (1-4 CPU Cycles)
    CP,     //Compares 2 Register Values - (1-4 CPU Cycles)
    POP,    //Pops A 2 Byte Value From Stack Into 2 Registers - (3-5 CPU Cycles)
    JP,     //Jumps To Any Specified Address - (3-10 CPU Cycles)
    PUSH,   //Pushes 2 Byte Value From 2 Registers Onto The Stack - (4-6 CPU Cycles)
    RET,    //Return From Subroutine Than Pops Return Function From Stack - (3 CPU Cycles)
    CALL,   //Calls A Subroutine And Pushes Return Address Onto Stack, Then Jumps To Specified Address - (6 CPU Cycles)
    RETI,   //Returns Interrupt Service Routine And Enables Interrupts - (5 CPU Cycles)
    LDH,    //Loads Data Between Register And Memory Address Om High Memory Range (0xFF00-0xFFFF) - (3-4 CPU Cycles)
    JPHL,   //Jumps To Register HL - (4 CPU Cycles)
    DI,     //Disables Interrupts Preventing Them From Happening - (4 CPU Cycles)
    EI,     //Enables Interrupts Allowing Them To Occur - (4 CPU Cycles)
    RST,    //Jumps To Predefined Address And Pushes Return Address Onto Stack
    CB, //See Below For CB Instructions

    //CB OPCODE Instructions - All Instructions Take 4 CPU Cycles
    RLC,    //Rotates Register Left Through Carry Flag
    RRC,    //Rotates Register Right Through Carry Flag
    RL,     //Rotates Register Left Through Carry Flag Without Modifying It
    RR,     //Rotates Register Right Through Carry Flag Without Modifying It
    SLA,    //Shifts Register Left, Setting The Carry Flag
    SRA,    //Shifts Register Right, Setting The Carry Flag
    SWAP,   //Swaps High And Low Nibbles Of A Register
    SRL,    //Shifts Register Right, Filling High Bits With 0s
    BIT,    //Tests A Bit In A Register And Updates Any Flags Accordingly
    RES,    //Clears A Bit In A Register
    SET,    //Sets A Bit In A Register
} instructionType;

//Condition Types
typedef enum {
    ConditionNone,  //NO CONDITION
    ConditionNZ,    //IF ZERO FLAG  = FALSE
    ConditionZ,     //IF ZERO FLAG  = TRUE
    ConditionNC,    //IF CARRY FLAG = FALSE
    ConditionC      //IF CARRY FLAG = TRUE
} conditionType;

//Instruction Data
typedef struct {
    instructionType type;
    addressMode mode;
    registerType reg1;
    registerType reg2;
    conditionType condition;
    uint8_t param;
} instruction;

#endif //INSTRUCTS_H
