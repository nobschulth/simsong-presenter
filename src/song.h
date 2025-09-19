#ifndef H_SONG
#define H_SONG

//represents a verse, bridge, ...
#include <stddef.h>
typedef struct SongElement {
    char* name;
    char* text;
} SongElement;

typedef struct Song {
    char* title;
    char* licence;
    SongElement* elements;
    size_t elementCount;
} Song;

Song* Song_CreateFromString(char* string);

#endif // !H_SONG
