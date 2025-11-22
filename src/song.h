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

typedef struct Book {
    Song** songs;
    size_t songCount;
} Book;

Song* Song_CreateFromString(const char* string);
void Song_free(Song* song);

Book* Book_CreateFromString(const char* string);
void Book_free(Book* book);

#endif // !H_SONG
