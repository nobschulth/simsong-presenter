#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <SDL3/SDL.h>
#include <stdlib.h>
#include <string.h>
#include <SDL3/SDL.h>
#include "SDL3/SDL_stdinc.h"
#include "SDL3_image/SDL_image.h"
#include "clay.h"
#include "clay_layout.h"
#include "../utils.h"
#include "../song.h"
#include "../app.h"
#include "clay_renderer.h"
#ifdef __EMSCRIPTEN__
#include "../em/em_clipboard.h"
#endif

const Clay_Color COLOR_BLACK = { 0, 0, 0, 255 };
const Clay_Color COLOR_BLACK_BG = { 17, 17, 17, 255 };
const Clay_Color COLOR_WHITE = { 255, 255, 255, 255 };
const Clay_Color COLOR_ACCENT_RED = { 255, 96, 96, 255 };
const Clay_Color COLOR_ACCENT_RED_LIGHT = { 255, 128, 130, 255 };
const Clay_Color COLOR_ACCENT_RED_DARK = { 255, 73, 79, 255 };
const Clay_Color COLOR_ACCENT_RED_ALPHA = { 255, 96, 96, 173 };

const Clay_ElementDeclaration elementBg = {
    .backgroundColor = COLOR_BLACK_BG,
    .layout = {
        .layoutDirection = CLAY_TOP_TO_BOTTOM,
        .childAlignment = { .x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_CENTER },
        .childGap = 20,
        .sizing = { .height = CLAY_SIZING_GROW(), .width = CLAY_SIZING_GROW(0) },
    }
};

SDL_Texture** g_images = NULL;
size_t g_imageSize = 1;
Song* g_currentSong = NULL;
size_t g_currentSongElementSelected = -1;
int* g_tabButtonIdData = NULL;
Book* g_currentBook = NULL;
int* g_bookButtonIdData = NULL;

void Layout_Initialize(SDL_Renderer* renderer) {
    const int imageCount = 1;
    g_images = safe_malloc(sizeof(SDL_Texture*) * imageCount);
    g_images[0] = IMG_LoadTexture(renderer, "resources/Images/Portrait/p1.png"); //Road with mountain
    for (int i = 0; i < imageCount; i++) {
        if (!g_images[i]) {
            SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load image!");
            exit(1);
        }
    }
}

void Layout_Quit() {
    for (int i = 0; i < g_imageSize; i++) {
        SDL_DestroyTexture(g_images[i]);
    }
    if (g_tabButtonIdData) {
        free(g_tabButtonIdData);
        g_tabButtonIdData = NULL;
    }
}

void Layout_Button_SongBack(Clay_ElementId elementId, Clay_PointerData pointerData, intptr_t userData) {
    if (pointerData.state == CLAY_POINTER_DATA_RELEASED_THIS_FRAME) {
        g_currentSongElementSelected = -1;
        if (!g_currentBook)
            Song_free(g_currentSong);
        g_currentSong = NULL;
        SDL_Clay_RenderQueueTextRedraw(1);
    }
}

void Layout_Button_BookBack(Clay_ElementId elementId, Clay_PointerData pointerData, intptr_t userData) {
    if (pointerData.state != CLAY_POINTER_DATA_RELEASED_THIS_FRAME) return;
    Book_free(g_currentBook);
    g_currentBook = NULL;
    free(g_bookButtonIdData);
    g_bookButtonIdData = NULL;
}

void Layout_Button_StartSong(Clay_ElementId elementId, Clay_PointerData pointerData, intptr_t userData) {
    if (pointerData.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME) {
        SDL_Clay_RenderQueueTextRedraw(2);
#ifdef __EMSCRIPTEN__
        clipboard_listen_for_paste(Layout_PasteSong, NULL);
        return;
#endif
        char* clipboardText = SDL_GetClipboardText();
        Song* song = Song_CreateFromString(clipboardText);
        SDL_free(clipboardText);
        if (!song) { return; }
        g_currentSong = song;
        g_currentSongElementSelected = 0;
    }
}

