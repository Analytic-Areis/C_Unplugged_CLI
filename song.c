#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"song.h"

Song* create_song(int id, char *name, char *singer, int min,int sec){
    Song* new_song=(Song*)malloc(sizeof(Song));
    if(new_song==NULL){
        printf(RED"Failed to allocate memory for song\n"RESET);
        return NULL;
    }
    new_song->id=id;
    new_song->name=(char*)malloc(strlen(name)+1);
    strcpy(new_song->name,name);
    new_song->singer=(char*)malloc(strlen(singer)+1);
    strcpy(new_song->singer,singer);
    new_song->duration_min=min;
    new_song->duration_sec=sec;
    new_song->next=NULL;
    return new_song;
}



Song* append(Song **head, Song *tail, Song *new_song){
    if(new_song == NULL) return tail;
    if(*head == NULL){
        *head = new_song;
        tail = new_song;
        return tail;
    }
    tail->next = new_song;
    tail = tail->next;
    return tail;
}

Song* load_songs(char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("file does not exist");
        return NULL;
    }

    Song *head = NULL, *tail = NULL;
    int id, total_sec;
    char name_buf[100], singer_buf[100];

    while (fscanf(file, "%d|%99[^|]|%99[^|]|%d",&id, name_buf, singer_buf, &total_sec) == 4) {
        int min = total_sec / 60;
        int sec = total_sec % 60;

        Song *new_song = create_song(id, name_buf, singer_buf, min, sec);
        if (new_song==NULL) continue;

        if (head==NULL) head = new_song;
        else tail->next = new_song;

        tail = new_song;
    }

    fclose(file);
    return head;
}

Song* load_display(char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("file does not exist");
        return NULL;
    }

    Song *head = NULL, *tail = NULL;
    int id, total_sec;
    char name_buf[100], singer_buf[100];

    while (fscanf(file, "%d|%99[^|]|%99[^|]|%d",&id, name_buf, singer_buf, &total_sec) == 4) {
        int min = total_sec / 60;
        int sec = total_sec % 60;

        Song *new_song = create_song(id, name_buf, singer_buf, min, sec);
        if (new_song==NULL) continue;

        if (head==NULL) head = new_song;
        else tail->next = new_song;

        tail = new_song;
    }

    fclose(file);
    return head;
}

Song* find_by_id(Song* head, int id){
    if(head == NULL) return NULL;
    Song* temp = head;
    while(temp != NULL) {
        if (temp->id == id) return temp;
        temp = temp->next;
    }
    return NULL;
}




void print_song(Song* s) {
    if (!s) return;
    printf("%02d  | %s | %s | %d:%02d\n", s->id, s->name, s->singer, s->duration_min, s->duration_sec);
}

void print_all_songs(Song* head) {
    printf("\n");
    for (Song* temp = head; temp!=NULL; temp = temp->next) print_song(temp);
}

void free_songs(Song* head) {
    while (head) {
        Song* n = head->next;
        free(head->name);
        free(head->singer);
        free(head);
        head = n;
    }
}
