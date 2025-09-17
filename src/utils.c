#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "SDL3/SDL_stdinc.h"
#include "utils.h"

void* safe_malloc(size_t size) {
    void* p = SDL_malloc(size);
    if (p == NULL) {
        fprintf(stderr, "Fatal: failed to allocate %zu bytes. Terminating.\n", size);
        exit(1);
    }
    return p;
}

void* safe_realloc(void* ptr, size_t size) {
    void* p = SDL_realloc(ptr, size);
    if (p == NULL) {
        fprintf(stderr, "Fatal: failed to reallocate %zu bytes. Terminating.\n", size);
        exit(1);
    }
    return p;
}
