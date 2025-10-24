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

Song* Song_CreateFromString(const char* string);
Song* Song_free(Song* song);

#endif // !H_SONG
