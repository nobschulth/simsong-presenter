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
        if (string[i] != '\0' && string[i] == '\n' && string[i + 1] == '\n') {
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
        } else if (string[i] == '\0') {
            song->licence = strndup(string, i);
            break;
        }
    }

    printf("------\nSonginfo:\n\n");
    printf("Titel: \"%s\"\n\n", song->title);
    for (int i = 0; i < song->elementCount; i++) {
        printf("Name: \"%s\"\n", song->elements[i].name);
        printf("Text: \"%s\"\n\n", song->elements[i].text);
    }
    printf("Credits: \"%s\"\n", song->licence);

    return song;
}

Song* Song_free(Song* song) {
    for (int i = 0; i < song->elementCount; i++) {
        free(song->elements[i].text);
        free(song->elements[i].name);
    }
    free(song->elements);
    free(song->licence);
    free(song->title);
    free(song);
}
