#ifndef H_EM_RESIZE
#define H_EM_RESIZE
#include <emscripten.h>
#include <stdio.h>

typedef void (*resize_handler)(int pixelW, int pixelH, int w, int h, void* state);

EMSCRIPTEN_KEEPALIVE
void emscripten_resize_return(resize_handler handler, int pixelW, int pixelH, int w, int h, void* state);


EM_JS(void, resize_js, (resize_handler handler, void* state), {
function resizeCanvas() {
    const pixelRatio = window.devicePixelRatio || 1;
    const width = window.innerWidth;
    const height = window.innerHeight;
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
        ['number', 'number', 'number', 'number', 'number', 'number'],
        [handler, canvas.width, canvas.height, width, height, state]
            );
}

// Resize initially and when fullscreen or orientation changes
window.addEventListener('resize', resizeCanvas);
document.addEventListener('fullscreenchange', resizeCanvas);
resizeCanvas();
});

void emscripten_resize_return(resize_handler handler, int pixelW, int pixelH, int w, int h, void* state) {
    if (handler) {
        handler(pixelW, pixelH, w, h, state);
    }
}
#endif
