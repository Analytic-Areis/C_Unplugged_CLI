#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "playlist.h"

playlist* create_playlist_node(int play_id, Song* ref) {
    playlist* n = (playlist*)malloc(sizeof(playlist));
    if (n == NULL) return NULL;
    n->play_id = play_id;
    n->ref = ref;
    n->next = NULL;
    n->prev = NULL;
    return n;
}

playlist* add_song_to_end(playlist** head, playlist** tail, int play_id, Song* ref, int album_flag) {
    playlist* n = create_playlist_node(play_id, ref);
    if (n == NULL) return *tail;

    if (*head == NULL){
        *head = n;
        *tail = n;
    }
    else{
        (*tail)->next = n;
        n->prev = *tail;
        *tail = n;
    }
    if(!album_flag) printf("Added to end: %s by %s\n", ref->name, ref->singer);
    
    return *tail;
}

playlist* add_song_next(playlist** head, playlist** tail, playlist* current, int play_id, Song* ref, int album_flag) {
    playlist* n = create_playlist_node(play_id, ref);
    if (n == NULL) return *tail;

    if (current == NULL) {
        if (*head == NULL) {
            *head = n;
            *tail = n;
        } else {
            n->next = *head;
            (*head)->prev = n;
            *head = n;
        }

        if (!album_flag) printf("Added to front: %s by %s\n", ref->name, ref->singer);
        return *tail;
    }

    if (current->next == NULL) {
        current->next = n;
        n->prev = current;
        *tail = n;
    } else {
        n->next = current->next;
        n->prev = current;
        current->next->prev = n;
        current->next = n;
    }
    if(!album_flag) printf("Added to next: %s by %s\n", ref->name, ref->singer);
    return *tail;
}

void add_album_to_playlist(playlist **head, playlist **tail, playlist **current, Album *album, Song *library, int *last_play_id,int next_end) {
    if (album == NULL) {
        printf("No album provided\n");
        return;
    }
    if (album->song_node == NULL) {
        printf("Album '%s' has no songs\n", album->name);
        return;
    }

    SongIDNode *snode = album->song_node;
    int added_count = 0;

    if (next_end) {
        while (snode != NULL) {
            Song *s = find_by_id(library, snode->song_id);
            if (s != NULL) {
                (*last_play_id)++;
                *tail = add_song_to_end(head, tail, *last_play_id, s, 1);
                added_count++;
            }
            snode = snode->next;
        }
    } else {
        playlist* insert_pos = *current;
        if (insert_pos == NULL) {
            while (snode != NULL) {
                Song *s = find_by_id(library, snode->song_id);
                if (s != NULL) {
                    (*last_play_id)++;
                    *tail = add_song_to_end(head, tail, *last_play_id, s, 1);
                    added_count++;
                }
                snode = snode->next;
            }
        } else {
            while (snode != NULL) {
                Song *s = find_by_id(library, snode->song_id);
                if (s != NULL) {
                    (*last_play_id)++;
                    playlist* new_node = create_playlist_node(*last_play_id, s);
                    if (new_node == NULL) { snode = snode->next; continue; }

                    new_node->next = insert_pos->next;
                    new_node->prev = insert_pos;
                    insert_pos->next = new_node;
                    if (new_node->next) new_node->next->prev = new_node;
                    else *tail = new_node;

                    insert_pos = new_node;
                    added_count++;
                }
                snode = snode->next;
            }
        }
    }

    if (added_count > 0) {
        if (next_end) printf("Added %d songs from album '%s' to end of playlist\n", added_count, album->name);
        else printf("Added %d songs from album '%s' to play next\n", added_count, album->name);
    } 
    else printf("No valid songs found in album '%s'\n", album->name);
}



void remove_song(playlist** head, playlist** tail, int serial) {
    if (*head == NULL) {
        printf("Playlist is empty!!\n");
        return;
    }
    int k=1;
    playlist* t = *head;
    while (t!=NULL) {
        if (k==serial) {
            printf("Removed: [%d] %s by %s\n", t->play_id, t->ref->name, t->ref->singer);
            if (t->prev != NULL) t->prev->next = t->next;
            else *head = t->next;
            if (t->next != NULL) t->next->prev = t->prev;
            else *tail = t->prev;
            free(t);
            printf("\n**** Current Playlist: ****\n");
            print_playlist_with_current(*head, NULL);
            return;
        }
        k++;
        t = t->next;
    }
    printf("Song with playlist SNo.%d is not been found\n", serial);
}

