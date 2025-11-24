#ifndef H_CLAY_LAYOUT
#define H_CLAY_LAYOUT
#include <SDL3/SDL.h>
#include <stddef.h>
#include "clay.h"

//Loads the images
void Layout_Initialize(SDL_Renderer* renderer);
void Layout_Quit();

void Layout_Button_StartSong(Clay_ElementId elementId, Clay_PointerData pointerData, intptr_t userData);
void Layout_PasteSong(const char* text, void* userdata);
void Layout_Button_StartBook(Clay_ElementId elementId, Clay_PointerData pointerData, intptr_t userData);
void Layout_PasteBook(const char* text, void* userdata);

void Layout_Button_SongBack(Clay_ElementId elementId, Clay_PointerData pointerData, intptr_t userData);
void Layout_Button_BookBack(Clay_ElementId elementId, Clay_PointerData pointerData, intptr_t userData);

void Layout_Component_Button(Clay_String text, void (*hoverFunc)(Clay_ElementId elementId, Clay_PointerData pointerData, intptr_t userData), void* userData);
void Layout_Render();
void Layout_Main();

void Layout_Button_BookSongSelect(Clay_ElementId elementId, Clay_PointerData pointerData, intptr_t userData);

typedef struct LayoutButtonTabbar {
    size_t id;
}LayoutButtonTabbar ;

void Layout_Button_Tabbar(Clay_ElementId elementId, Clay_PointerData pointerData, intptr_t userData);
void Layout_Componenet_Tabbar();
void Layout_BookSongSelect();
void Layout_Song1();


#endif // !H_CLAY_LAYOUT
