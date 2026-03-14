#ifndef PLAYLIST_H
#define PLAYLIST_H
#include "song.h"
#include "album.h"
#include "log.h"

typedef struct playlist{
    int play_id;
    Song *ref;
    struct playlist *next;
    struct playlist *prev;
}playlist;

playlist* create_playlist_node(int play_id,Song *ref);
playlist* add_song_to_end(playlist **head,playlist **tail,int play_id,Song *ref,int album_flag);
playlist* add_song_next(playlist **head,playlist **tail,playlist *current,int play_id,Song *ref,int album_flag);
void add_album_to_playlist(playlist **head, playlist **tail, playlist **current,Album *album, Song *library, int *last_play_id, int next_end);
void remove_song(playlist **head,playlist **tail,int play_id);
void play_next(playlist **current,playlist *head);
void play_prev(playlist **current,playlist *tail);
int play(playlist **current);
void pause_song(playlist *current);
void stop_song(playlist **current);
void show_current(playlist *current);
void print_playlist_with_current(playlist *head,playlist *current);
void clear_playlist(playlist **head);
void save_playlist(playlist *head,playlist *current,char *filename);
playlist* load_playlist(playlist **tail,playlist **current,char *filename,Song *library,int *last_play_id);

#endif
