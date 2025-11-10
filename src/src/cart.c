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
	"MBC1+RAM",
	"MBC1+RAM+BATTERY",
	"0x04 ???",
	"MBC2",
	"MBC2+BATTERY",
	"0x07 ???",
	"ROM+RAM 1",
	"ROM+RAM+BATTERY 1",
	"0x0A ???",
	"MMM01",
	"MMM01+RAM",
	"MMM01+RAM+BATTERY",
	"0x0E ???",
	"MBC3+TIMER+BATTERY",
	"MBC3+TIMER+RAM+BATTERY 2",
	"MBC3",
	"MBC3+RAM 2",
	"MBC3+RAM+BATTERY 2",
	"0x14 ???",
	"0x15 ???",
	"0x16 ???",
	"0x17 ???",
	"0x18 ???",
	"MBC5",
	"MBC5+RAM",
	"MBC5+RAM+BATTERY",
	"MBC5+RUMBLE",
	"MBC5+RUMBLE+RAM",
	"MBC5+RUMBLE+RAM+BATTERY",
	"0x1F ???",
	"MBC6",
	"0x21 ???",
	"MBC7+SENSOR+RUMBLE+RAM+BATTERY",
};

static const char *LicCode[0xA5] = {
	[0x00] = "None",
	[0x01] = "Nintendo R&D1",
	[0x08] = "Capcom",
	[0x13] = "Electronic Arts",
	[0x18] = "Hudson Soft",
	[0x19] = "b-ai",
	[0x20] = "kss",
	[0x22] = "pow",
	[0x24] = "PCM Complete",
	[0x25] = "san-x",
	[0x28] = "Kemco Japan",
	[0x29] = "seta",
	[0x30] = "Viacom",
	[0x31] = "Nintendo",
	[0x32] = "Bandai",
	[0x33] = "Ocean/Acclaim",
	[0x34] = "Konami",
	[0x35] = "Hector",
	[0x37] = "Taito",
	[0x38] = "Hudson",
	[0x39] = "Banpresto",
	[0x41] = "Ubi Soft",
	[0x42] = "Atlus",
	[0x44] = "Malibu",
	[0x46] = "angel",
	[0x47] = "Bullet-Proof",
	[0x49] = "irem",
	[0x50] = "Absolute",
	[0x51] = "Acclaim",
	[0x52] = "Activision",
	[0x53] = "American sammy",
	[0x54] = "Konami",
	[0x55] = "Hi tech entertainment",
	[0x56] = "LJN",
	[0x57] = "Matchbox",
	[0x58] = "Mattel",
	[0x59] = "Milton Bradley",
	[0x60] = "Titus",
	[0x61] = "Virgin",
	[0x64] = "LucasArts",
	[0x67] = "Ocean",
	[0x69] = "Electronic Arts",
	[0x70] = "Infogrames",
	[0x71] = "Interplay",
	[0x72] = "Broderbund",
	[0x73] = "sculptured",
	[0x75] = "sci",
	[0x78] = "THQ",
	[0x79] = "Accolade",
	[0x80] = "misawa",
	[0x83] = "lozc",
	[0x86] = "Tokuma Shoten Intermedia",
	[0x87] = "Tsukuda Original",
	[0x91] = "Chunsoft",
	[0x92] = "Video system",
	[0x93] = "Ocean/Acclaim",
	[0x95] = "Varie",
	[0x96] = "Yonezawa/s’pal",
	[0x97] = "Kaneko",
	[0x99] = "Pack in soft",
	[0xA4] = "Konami (Yu-Gi-Oh!)"
};

const char *cartLicName() {
	if(ctx.header->newLicCode <= 0xA4) {
		return LicCode[ctx.header->licCode];
	}

	return "UNKNOWN";
}

const char *cartTypeName() {
    if (ctx.header->type <= 0x22) {
        return RomTypes[ctx.header->type];
    }

    return "UNKNOWN";
}

bool cartLoad(char *cart) {
    snprintf(ctx.filename, sizeof(ctx.filename), "%s", cart);

    FILE *fp = fopen(cart, "r");

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
        x = x - ctx.romData[i] - 1;
    }

    printf("\t Checksum : %2.2X (%s)\n", ctx.header->checksum, (x & 0xFF) ? "PASSED" : "FAILED");

    return true;
}

uint8_t cartRead(uint16_t address) {
	//ROM ONLY SUPPORTED TYPE AS OF RIGHT NOW

	return ctx.romData[address];
}

void cartWrite(uint16_t address, uint8_t value) {
	printf("Cart Write attempted at 0x%04X with value 0x%02X (NOT IMPLEMENTED)\n", address, value);
	//NOIMPL
}