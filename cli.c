#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"cli.h"

void cli_show_all_songs(Song *library){
    if(library == NULL){
        printf("No songs in library.\n");
        return;
    }
    log_msg("Printed All Songs\n");
    print_all_songs(library);
}

void cli_search_song_by_id(Song *library){
    int id;
    printf("Enter Song Serial No.: ");
    scanf("%d",&id);

    Song *s=find_by_id(library,id);
    if(s == NULL){
        printf("Song with S No.%d not found\n",id);
        log_msg(RED"Searched an invalid song\n"RESET);
        return;
    }
    char buf[256];
    sprintf(buf, "Searched Song: %d by %s\n", s->id, s->singer);
    log_msg(buf);
    print_song(s);
}

void cli_create_album(AlbumNode **head, AlbumNode **tail){
    char name[256];
    printf("\nEnter album name: ");
    scanf(" %[^\n]",name);

    int id = final_alb_id(*head);
    id++;

    Album *a = create_album(id,name);
    AlbumNode *n = create_album_node(a);
    insert_end(head,tail,n);

    printf("Created album %s (id: %d)\n",name,id);
    char buf[256];
    sprintf(buf, "Created album %s (id: %d)\n",name,id);
    log_msg(buf);
}


void cli_show_all_albums(AlbumNode **head, Song *library){
    print_all_albums(head);
    log_msg("Printed All Albums\n");

}

void cli_show_album(AlbumNode **head, Song *library){
    if(*head == NULL){
        printf("No Albums Found!\n");
        return;
    }
    print_all_albums(head);
    int serial;
    printf("\nEnter album serial number: ");
    scanf("%d",&serial);

    AlbumNode *n = find_album_serial(head,serial);
    if(n == NULL){
        printf(RED"Invalid serial number\n"RESET);
        return;
    }
    print_album(head,n->album_ptr,library,1,1);
    char buf[256];
    sprintf(buf, "Printed Album details %s\n",n->album_ptr->name);
    log_msg(buf);
}

void cli_remove_album(AlbumNode **head, AlbumNode **tail){
    if(*head == NULL){
        printf("No Albums Found!\n");
        return;
    }
    print_all_albums(head);
    int serial;
    printf("\nEnter album serial number to remove: ");
    scanf("%d",&serial);

    remove_album_serial(head,tail,serial);
    char buf[256];
    sprintf(buf, "Removed Album with SNo. %d\n",serial);
    log_msg(buf);
}

void cli_album_add_song(AlbumNode **head, Song *library){
    if(*head == NULL){
        printf("No Albums Found!\n");
        return;
    }
    print_all_albums(head);
    int serial, song_id;
    printf("\nEnter album SNo.: ");
    scanf("%d",&serial);

    AlbumNode *alb = find_album_serial(head,serial);
    if(alb == NULL){
        printf(RED"Invalid album serial number\n"RESET);
        return;
    }
    print_all_songs(library);
    printf("\nEnter song SNo.: ");
    scanf("%d",&song_id);

    add_song_album_id(alb->album_ptr,song_id,library);
    char buf[256];
    sprintf(buf, "Added song id %d to album SNo.%s\n",song_id,alb->album_ptr->name);
    log_msg(buf);
}

void cli_album_remove_song(AlbumNode **head, Song *library){
    if(*head == NULL){
        printf("No Albums Found!\n");
        return;
    }
    print_all_albums(head);
    int serial, s_num;
    printf("Enter album serial: ");
    scanf("%d",&serial);

    AlbumNode *alb = find_album_serial(head,serial);
    if(alb == NULL){
        printf(RED"Invalid album serial\n"RESET);
        return;
    }

    if(alb->album_ptr == NULL){
        printf(RED"Error: album data missing\n"RESET);
        return;
    }

    if(alb->album_ptr->song_node == NULL){
        printf("This album is already empty\n");
        return;
    }

    print_album(head,alb->album_ptr,library,1,1);

    printf("Enter song serial number inside album: ");
    scanf("%d",&s_num);
    printf("\n");
    remove_song_album_id(alb->album_ptr,s_num,library);
    char buf[256];
    sprintf(buf, "Removed Song %d from Album with SNo. %d\n",s_num,serial);
    log_msg(buf);
}