void Layout_PasteSong(const char* text, void* userdata) {
    SDL_Clay_RenderQueueTextRedraw(2);
    Song* song = Song_CreateFromString(text);
    if (!song) { return; }
    g_currentSong = song;
    g_currentSongElementSelected = 0;
}

void Layout_Button_StartBook(Clay_ElementId elementId, Clay_PointerData pointerData, intptr_t userData) {
    if (pointerData.state == CLAY_POINTER_DATA_RELEASED_THIS_FRAME) {
        SDL_Clay_RenderQueueTextRedraw(2);
#ifdef __EMSCRIPTEN__
        clipboard_listen_for_paste(Layout_PasteBook, NULL);
        return;
#endif
        char* clipboardText = SDL_GetClipboardText();
        Book* book = Book_CreateFromString(clipboardText);
        SDL_free(clipboardText);
        if (!book) { return; }
        g_currentBook = book;
    }
}

void Layout_PasteBook(const char* text, void* userdata) {
    SDL_Clay_RenderQueueTextRedraw(2);
    Book* book = Book_CreateFromString(text);
    if (!book) { return; }
    g_currentBook = book;
}

void Layout_Component_Button(Clay_String text, void (*hoverFunc)(Clay_ElementId elementId, Clay_PointerData pointerData, intptr_t userData), void* userData) {
    CLAY_AUTO_ID({
        .backgroundColor = Clay_Hovered() ? COLOR_ACCENT_RED_LIGHT : COLOR_ACCENT_RED,
        .layout = { 
            .padding = CLAY_PADDING_ALL(5),
            .childAlignment = { .x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_CENTER },
        },
    }) {
        if (hoverFunc)
            Clay_OnHover(hoverFunc, userData != NULL ? ((intptr_t)userData) : (intptr_t)NULL);
        CLAY_TEXT(text, CLAY_TEXT_CONFIG({ 
            .textColor = COLOR_WHITE,
            .fontId = 1,
            .fontSize = 42 * getFontScale(),
        }));
    }
}

void Layout_Render() {
    if (g_currentSong) {
        Layout_Song1();
    } else if (g_currentBook != NULL) {
        Layout_BookSongSelect();
    } else {
        Layout_Main();
    }
}

void Layout_Main() {
    CLAY(CLAY_ID("Main"), {
        .backgroundColor = COLOR_BLACK_BG,
        .layout = {
            .layoutDirection = CLAY_TOP_TO_BOTTOM,
            .childAlignment = { .x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_CENTER },
            .childGap = 40,
            .sizing = { .height = CLAY_SIZING_GROW(), .width = CLAY_SIZING_GROW(0) },
        }
    }) {
        CLAY_TEXT(CLAY_STRING("Copy a song from Songselect and paste it to start presenting"), CLAY_TEXT_CONFIG({
            .textColor = COLOR_WHITE,
            .fontId = 1,
            .fontSize = 28 * getFontScale(),
        }));
        Layout_Component_Button(CLAY_STRING("Paste song from clipboard"), Layout_Button_StartSong, NULL);
        Layout_Component_Button(CLAY_STRING("Paste songbook from clipboard"), Layout_Button_StartBook, NULL);
    }
}

void Layout_Button_BookSongSelect(Clay_ElementId elementId, Clay_PointerData pointerData, intptr_t userData) {
    if (pointerData.state != CLAY_POINTER_DATA_RELEASED_THIS_FRAME) return;
    SDL_Clay_RenderQueueTextRedraw(2);
    g_currentSong = g_currentBook->songs[*(int*)userData];
    g_currentSongElementSelected = 0;
}

void Layout_Button_Tabbar(Clay_ElementId elementId, Clay_PointerData pointerData, intptr_t userData) {
    if (pointerData.state == CLAY_POINTER_DATA_RELEASED_THIS_FRAME) {
        SDL_Clay_RenderQueueTextRedraw(1);
        g_currentSongElementSelected = *(int*)userData;
    }
}

