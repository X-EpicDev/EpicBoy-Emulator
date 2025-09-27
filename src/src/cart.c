#include "../inc/cart.h"

typedef struct {
    char filename [1024];
    uint32_t romSize;
    uint8_t *romData;
    rom_header *header;
} cartContext;

static cartContext ctx;

static const char *RomTypes[] = {
    "ROM ONLY",
    "MBC1",
    "MBC1 + RAM",
    "MBC1 + RAM + BATTERY",
    "0x04 INVALID",
    "MBC2",
    "MBC2 + BATTERY",
    "0x07 INVALID",
    "ROM + RAM 1",
    "ROM + RAM + BATTERY 1",
    "0x0A INVALID",
    "MMM01",
    "MMM01 + RAM",
    "MMM01 + RAM + BATTERY",
    "0x0E INVALID",
    "MBC3 + TIMER + BATTERY",
    "MBC3 + TIMER + RAM + BATTERY 2",
    "MBC3",
    "MBC3 + RAM 2",
    "MBC3 + RAM + BATTERY 2",
    "0x14 INVALID",
    "0x15 INVALID",
    "0x16 INVALID",
    "0x17 INVALID",
    "0x18 INVALID",
    "MBC5",
    "MBC5 + RAM",
    "MBC5 + RAM + BATTERY",
    "MBC5 + RUMBLE",
    "MBC5 + RUMBLE + RAM",
    "MBC5 + RUMBLE + RAM + BATTERY",
    "0x1F INVALID",
    "MBC6",
    "0x21 INVALID",
    "MBC7 + SENSOR + RUMBLE + RAM + BATTERY"
};

