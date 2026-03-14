#ifndef CLI_H
#define CLI_H
#include "song.h"
#include "album.h"
#include "playlist.h"
#include "log.h"

//songs cli
void cli_show_all_songs(Song *library);
void cli_search_song_by_id(Song *library);

// album cli
void cli_create_album(AlbumNode **head, AlbumNode **tail);
void cli_show_all_albums(AlbumNode **head, Song *library);
void cli_show_album(AlbumNode **head, Song *library);
void cli_remove_album(AlbumNode **head, AlbumNode **tail);
void cli_album_add_song(AlbumNode **head, Song *library);
void cli_album_remove_song(AlbumNode **head, Song *library);

// playlist
void cli_playlist_song(playlist** head, playlist** tail,playlist** current,Song* library,int* last);
void cli_playlist_add_song_end(playlist** head, playlist** tail, Song* library,int *last);
void cli_playlist_add_song_next(playlist** head, playlist** tail, playlist** current, Song* library, int* last_play_id); // Corrected function for adding next song
void cli_playlist_add_album(playlist** head, playlist** tail, playlist** current, Song* library, AlbumNode** head_album, int* last_play_id);
void cli_playlist_remove_song(playlist** head, playlist** tail);
void cli_playlist_clear(playlist** head, playlist** tail, playlist** current, int* last_play_id);
void cli_playlist_show_current(playlist* current);
void cli_playlist_play_next(playlist** current, playlist* head);
void cli_playlist_play_prev(playlist** current, playlist* tail);
void cli_playlist_pause(playlist* current);
int cli_playlist_play(playlist** current);
void cli_playlist_stop(playlist** current);
void cli_playlist_show_all(playlist* head, playlist* current);
#endif