void Layout_Componenet_Tabbar() {
    CLAY(CLAY_ID("Tabbar"), {
        .layout = {
            .layoutDirection = CLAY_LEFT_TO_RIGHT,
            .childAlignment = { .x = CLAY_ALIGN_X_LEFT, .y = CLAY_ALIGN_Y_CENTER },
            .sizing = { .height = CLAY_SIZING_FIT(), .width = CLAY_SIZING_GROW() },
        },
        .clip = { .horizontal = true, .childOffset = Clay_GetScrollOffset() }
    }) {
        if (!g_tabButtonIdData) {
            g_tabButtonIdData = safe_malloc(sizeof(int) * g_currentSong->elementCount);
        }
        //render the clickable items
        for (int i = 0; i < g_currentSong->elementCount; i++) {
            g_tabButtonIdData[i] = i;
            char* currentTitle = g_currentSong->elements[i].name;
            CLAY(CLAY_IDI("TabbarItem", i), {
                .backgroundColor = i == g_currentSongElementSelected ? COLOR_WHITE : COLOR_BLACK_BG,
                .layout = {
                    .padding = CLAY_PADDING_ALL(5),
                    .childAlignment = { .x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_CENTER }
                },
            }) {
                Clay_OnHover(Layout_Button_Tabbar, (intptr_t)(g_tabButtonIdData + i));
                CLAY_TEXT(((Clay_String){ .chars = currentTitle, .length = strlen(currentTitle), .isStaticallyAllocated = false}), CLAY_TEXT_CONFIG({
                    .textAlignment = CLAY_TEXT_ALIGN_CENTER,
                    .fontId = 1,
                    .fontSize = 49 * getFontScale(),
                    .textColor = i == g_currentSongElementSelected ? COLOR_BLACK_BG : COLOR_WHITE,
                }));
            }
        }
    }
}

void Layout_BookSongSelect() {
    if (!g_bookButtonIdData) {
        g_bookButtonIdData = safe_malloc(sizeof(int) * g_currentBook->songCount);
    }
    CLAY_AUTO_ID({
        .backgroundColor = COLOR_BLACK_BG, 
        .layout = {
            .layoutDirection = CLAY_LEFT_TO_RIGHT,
            .childAlignment = { .x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_TOP },
            .sizing = { .height = CLAY_SIZING_GROW(), .width = CLAY_SIZING_GROW() }
        }
    }) {
        CLAY(CLAY_ID("SongSelect"), {
            .layout = {
                .layoutDirection = CLAY_TOP_TO_BOTTOM,
                .childAlignment = { .x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_CENTER },
                .sizing = { .height = CLAY_SIZING_GROW(), .width = CLAY_SIZING_GROW() },
                .padding = CLAY_PADDING_ALL(30),
                .childGap = 20,
            },
            .clip = { .vertical = true, .childOffset = Clay_GetScrollOffset() },
        }) {
            for (int i = 0; i < g_currentBook->songCount; i++) {
                g_bookButtonIdData[i] = i;
                Clay_String stringButton = { .chars = g_currentBook->songs[i]->title, .isStaticallyAllocated = false, .length = strlen(g_currentBook->songs[i]->title) };
                Layout_Component_Button(stringButton, Layout_Button_BookSongSelect, (g_bookButtonIdData + i));
            }
        }
        CLAY_AUTO_ID({
            .layout = {
                .sizing = { .width = CLAY_SIZING_PERCENT(0), .height = CLAY_SIZING_PERCENT(1)},
                .childAlignment = { .x = CLAY_ALIGN_X_RIGHT, .y = CLAY_ALIGN_Y_BOTTOM },
            }
        }) {
            Layout_Component_Button(CLAY_STRING("Back"), Layout_Button_BookBack, NULL);
        }
    }
}