//will add the remaining license codes soon
static const char *LicCode[0x100] = {
	[0x00] = "NONE",
	[0x01] = "NINTENDO RnD",
	[0x08] = "CAPCOM",
	[0x09] = "HOT-B",
	[0x0A] = "JALECO",
	[0x0B] = "COCONUTS",
	[0x0C] = "ELITE SYSTEMS",
	[0x13] = "ELECTRONIC ARTS",
	[0x18] = "HUDSONSOFT",
	[0x19] = "ITC ENTERTAINMENT",
	[0x1A] = "YANOMAN",
	[0x1D] = "CLARY",
	[0x1F] = "VIRGIN",
	[0x20] = "KSS",
	[0x24] = "PCM COMPLETE",
	[0x25] = "SAN-X",
	[0x28] = "KOTOBUKI SYSTEMS",
	[0x29] = "SETA",
	[0x30] = "INFOGRAMES",
	[0x31] = "NINTENDO",
	[0x32] = "BANDAI",
	[0x33] = "GBC GAME (IGNORE FOR NOW)",
	[0x34] = "KONAMI",
	[0x35] = "HECTOR",
	[0x38] = "CAPCOM",
	[0x39] = "BANPRESTO",
	[0x3C] = "*ENTERTAINMENT I",
	[0x3E] = "GREMLIN",
	[0x41] = "UBISOFT",
	[0x42] = "ATLUS",
	[0x44] = "MALIBU",
	[0x46] = "ANGEL",
	[0x47] = "SPECTRUM HOLOBY",
	[0x49] = "IREM",
	[0x4A] = "VIRGIN",
	[0x4D] = "MALIBU",
	[0x4F] = "U.S. GOLD",
	[0x50] = "ABSOLUTE",
	[0x51] = "ACCLAIM",
	[0x52] = "ACTIVISION",
	[0x53] = "AMERICAN SAMMY",
	[0x54] = "GAMETEK",
	[0x55] = "PARK PLACE",
	[0x56] = "LJN",
	[0x57] = "MATCHBOX",
	[0x59] = "MILTON BRADLEY",
	[0x5A] = "MINDSCAPE",
	[0x5B] = "ROMSTAR",
	[0x5C] = "NAXAT SOFT",
	[0x5D] = "TRADEWEST",
	[0x60] = "TITUS",
	[0x61] = "VIRGIN",
	[0x67] = "OCEAN",
	[0x69] = "ELECTRONIC ARTS",
	[0x6E] = "ELITE SYSTEMS",
	[0x6F] = "ELECTRO BRAIN",
	[0x70] = "INFOGRAMMES",
	[0x71] = "INTERPLAY",
	[0x72] = "BRODERBUND",
	[0x73] = "SCULPTERED SOFT",
	[0x75] = "THE SALES CURVE",
	[0x78] = "T*HQ",
	[0x79] = "ACCOLADE",
	[0x7A] = "TRIFFIX ENTERTAINMENT",
	[0x7C] = "MICROPROSE",
	[0x7F] = "KEMCO",
	[0x80] = "MISAWA ENTERTAINMENT",
	[0x83] = "LOZC",
	[0x86] = "TOKUMA SHOTEN INTERMEDIA",
	[0x8B] = "BULLET-PROOF SOFTWARE",
	[0x8C] = "VIC TOKAI",
	[0x8E] = "APE",
	[0x8F] = "I'MAX",
	[0x91] = "CHUN SOFT",
	[0x92] = "VIDEO SYSTEM",
	[0x93] = "TSUBURAVA",
	[0x95] = "VARIE",
	[0x96] = "YONEZAWA/S'PAL",
	[0x97] = "KANEKO",
	[0x99] = "ARC",
	[0x9A] = "NIHON BUSSAN",
	[0x9B] = "TECMO",
	[0x9C] = "IMAGINEER",
	[0x9D] = "BANPRESTO",
	[0x9F] = "NOVA",
	[0xA1] = "HORI ELECTRIC",
	[0xA2] = "BANDAI",
	[0xA4] = "KONAMI",
	[0xA6] = "KAWADA",
	[0xA7] = "TAKARA",
	[0xA9] = "TECHNOS JAPAN",
	[0xAA] = "BRODERBUND",
	[0xAC] = "TOEI ANIMATION",
	[0xAD] = "TOHO",
	[0xAF] = "NAMCO",
	[0xB0] = "ACCLAIM",
	[0xB1] = "ASCII OR NEXOFT",
	[0xB2] = "BANDAI",
	[0xB4] = "ENIX",
	[0xB6] = "HAL",
	[0xB7] = "SNK",
	[0xB9] = "PONY CANYON",
	[0xBA] = "*CULTURE BRAIN O",
	[0xBB] = "SUNSOFT",
	[0xBD] = "SONY IMAGESOFT",
	[0xBF] = "SAMMY",
	[0xC0] = "TAITO",
	[0xC2] = "KEMCO",
	[0xC3] = "SQUARESOFT",
	[0xC4] = "TOKUMA SHOTEN INTERMEDIA",
	[0xC5] = "DATA EAST",
	[0xC6] = "TONKIN HOUSE",
	[0xC8] = "KOEI",
	[0xC9] = "UFL",
	[0xCA] = "ULTRA",
	[0xCB] = "VAP",
	[0xCC] = "USE",
	[0xCD] = "MELDAC",
	[0xCE] = "*PONY CANYON OR",
	[0xCF] = "ANGEL",
	[0xD0] = "TAITO",
	[0xD1] = "SOFEL",
	[0xD2] = "QUEST",
	[0xD3] = "SIGMA ENTERPRISES",
	[0xD4] = "ASK KODANSHA",
	[0xD6] = "NAXAT SOFT",
	[0xD7] = "COPYA SYSTEMS",
	[0xD9] = "BANPRESTO",
	[0xDA] = "TOMY",
	[0xDB] = "LJN",
	[0xDD] = "NCS",
	[0xDE] = "HUMAN",
	[0xDF] = "ALTRON",
	[0xE0] = "JALECO",
	[0xE1] = "TOWACHIKI",
	[0xE2] = "UUTAKA",
	[0xE3] = "VARIE",
	[0xE5] = "EPOCH",
	[0xE7] = "ATHENA",
	[0xE8] = "ASMIK",
	[0xE9] = "NATSUME",
	[0xEA] = "KING RECORDS",
	[0xEB] = "ATLUS",
	[0xEC] = "EPIC/SONY RECORDS",
	[0xEE] = "IGS",
	[0xF0] = "A WAVE",
	[0xF3] = "EXTREME ENTERTAINMENT",
	[0xFF] = "LJN",
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

	//Set ctx.header to the header data
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

uint8_t cartRead(uint16_t address) {
	//ROM ONLY SUPPORTED TYPE AS OF RIGHT NOW

	return ctx.romData[address];
}

void cartWrite(uint16_t address, uint8_t value) {
	printf("Cart Write(%04X)\n", address);
	//NOT IMPLEMENTED
}