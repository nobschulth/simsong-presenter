#include <stdint.h>
#include <stdio.h>
#include <SDL3/SDL.h>
#include <stdlib.h>
#include "SDL3/SDL_log.h"
#include "SDL3_image/SDL_image.h"
#include "clay.h"
#include "clay_layout.h"
#include "../utils.h"

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

SDL_Texture** images = NULL;

void Layout_Initialize(SDL_Renderer* renderer) {
    const int imageCount = 1;
    images = safe_malloc(sizeof(SDL_Texture*) * imageCount);
    images[0] = IMG_LoadTexture(renderer, "resources/Images/Portrait/p1.png"); //Road with mountain
    for (int i = 0; i < imageCount; i++) {
        if (!images[i]) {
            SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load image!");
            exit(1);
        }
    }
}

void Layout_Button_Start(Clay_ElementId elementId, Clay_PointerData pointerData, intptr_t userData) {
    if (pointerData.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME) {
        printf("%s\n", SDL_GetClipboardText());
    }
}

void Layout_Component_Button(Clay_String text, void (*hoverFunc)(Clay_ElementId elementId, Clay_PointerData pointerData, intptr_t userData)) {
    CLAY_AUTO_ID({
        .backgroundColor = Clay_Hovered() ? COLOR_ACCENT_RED_LIGHT : COLOR_ACCENT_RED,
        .layout = { 
            .padding = CLAY_PADDING_ALL(5),
            .childAlignment = { .x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_CENTER },
        },
    }) {
        Clay_OnHover(hoverFunc, (intptr_t)NULL);
        CLAY_TEXT(text, CLAY_TEXT_CONFIG({ 
            .textColor = COLOR_WHITE,
            .fontId = 1,
            .fontSize = 28,
        }));
    }
}

void Layout_Render() {
    //Layout_Main();
    Layout_Song1();
}

void Layout_Main() {
    CLAY(CLAY_ID("Main"), {
        .backgroundColor = COLOR_BLACK_BG,
        .layout = {
            .layoutDirection = CLAY_TOP_TO_BOTTOM,
            .childAlignment = { .x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_CENTER },
            .childGap = 20,
            .sizing = { .height = CLAY_SIZING_GROW(), .width = CLAY_SIZING_GROW(0) },
        }
    }) {
        CLAY_TEXT(CLAY_STRING("Copy a song from Songselect and press the button to start presenting"), CLAY_TEXT_CONFIG({
            .textColor = COLOR_WHITE,
            .fontId = 1,
            .fontSize = 20,
        }));
        Layout_Component_Button(CLAY_STRING("Paste song from clipboard"), Layout_Button_Start);
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
        //render the clickable items
        uint32_t selected = 2;
        for (int i = 0; i < 20; i++) {
            CLAY(CLAY_IDI("TabbarItem", i), {
                .backgroundColor = i == selected ? COLOR_WHITE : COLOR_BLACK_BG,
                .layout = {
                    .padding = CLAY_PADDING_ALL(5),
                    .childAlignment = { .x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_CENTER }
                }
            }) {
                CLAY_TEXT(CLAY_STRING("Item 2"), CLAY_TEXT_CONFIG({
                    .textAlignment = CLAY_TEXT_ALIGN_CENTER,
                    .fontId = 1,
                    .fontSize = 32,
                    .textColor = i == selected ? COLOR_BLACK_BG : COLOR_WHITE,
                }));
            }
        }
    }
}

void Layout_Song1() {
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
            CLAY_AUTO_ID({.layout = { .sizing = { .height = CLAY_SIZING_PERCENT(0.2) } }}) {}
            CLAY_TEXT(CLAY_STRING("Liedtext lorem impsum\nirelium de riesa.\nQua corrium da sie rum.\nLiemun sier krea.\nLeat zera strop."), CLAY_TEXT_CONFIG({
                .textColor = COLOR_WHITE,
                .fontId = 1,
                .fontSize = 60,
                .textAlignment = CLAY_TEXT_ALIGN_CENTER,
            })); 
            //Credits
            CLAY_AUTO_ID({
                .layout = {
                    .childAlignment = { .x = CLAY_ALIGN_X_LEFT, .y = CLAY_ALIGN_Y_BOTTOM },
                    .sizing = { .height = CLAY_SIZING_GROW(), .width = CLAY_SIZING_GROW() },
                    .padding = CLAY_PADDING_ALL(10),
                }
            }) {
                CLAY_TEXT(CLAY_STRING("©Lizens:\nlorem impsum\nirelium de riesa ©Qua corrium da sie rum. Liemun sier krea Leat zera strop."), CLAY_TEXT_CONFIG({
                    .textColor = COLOR_WHITE,
                    .fontId = 1,
                    .fontSize = 10,
                    .textAlignment = CLAY_TEXT_ALIGN_LEFT,
                }));
            }
        }

        //right image
        CLAY_AUTO_ID({
            //.aspectRatio = 9.0/16,
            .image = { .imageData = images[0] },
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
                    .sizing = { .height = CLAY_SIZING_FIT(), .width = CLAY_SIZING_GROW() },
                    .childAlignment = { .x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_CENTER },
                    .padding = CLAY_PADDING_ALL(20),
                }
            }) {
                CLAY_TEXT(CLAY_STRING("Songtitel\n Sehr lang"), CLAY_TEXT_CONFIG({
                    .fontId = 2,
                    .fontSize = 56,
                    .textColor = COLOR_WHITE,
                    .textAlignment = CLAY_TEXT_ALIGN_CENTER
                }));
            }
        }
    }
}