void cli_playlist_song(playlist** head, playlist** tail,playlist** current,Song* library,int* last){
    int k;
    printf("Enter 0 to add song to next / 1 to add song to end:");
    scanf("%d",&k);
    print_all_songs(library);
    printf("\n");
    if(k) cli_playlist_add_song_end(head,tail,library,last);
    else cli_playlist_add_song_next(head,tail,current,library,last);
}

void cli_playlist_add_song_end(playlist** head, playlist** tail, Song* library, int* last_play_id){
    int song_id;
    printf("Enter the SNo. which you want to add at end: ");
    scanf("%d", &song_id);

    Song* s = find_by_id(library, song_id);
    if (!s) {
        printf("Song id %d not found in library\n", song_id);
        return;
    }

    (*last_play_id)++;
    add_song_to_end(head, tail, *last_play_id, s, 0);
    char buf[256];
    sprintf(buf, "Added Song %d at the end of playlist\n",song_id);
    log_msg(buf);
}

void cli_playlist_add_song_next(playlist** head, playlist** tail, playlist** current, Song* library, int* last_play_id){
    int song_id;
    printf("Enter song id to add to play next: ");
    scanf("%d", &song_id);

    Song* s = find_by_id(library, song_id);
    if (s == NULL) {
        printf("Song id %d not found in library\n", song_id);
        return;
    }
    (*last_play_id)++;
    add_song_next(head, tail, *current, *last_play_id, s,0);
    char buf[256];
    sprintf(buf, "Added Song %d at the next of playlist\n",song_id);
    log_msg(buf);
}

void cli_playlist_add_album(playlist** head, playlist** tail, playlist** current, Song* library, AlbumNode** head_album, int* last_play_id){
    if(*head_album == NULL){
        printf("No Albums available!\n");
        return;
    }
    print_all_albums(head_album);
    int serial;
    int choice;

    printf("Enter album serial number to add: ");
    scanf("%d", &serial);
    AlbumNode* alb_node = find_album_serial(head_album, serial);
    if (alb_node == NULL) {
        printf("Album serial %d not found\n", serial);
        return;
    }
    printf("Add album '%s':\n", alb_node->album_ptr->name);
    printf("  Enter 0 to add to the END.\n");
    printf("  Enter 1 to add to play NEXT.\n");
    printf("Choice: ");
    scanf("%d", &choice);
    add_album_to_playlist(head, tail,current,alb_node->album_ptr,library,last_play_id,choice);
    char buf[256];
    sprintf(buf, "Added Album '%s' at the end of playlist\n",alb_node->album_ptr->name);
    log_msg(buf);
}

void cli_playlist_remove_song(playlist** head, playlist** tail) {
    if(*head==NULL){
        printf("No Songs in playlist!\n");
        return;
    }
    int serial;
    printf("Enter playlist SNo. to remove: ");
    scanf("%d", &serial);
    remove_song(head, tail, serial);
    char buf[256];
    sprintf(buf, "Removed Song SNo.%d at the end of playlist\n",serial);
    log_msg(buf);
}

void cli_playlist_clear(playlist** head, playlist** tail, playlist** current, int* last_play_id) {
    int conf;
    printf("Are you sure that you want to clear the playlist? (1 - yes / 0 - no)\n");
    scanf("%d",&conf);
    if(conf){
        clear_playlist(head);
        *head = NULL;
        *tail = NULL;
        *current = NULL;
        *last_play_id = 0;
        printf("Playlist cleared\n");
        char buf[256];
        sprintf(buf, "Clear playlist\n");
        log_msg(buf);
    }
}

void cli_playlist_show_current(playlist* current) {
    show_current(current);
    log_msg("Showed Current\n");
}

int cli_playlist_play(playlist** current){
    play(current);
    log_msg("Started Playback\n");
    return 1;
}

void cli_playlist_play_next(playlist** current, playlist* head) {
    play_next(current, head);
    log_msg("Played Next\n");
}

void cli_playlist_play_prev(playlist** current, playlist* tail) {
    play_prev(current, tail);
    log_msg("Played Previous\n");
}

void cli_playlist_pause(playlist* current) {
    pause_song(current);
    log_msg("Paused Song\n");
}

void cli_playlist_stop(playlist** current) {
    stop_song(current);
    log_msg("Stopped Playing\n");
}

void cli_playlist_show_all(playlist* head, playlist* current) {
    print_playlist_with_current(head, current);
    log_msg("Printed Playlist\n");
}
