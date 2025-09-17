#include "SDL3/SDL_init.h"
#include "SDL3/SDL_log.h"
#include "SDL3_ttf/SDL_ttf.h"
#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL_main.h>
#include <SDL3/SDL.h>
#define CLAY_IMPLEMENTATION
#include "clay/clay.h"
#include "clay/clay_renderer.h"
#include "app.h"
#include "utils.h"

const int WINDOW_W = 800;
const int WINDOW_H = 450;

//Ititialize SDL and Clay
SDL_AppResult SDL_AppInit(void** appstate, int argc, char *argv[]) {
// --- SDL ---
    SDL_SetAppMetadata("Simsong Presenter", "0.1.0", "com.nobschulth.simsong-presenter");
    Appstate* newAppstate = safe_malloc(sizeof(Appstate));
    newAppstate->previousTick = 0;

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not initializse SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer(
        "Simsong Presenter",
        WINDOW_W, WINDOW_H, //size
        0, //flags
        &(newAppstate->sdlWindow),
        &(newAppstate->rendererData.renderer)
    )) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    
    SDL_SetWindowResizable(newAppstate->sdlWindow, true);

    //appstate
    newAppstate->rendererData.renderer = newAppstate->rendererData.renderer;
    newAppstate->rendererData.textEngine = TTF_CreateRendererTextEngine(newAppstate->rendererData.renderer);
    if (!newAppstate->rendererData.textEngine) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to create text engine from renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    //load fonts
    if (!TTF_Init()) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to init TTF");
        return SDL_APP_FAILURE;
    }
    newAppstate->rendererData.fonts = safe_malloc(sizeof(TTF_Font*));
    newAppstate->rendererData.fonts[0] = TTF_OpenFont("resources/CaskaydiaMonoNerdFont-Regular.ttf", 20);
    for (int i = 0; i < 1; i++) {
        if (!newAppstate->rendererData.fonts[i]) {
            SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to allocate memory for the font array: %s", SDL_GetError());
            return SDL_APP_FAILURE;
        }
    }

// --- Clay ---
    uint64_t clayMinMemory = Clay_MinMemorySize();
    Clay_Arena arena = Clay_CreateArenaWithCapacityAndMemory(clayMinMemory, safe_malloc(clayMinMemory));
    Clay_Initialize(arena, (Clay_Dimensions){ (float) WINDOW_W, (float) WINDOW_H }, (Clay_ErrorHandler){ HandleClayErrors });
    Clay_SetMeasureTextFunction(SDL_MeasureText, newAppstate->rendererData.fonts);

    Clay_SetDebugModeEnabled(true);
    *appstate = newAppstate;
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* voidstate, SDL_Event* event) {
    switch (event->type) {
        case SDL_EVENT_QUIT:
            return SDL_APP_SUCCESS;
        case SDL_EVENT_WINDOW_RESIZED:
            Clay_SetLayoutDimensions((Clay_Dimensions) { (float) event->window.data1, (float) event->window.data2 });
            break;
        case SDL_EVENT_MOUSE_MOTION:
            Clay_SetPointerState((Clay_Vector2) { event->motion.x, event->motion.y },
                event->motion.state & SDL_BUTTON_LMASK);
            break;
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
            Clay_SetPointerState((Clay_Vector2) { event->button.x, event->button.y },
                event->button.button == SDL_BUTTON_LEFT);
            break;
        case SDL_EVENT_MOUSE_WHEEL:
            Clay_UpdateScrollContainers(true, (Clay_Vector2) { event->wheel.x, event->wheel.y }, 0.01f);
            break;
    }
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* voidstate) {
    Appstate* appstate = (Appstate*)voidstate;
    //calculate deltatime
    Uint64 tick = SDL_GetTicks();
    const double delta = appstate->previousTick - tick / 1000.0;

    //clear
    SDL_SetRenderDrawColor(appstate->rendererData.renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(appstate->rendererData.renderer);
    
    Clay_BeginLayout();
    CLAY(CLAY_ID("Main"), { .layout = { .sizing = { .height = CLAY_SIZING_GROW(0), .width = CLAY_SIZING_GROW(0) }, .padding = CLAY_PADDING_ALL(15) }, .backgroundColor = { 25, 25, 25, 255 } }) {
        CLAY(CLAY_ID("InnerRect"), { .layout = { .sizing = { .height = CLAY_SIZING_GROW(0), .width = CLAY_SIZING_GROW(0) }, .padding = CLAY_PADDING_ALL(15)}, .backgroundColor = {200, 200, 200, 255}, .cornerRadius = 15 }) {

        }
    }
    Clay_RenderCommandArray renderCommands = Clay_EndLayout();
    SDL_Clay_RenderClayCommands(&appstate->rendererData, &renderCommands);

    SDL_RenderPresent(appstate->rendererData.renderer);

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* voidstate, SDL_AppResult result) {
    //sdl does the window and renderer cleanup, so no need to free them
}
