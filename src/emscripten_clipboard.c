#include <SDL3/SDL.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>

// Expose a JS function to read clipboard
EM_JS(void, emscripten_request_clipboard, (), {
    // Only works inside a user event handler (e.g. keydown/click)
    if (navigator.clipboard && navigator.clipboard.readText) {
        navigator.clipboard.readText().then(function(text) {
            _SDL_SetClipboardText(text); // call SDL’s clipboard setter
        }).catch(function(err) {
            console.warn("Clipboard read failed:", err);
        });
    } else {
        console.warn("Clipboard API not supported.");
    }
});
#endif

// Ready-to-use function
char* get_clipboard_text(void) {
#ifdef __EMSCRIPTEN__
    // Request clipboard update from browser
    emscripten_request_clipboard();
    // Return whatever SDL currently has stored
    return SDL_GetClipboardText();
#else
    // On native builds, just use SDL
    return SDL_GetClipboardText();
#endif
}