int play(playlist** current){
    if(current == NULL || *current == NULL) {
        printf("\nPlaylist is empty\n");
        return 0;
    }
    Song *s=(*current)->ref;
    printf("\n=> Playing: %s by %s\n",s->name,s->singer);
    return 1;
}

void play_next(playlist **current,playlist *head){
    if(head == NULL) {
        printf("Playlist is empty\n");
        return;
    }
    if(*current==NULL){
        *current = head;
        play(current);
        return;
    }
    if((*current)->next==NULL) *current=head;
    else *current=(*current)->next;
    Song *s=(*current)->ref;
    printf("\n=> Now playing %s by %s\n",s->name,s->singer);
}

void play_prev(playlist **current,playlist *tail){
    if(tail == NULL) {
        printf("Playlist is empty\n");
        return;
    }
    if(*current==NULL){
        *current = tail;
        Song *s = (*current)->ref;
        printf("\n=> Now playing %s by %s\n",s->name,s->singer);
        return;
    }
    if((*current)->prev==NULL) *current=tail;
    else *current=(*current)->prev;
    Song *s=(*current)->ref;
    printf("\n=> Now playing %s by %s\n",s->name,s->singer);
}

void pause_song(playlist *current){
    if(current==NULL){
        printf("Playlist is empty\n");
        return;
    }
    Song *s=current->ref;
    printf("* Paused %s by %s\n",s->name,s->singer);
}

void stop_song(playlist **current){
    if(*current==NULL){
        printf("! Playlist is empty !\n");
        return;
    }
    printf("\n! Stopped the playlist !\n");
    *current=NULL;
}

void show_current(playlist* current) {
    if (current == NULL) {
        printf("Nothing is currently playing\n");
        return;
    }
    Song* s = current->ref;
    printf("Currently playing: %s by %s [%d:%02d]\n", s->name, s->singer, s->duration_min, s->duration_sec);
}


void print_playlist_with_current(playlist* head, playlist* current) {
    if (head == NULL) {
        printf("! Playlist is empty !\n");
        return;
    }

    printf("\n******** displaying playlist: ******** \n");
    playlist* temp = head;
    int l=1;
    while (temp!=NULL) {
        Song* s = temp->ref;
        if (temp == current) printf("\n=>  Now playing (%d) %s | %s | %d:%02d  <=\n",l, s->name, s->singer, s->duration_min, s->duration_sec);
        else printf("(%d) %s | %s | %d:%02d\n",l, s->name, s->singer, s->duration_min, s->duration_sec);
        l++;
        temp = temp->next;
    }
}



void clear_playlist(playlist** head) {
    playlist* temp1 = *head;
    while (temp1 != NULL) {
        playlist* temp2 = temp1->next;
        free(temp1);
        temp1 = temp2;
    }
    *head = NULL;
}


void save_playlist(playlist* head, playlist* current, char* filename) {
    FILE* f = fopen(filename, "w");
    if (!f) {
        printf("Cannot open %s for writing\n", filename);
        return;
    }

    if (current == NULL) fprintf(f, "-1\n");
    else fprintf(f, "%d\n", current->play_id);

    playlist* t = head;
    while (t) {
        fprintf(f, "%d|%d", t->play_id, t->ref->id);
        if (t->next) fprintf(f, ",");
        t = t->next;
    }

    fprintf(f, "\n");
    fclose(f);
    printf("Playlist saved to %s\n", filename);
}

playlist* load_playlist(playlist** tail,playlist** current,char* filename,Song* library,int* last_play_id){
    FILE* f=fopen(filename,"r");
    if(!f){
        printf("No saved playlist found\n");
        *last_play_id=0;
        return NULL;
    }
    char line[512];
    int current_pid;
    playlist* head=NULL;
    if(fgets(line,sizeof(line),f)) sscanf(line,"%d",&current_pid);
    if(fgets(line,sizeof(line),f)){
        char* token=strtok(line,",");
        while(token){
            int pid=0,sid=0;
            sscanf(token,"%d|%d",&pid,&sid);
            Song* s=find_by_id(library,sid);
            if(s!=NULL) *tail=add_song_to_end(&head,tail,pid,s,1);
            *last_play_id=pid;
            token=strtok(NULL,",");
        }
    }
    fclose(f);
    *current=head;
    while(*current != NULL && (*current)->play_id != current_pid) *current=(*current)->next;
    if (*current == NULL && head != NULL) *current = head;
    printf("Playlist loaded from %s\n",filename);
    return head;
}
