#ifndef H_EM_CLIPBOARD
#define H_EM_CLIPBOARD
#include <emscripten.h>
#include <stdio.h>

typedef void (*paste_handler)(const char *text, void *userdata);

// Forward declaration of the C callback that JS will call
EMSCRIPTEN_KEEPALIVE
void emscripten_browser_clipboard_paste_return(const char *paste_data,
                                               paste_handler callback,
                                               void *userdata);

// Register paste event handler in JS, calling back into C
EM_JS(void, paste_js, (paste_handler callback, void *userdata), {
    navigator.clipboard.readText().then(text => {
      Module.ccall(
        'emscripten_browser_clipboard_paste_return',
        'void',
        ['string', 'number', 'number'],
        [text, callback, userdata]
      );
    }).catch(err => {
      console.log("Clipboard read failed:", err);
    });
});
/*
EM_JS(void, paste_js, (paste_handler callback, void *userdata), {
    canvas.addEventListener("click", () => {
  navigator.clipboard.readText().then(text => {
    Module.ccall(
      'emscripten_browser_clipboard_paste_return',
      'void',
      ['string', 'number', 'number'],
      [event.clipboardData.getData('text/plain'), callback, userdata]
    );
  }).catch(err => {
    console.log("Clipboard read failed:", err);
  });
  });
});*/

// C function you call to start listening for paste events
static inline void clipboard_listen_for_paste(paste_handler cb, void *userdata) {
  paste_js(cb, userdata);
}

// Implementation of the callback entry point
void emscripten_browser_clipboard_paste_return(const char *paste_data,
                                               paste_handler callback,
                                               void *userdata) {
  if (callback) {
    callback(paste_data, userdata);
  }
}
#endif
