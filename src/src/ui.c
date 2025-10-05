#include "../inc/ui.h"
#include "../inc/emu.h"

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

SDL_Window *sdlWindow;
SDL_Renderer *sdlRenderer;
SDL_Texture *sdlTexture;
SDL_Surface *screen;

void uiInit() {
    SDL_CreateWindowAndRenderer("yep",SCREEN_WIDTH, SCREEN_HEIGHT, 0, &sdlWindow, &sdlRenderer);
}

void delay(uint32_t ms) {
    SDL_Delay(ms);
}

void uiHandleEvents() {
    SDL_Event e;
    while (SDL_PollEvent(&e) > 0)
    {
        //TODO SDL_UpdateWindowSurface(sdlWindow);
        //TODO SDL_UpdateWindowSurface(sdlTraceWindow);
        //TODO SDL_UpdateWindowSurface(sdlDebugWindow);

        if (e.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED) {
            emuGetContext()->die = true;
        }
    }
}