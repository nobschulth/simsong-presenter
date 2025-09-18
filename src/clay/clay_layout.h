#ifndef H_CLAY_LAYOUT
#define H_CLAY_LAYOUT
#include <SDL3/SDL.h>
#include "clay.h"

//Loads the images
void Layout_Initialize(SDL_Renderer* renderer);

void Layout_Button_Start(Clay_ElementId elementId, Clay_PointerData pointerData, intptr_t userData);
void Layout_Component_Button(Clay_String text, void (*hoverFunc)(Clay_ElementId elementId, Clay_PointerData pointerData, intptr_t userData));
void Layout_Render();
void Layout_Main();
void Layout_Componenet_Tabbar();
void Layout_Song1();


#endif // !H_CLAY_LAYOUT
