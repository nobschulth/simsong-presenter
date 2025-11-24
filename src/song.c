#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "song.h"
#include "utils.h"

Song* Song_CreateFromString(const char* string) {
    if (!string) return NULL;
    Song* song = safe_malloc(sizeof(Song));
    song->elementCount = 0;
    //search for the name
    for (int i = 0; i > -1; i++) {
        if (string[i] != '\0' && string[i] == '\n' && string[i + 1] == '\n') {
            song->title = strndup(string, i);
            string += i + 2;
            break;
        } else if (string[i] == '\0') {
            return NULL;
        }
    }
    //search for the elements
    for (int i = 0; i > -1; i++) {
        bool stringEnds = string[i] == '\0' || string[i + 1] == '\0' || string[i + 2] == '\0';
        bool bookSeperation = !stringEnds && string[i] == '&' && string[i + 1] == '&' && string[i + 2] == '&' && string[i - 1] == '\n';
        if (string[i] != '\0' && string[i] == '\n' && string[i + 1] == '\n' && !bookSeperation) {
            if (song->elementCount == 0) {
                song->elementCount++;
                song->elements = safe_malloc(sizeof(SongElement));
            } else {
                song->elementCount++;
                song->elements = safe_realloc(song->elements, sizeof(SongElement) * song->elementCount);
            }
            
            //add the title
            for (int j = 0; j > -1; j++) {
                if (string[j] == '\n') {
                    song->elements[song->elementCount - 1].name = strndup(string, j);
                    string += j + 1;
                    i -= j + 1;
                    break;
                }
            }
            
            song->elements[song->elementCount - 1].text = strndup(string, i);
            string += i + 2;
            i = 0;
        } else if (string[i] == '\0' || bookSeperation) {
            song->licence = strndup(string, i);
            break;
        }
    }

    return song;
}

void Song_free(Song* song) {
    for (int i = 0; i < song->elementCount; i++) {
        free(song->elements[i].text);
        free(song->elements[i].name);
    }
    free(song->elements);
    free(song->licence);
    free(song->title);
    free(song);
}

Book* Book_CreateFromString(const char* string) {
    if (!string) return NULL;
    Book* book = safe_malloc(sizeof(Book));
    book->songCount = 0;

    int offset = 0;
    while (true) {
        if (book->songCount == 0) {
            book->songs = safe_malloc(sizeof(Song*));
        } else {
            book->songs = safe_realloc(book->songs, sizeof(Song*) * (book->songCount + 1));
        }
        book->songCount++;

        book->songs[book->songCount - 1] = Song_CreateFromString(string + offset);
        char* ptr = strstr(string + offset, "\n&&&");
        if (!ptr) break;
        offset = ptr - string + 5;
    }

    return book;
}

void Book_free(Book* book) {
    for (int i = 0; i < book->songCount; i++) {
        Song_free(book->songs[i]);
    }
    free(book->songs);
    free(book);
}