void Layout_Song1() {
    Clay_String stringCredits = { .chars = g_currentSong->licence, .isStaticallyAllocated = false, .length = strlen(g_currentSong->licence) };
    Clay_String stringTitle = { .chars = g_currentSong->title, .isStaticallyAllocated = false, .length = strlen(g_currentSong->title) };
    char* currentText = g_currentSong->elements[g_currentSongElementSelected].text;
    CLAY(CLAY_ID("Song1"), {
        .backgroundColor = COLOR_BLACK_BG, 
        .layout = {
            .layoutDirection = CLAY_LEFT_TO_RIGHT,
            .childAlignment = { .x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_CENTER },
            .sizing = { .height = CLAY_SIZING_GROW(), .width = CLAY_SIZING_GROW() },
        }
    }) {
        //Left gray box
        CLAY_AUTO_ID({
            .layout = {
                .layoutDirection = CLAY_TOP_TO_BOTTOM,
                .childAlignment = { .x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_TOP },
                .sizing = { .height = CLAY_SIZING_GROW(), .width = CLAY_SIZING_GROW() },
            }
        }) {
            Layout_Componenet_Tabbar();
            CLAY_AUTO_ID({.layout = { .sizing = { .height = CLAY_SIZING_PERCENT(0.1) } }}) {}
            CLAY_AUTO_ID({
                .layout = {
                    .childAlignment = { .x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_CENTER },
                    .padding = { .left = 30, .right = 30 }
                }
            }) {
            CLAY_TEXT(((Clay_String){ .chars = currentText, .length = strlen(currentText), .isStaticallyAllocated = false }), CLAY_TEXT_CONFIG({
                .textColor = COLOR_WHITE,
                .fontId = 1,
                .fontSize = 60 * getFontScale(),
                .textAlignment = CLAY_TEXT_ALIGN_CENTER,
            }));
            }
            //Credits
            //CLAY_TEXT((Clay_String){ .chars = "123" }, CLAY_TEXT_CONFIG({ .fontId = 2 }));
            CLAY_AUTO_ID({
                .layout = {
                    .childAlignment = { .x = CLAY_ALIGN_X_LEFT, .y = CLAY_ALIGN_Y_BOTTOM },
                    .sizing = { .height = CLAY_SIZING_GROW(), .width = CLAY_SIZING_GROW() },
                    .padding = CLAY_PADDING_ALL(10),
                }
            }) {
                CLAY_TEXT(stringCredits, CLAY_TEXT_CONFIG({
                    .textColor = COLOR_WHITE,
                    .fontId = 1,
                    .fontSize = 10 * getFontScale(),
                    .textAlignment = CLAY_TEXT_ALIGN_LEFT,
                }));
            }
        }

        //right image
        CLAY_AUTO_ID({
            //.aspectRatio = 9.0/16,
            .image = { .imageData = g_images[0] },
            .layout = {
                //using this calculation as the aspectRatio field gives wrong results
                //it fills the height and calculates the width required to be 9:16
                .sizing = { .height = CLAY_SIZING_GROW(), .width = CLAY_SIZING_FIXED(9 * Clay_GetElementData(Clay_GetElementId(CLAY_STRING("Song1"))).boundingBox.height / 16.0) },
                .layoutDirection = CLAY_TOP_TO_BOTTOM, 
                .childAlignment = { .x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_TOP }
            },
            .clip = true,
        }) {
            CLAY_AUTO_ID({ .layout = { .sizing = { .height = CLAY_SIZING_PERCENT(0.2) } } }) {}
            //Red Title label on image
            CLAY_AUTO_ID({
                .backgroundColor = COLOR_ACCENT_RED_ALPHA,
                .layout = {
                    .sizing = { .height = CLAY_SIZING_FIT(), .width = CLAY_SIZING_PERCENT(1) },
                    .childAlignment = { .x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_CENTER },
                    .padding = CLAY_PADDING_ALL(20),
                }
            }) {
                CLAY_TEXT(stringTitle, CLAY_TEXT_CONFIG({
                    .fontId = 2,
                    .fontSize = 56 * getFontScale(),
                    .textColor = COLOR_WHITE,
                    .textAlignment = CLAY_TEXT_ALIGN_CENTER,
                }));
            }
        }
        CLAY_AUTO_ID({
            .layout = {
                .sizing = { .height = CLAY_SIZING_GROW(1), .width = CLAY_SIZING_PERCENT(0) },
                .childAlignment = { .x = CLAY_ALIGN_X_RIGHT, .y = CLAY_ALIGN_Y_BOTTOM },
            }

        }) {
            Layout_Component_Button(CLAY_STRING("Back"), Layout_Button_SongBack, NULL);
        }
    }
}
