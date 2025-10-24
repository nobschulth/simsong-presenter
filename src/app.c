#include <stdio.h>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "app.h"
#include "SDL3/SDL_video.h"
#include "utils.h"

Appstate* g_appstate = NULL;

void setAppstate(Appstate* appstate) {
    g_appstate = appstate;
}

void appstate_free(Appstate* appstate) {
    SDL_DestroyWindow(appstate->sdlWindow);
    SDL_DestroyRenderer(appstate->rendererData.renderer);
    TTF_DestroySurfaceTextEngine(appstate->rendererData.textEngine);
    for (int i = 0; i < appstate->rendererData.fontLength; i++) {
        TTF_CloseFont(appstate->rendererData.fonts[i]);
    }
}

bool appstate_load_font(Appstate* appstate, char* path) {
    TTF_Font* font = TTF_OpenFont(path, 20);
    if (!font) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load font in path \"%s\": %s", path, SDL_GetError());
        return false;
    }
    appstate->rendererData.fontLength++;
    if (appstate->rendererData.fontLength < 1) {
        appstate->rendererData.fonts = safe_malloc(1);
    } else {
        appstate->rendererData.fonts = safe_realloc(appstate->rendererData.fonts, appstate->rendererData.fontLength);
    }
    appstate->rendererData.fonts[appstate->rendererData.fontLength - 1] = font;
    return true;
}

Clay_Dimensions SDL_MeasureText(Clay_StringSlice text, Clay_TextElementConfig *config, void *userData) {
    TTF_Font **fonts = userData;
    TTF_Font *font = fonts[config->fontId];
    int width, height;

    TTF_SetFontSize(font, config->fontSize);
    if (!TTF_GetStringSize(font, text.chars, text.length, &width, &height)) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to measure text: %s", SDL_GetError());
    }

    return (Clay_Dimensions) { (float) width, (float) height };
}

void HandleClayErrors(Clay_ErrorData errorData) {
    printf("%s", errorData.errorText.chars);
}

float getFontScale() {
    int h = 0;
    SDL_GetWindowSize(g_appstate->sdlWindow, NULL, &h);
    return h / 900.0;
}
