#ifndef SONG_H
#define SONG_H

#include"log.h"

#define RED            "\033[31m"
#define MAGENTA        "\033[35m"
#define YELLOW         "\033[33m"
#define WHITE          "\033[37m"
#define BOLD           "\033[1m"
#define RESET          "\033[0m"

typedef struct Song{
    int id;
    char* name;
    char* singer;
    int duration_min;
    int duration_sec;
    struct Song* next;
}Song;


Song* create_song(int id, char *name, char *singer, int duration_min,int duration_sec);
Song* append(Song** h, Song* tail, Song* new_song);

Song* load_songs(char* filename);
Song* load_display(char *filename);

Song* find_by_id(Song* h, int id);

void print_song(Song* song);
void print_all_songs(Song* h);

void free_songs(Song* h);

#endif