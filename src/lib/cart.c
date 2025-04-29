#include "../include/cart.h"

typedef struct {
    char filename [1024];
    uint32_t romSize;
    uint8_t *romData;
    rom_header *header;
} cartContext;

static cartContext ctx;

//will add the remaining rom types soon
static const char *RomTypes[] = {
    "ROM ONLY"
};

//will add the remaining license codes soon
static const char *LicCode[0xA5] = {
    [0x00] = "None"
};

const char *cartTypeName() {
    if (ctx.header->type <= 0x22) {
        return RomTypes[ctx.header->type];
    }
    return "UNKNOWN";
}

const char *cartLicName() {
    if(ctx.header->newLicCode <= 0xA4) {
        return LicCode[ctx.header->licCode];
    }
    return "UNKNOWN";
}

bool cartLoad(char *cart) {
    snprintf(ctx.filename, sizeof(ctx.filename), "%s", cart);

    FILE *fp = fopen(ctx.filename, "r");

    if(!fp) {
        printf("failed to open %s\n", cart);
        return false;
    }
    printf("Opened %s\n", ctx.filename);

    //get rom size and than return to start
    fseek(fp, 0, SEEK_END);
    ctx.romSize = ftell(fp);
    rewind(fp);

    //allocate the rom size into ram
    ctx.romData = malloc(ctx.romSize);
    fread(ctx.romData, ctx.romSize, 1, fp);
    fclose(fp);

    ctx.header = (rom_header *)(ctx.romData + 0x100);
    ctx.header->title[15] = 0;

    //print cartridge data
    printf("Loaded Cartridge: \n");
    printf("\t Title    : %s\n", ctx.header->title);
    printf("\t Type     : %2.2X (%s)\n", ctx.header->type, cartTypeName());
    printf("\t ROM Size : %d KB\n", 32 << ctx.header->romSize);
    printf("\t RAM Size : %2.2X\n", ctx.header->ramSize);
    printf("\t LIC Code : %2.2X (%s)\n", ctx.header->licCode, cartLicName());
    printf("\t ROM Vers : %2.2X\n", ctx.header->version);

    //checksum
    uint16_t x = 0;
    for (uint16_t i = 0x0134; i <= 0x014C; i++) {
        x = x - ctx.romData[i] - i;
    }

    printf("\t Checksum : %2.2X (%s)\n", ctx.header->checksum, (x & 0xFF) ? "PASSED" : "FAILED");

    return true;
}