#ifndef H_CLAY_RENDERER
#define H_CLAY_RENDERER
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>
#include <stddef.h>
#include <stdint.h>
#include "clay.h"

typedef struct {
    SDL_Renderer *renderer;
    TTF_TextEngine *textEngine;
    TTF_Font **fonts;
    size_t fontLength;
} Clay_SDL3RendererData;

//Text textures are cached so the text does not need to be redrawn each frame (mainly for the emscripten performance)
typedef struct {
    uint32_t id;
    SDL_Texture* texture;
    int w;
    int h;
} SDL_Clay_RenderTextCacheEntry;

typedef struct {
    SDL_Clay_RenderTextCacheEntry* entries;
    size_t count;
} SDL_Clay_RenderTextCache;

static void SDL_Clay_RenderFillRoundedRect(Clay_SDL3RendererData *rendererData, const SDL_FRect rect, const float cornerRadius, const Clay_Color _color);
static void SDL_Clay_RenderArc(Clay_SDL3RendererData *rendererData, const SDL_FPoint center, const float radius, const float startAngle, const float endAngle, const float thickness, const Clay_Color color);
void SDL_Clay_RenderClayCommands(Clay_SDL3RendererData *rendererData, Clay_RenderCommandArray *rcommands);

//sets the text to be redrawn this frame for performance reasons
//frames is the amount of frames to be redrawn
void SDL_Clay_RenderQueueTextRedraw(uint8_t frames);

SDL_Clay_RenderTextCacheEntry* SDL_Clay_RenderTextCacheGetEntry(uint32_t id);
#endif
