#ifndef H_EM_RESIZE
#define H_EM_RESIZE
#include <emscripten.h>
#include "../app.h"


EMSCRIPTEN_KEEPALIVE
void emscripten_resize_return(int pixelW, int pixelH, void* state);


EM_JS(void, resize_js, (void* state), {
function isFullscreen() {
    return !!(
        document.fullscreenElement ||       // standard
        document.webkitFullscreenElement || // Chrome, Safari, Opera
        document.mozFullScreenElement ||    // Firefox
        document.msFullscreenElement        // IE/Edge
    );
}
function resizeCanvas() {
    const pixelRatio = window.devicePixelRatio || 1;
    const fs = isFullscreen();
    const scale = fs ? 1 : 1/1.5;
    const width = window.innerWidth * scale;
    const height = window.innerHeight * scale;
    const canvas = Module.canvas;

    // Set the actual pixel size
    canvas.width = width * pixelRatio;
    canvas.height = height * pixelRatio;

    // Keep the CSS display size normal
    canvas.style.width = width + "px";
    canvas.style.height = height + "px";

  // Inform SDL of the new size
    Module.ccall(
        'emscripten_resize_return',
        'void',
        ['number', 'number', 'number'],
        [canvas.width, canvas.height, state]
            );
}

// Resize initially and when fullscreen or orientation changes
window.addEventListener('resize', resizeCanvas);
document.addEventListener('fullscreenchange', resizeCanvas);
resizeCanvas();
});

void emscripten_resize_return(int pixelW, int pixelH, void* state) {
    Appstate* appstate = (Appstate*)state;
    SDL_SetWindowSize(appstate->sdlWindow, pixelW, pixelH);
}
#endif
