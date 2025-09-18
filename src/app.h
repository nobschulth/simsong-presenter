#ifndef H_APP
#define H_APP
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "clay/clay.h"
#include "clay/clay_renderer.h"

typedef struct Appstate {
    SDL_Window* sdlWindow;
    Uint64 previousTick;
    Clay_SDL3RendererData rendererData;
    bool pointerDown;
    Clay_Vector2 scrollDelta;
} Appstate;

void appstate_free(Appstate* appstate);
//quite useless but I leave it here
bool appstate_load_font(Appstate* appstate, char* path);

Clay_Dimensions SDL_MeasureText(Clay_StringSlice text, Clay_TextElementConfig *config, void *userData);
void HandleClayErrors(Clay_ErrorData errorData);

#endif
