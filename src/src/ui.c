#include "../inc/ui.h"
#include "../inc/emu.h"
#include "../inc/bus.h"

#include <SDL.h>
#include <SDL_ttf.h>

SDL_Window *sdlWindow;
SDL_Renderer *sdlRenderer;
SDL_Texture *sdlTexture;
SDL_Surface *screen;

SDL_Window *sdlDebugWindow;
SDL_Renderer *sdlDebugRenderer;
SDL_Texture *sdlDebugTexture;
SDL_Surface *debugScreen;

static int scale = 4;

void uiInit() {

    SDL_Init(SDL_INIT_VIDEO);
    printf("SDL INIT\n");
    TTF_Init();
    printf("TTF INIT\n");

    SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &sdlWindow, &sdlRenderer);

    SDL_CreateWindowAndRenderer(16*8*scale, 32*8*scale, 0, &sdlDebugWindow, &sdlDebugRenderer);

    debugScreen = SDL_CreateRGBSurface(0, (16*8*scale) + (16*scale),
        (32*8*scale) + (64*scale),32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);

    sdlDebugTexture = SDL_CreateTexture(sdlDebugRenderer, SDL_PIXELFORMAT_ARGB8888,SDL_TEXTUREACCESS_STREAMING,
       (16*8*scale) + (16*scale),
        (32*8*scale) + (64*scale));

    int x,y;
    SDL_GetWindowPosition(sdlWindow, &x, &y);
    SDL_SetWindowPosition(sdlDebugWindow, x + SCREEN_WIDTH + 10, y);
}

void delay(uint32_t ms) {
    SDL_Delay(ms);
}

static unsigned long tileColours[4] = {0xFFFFFFFF, 0xFFAAAAAA, 0xFF555555, 0xFF000000};

void displayTile(SDL_Surface *surface, uint16_t startLocation, uint16_t tileNum, int x, int y) {
    SDL_Rect rc;

    for (int tileY=0; tileY<16; tileY += 2) {
        uint8_t b1 = busRead(startLocation + (tileNum * 16) + tileY);
        uint8_t b2 = busRead(startLocation + (tileNum * 16) + tileY + 1);

        for (int bit =7; bit >= 0; bit--) {
            uint8_t hi = !!(b1 & (1 << bit)) << 1;
            uint8_t lo = !!(b2 & (1 << bit));

            uint8_t colour = hi | lo;

            rc.x = x + ((7 - bit) * scale);
            rc.y = y + (tileY / 2 * scale);
            rc.w = scale;
            rc.h = scale;

            SDL_FillRect(surface, &rc, tileColours[colour]);
        }
    }
}

void updateDBGWindow() {
    int xDraw = 0;
    int yDraw = 0;
    int tileNum = 0;

    SDL_Rect rc;
    rc.x = 0;
    rc.y = 0;
    rc.w = debugScreen->w;
    rc.h = debugScreen->h;
    SDL_FillRect(debugScreen, &rc, 0xFF111111);

    uint16_t address = 0x8000;

    //Tile Count (24x16) - 384 Tiles
    for (int y = 0; y < 24; y++) {
        for (int x = 0; x < 16; x++) {
            displayTile(debugScreen, address, tileNum, xDraw + (x * scale), yDraw + (y * scale));
            xDraw += (8 * scale);
            tileNum++;
        }

        yDraw += (8*scale);
        xDraw = 0;
    }

    SDL_UpdateTexture(sdlDebugTexture, NULL, debugScreen->pixels, debugScreen->pitch);
    SDL_RenderClear(sdlDebugRenderer);
    SDL_RenderCopy(sdlDebugRenderer, sdlDebugTexture, NULL, NULL);
    SDL_RenderPresent(sdlDebugRenderer);
}

void uiUpdate() {
    updateDBGWindow();
}

void uiHandleEvents() {
    SDL_Event e;
    while (SDL_PollEvent(&e) > 0)
    {
        //TODO SDL_UpdateWindowSurface(sdlWindow);
        //TODO SDL_UpdateWindowSurface(sdlTraceWindow);
        //TODO SDL_UpdateWindowSurface(sdlDebugWindow);

        if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_CLOSE) {
            emuGetContext()->die = true;
        }
    }
}