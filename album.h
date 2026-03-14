#ifndef ALBUM_H
#define ALBUM_H
#include"song.h"
#include "log.h"

typedef struct SongIDNode{
    int song_id;
    Song *ref;
    struct SongIDNode *next;
}SongIDNode;

typedef struct Album{
    int id;
    char *name;
    SongIDNode *song_node;
}Album;

typedef struct AlbumNode{
    Album *album_ptr;
    struct AlbumNode *next;
    struct AlbumNode *prev;
}AlbumNode;

int final_alb_id(AlbumNode *head);
Album* create_album(int id,char* name);
AlbumNode* create_album_node(Album* album);
AlbumNode* insert_end(AlbumNode **head, AlbumNode **tail, AlbumNode* new_node);

AlbumNode* find_album_serial(AlbumNode **head, int serial);
int get_serial_album_id(AlbumNode **head, int album_id);

int album_dup_check(Album *album,int song_id);

void add_song_album_direct(Album *album,Song *s,int boot_check);
void add_song_album_id(Album *album,int song_id,Song *song_library);
void add_song_album_direct_append(Album *album, Song *s);

void remove_song_album_direct(Album *album,Song *s);
void remove_song_album_id(Album *album,int song_id,Song *song_library);

void remove_album_serial(AlbumNode **head, AlbumNode **tail, int serial);

void print_album(AlbumNode** head,Album *album,Song *song_library,int val,int single);
void print_all_albums(AlbumNode **head);

void save_album(AlbumNode **head,char *filename);
AlbumNode* load_album(char *filename,Song *song_library);

void free_album_songs(SongIDNode *head);
void free_albums(AlbumNode *head);

#endif
