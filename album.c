#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"album.h"

int final_alb_id(AlbumNode *head){
    if(head == NULL) return 0; //no albums, so return 0
    
    AlbumNode *temp=head;
    int max_id=0;
    while(temp != NULL){
        if(temp->album_ptr->id > max_id) max_id = temp->album_ptr->id;
        temp = temp->next;
    }
    return max_id;
}

//to create a album which is the basic element of linked list of albums - it has id,name,head linked list of song_ids
Album* create_album(int id,char* name){
    Album* new_album=(Album*)malloc(sizeof(Album));
    if(new_album == NULL){
        printf(RED "Failed to allocate memory for album\n"RESET);
        return NULL;
    }
    new_album->id=id;
    new_album->name=(char*)malloc(strlen(name)+1);
    strcpy(new_album->name,name);
    new_album->song_node=NULL;
    return new_album;
}

//to create the Node with the basic elent mentioned above
AlbumNode* create_album_node(Album* album){
    AlbumNode* album_node=(AlbumNode*)malloc(sizeof(AlbumNode));
    if(album_node == NULL) return NULL;
    album_node->album_ptr=album;
    album_node->next=NULL;
    album_node->prev=NULL;
    return album_node;
}

//insert end for the node created above
AlbumNode* insert_end(AlbumNode **head, AlbumNode **tail, AlbumNode* new_node){
    if(new_node == NULL) return *tail;

    if(*head == NULL){
        *head = new_node;
        *tail = new_node;
    } else {
        new_node->prev = *tail;
        (*tail)->next = new_node;
        *tail = new_node;
    }
    return *tail;
}

AlbumNode* find_album_serial(AlbumNode **head, int serial){
    if (serial <= 0) return NULL;
    if(*head == NULL) return NULL;

    AlbumNode *temp = *head;
    int k = serial - 1;

    while(temp != NULL && k--) temp = temp->next;
    return temp;
}

int get_serial_album_id(AlbumNode **head, int album_id){
    if (!*head) return -1;

    AlbumNode *t = *head;
    int serial = 1;

    while (t!=NULL){
        if (t->album_ptr->id == album_id) return serial;
        t = t->next;
        serial++;
    }
    return -1;
}


int album_dup_check(Album *album,int song_id){
    SongIDNode *temp=album->song_node;
    while(temp != NULL){
        if(temp->song_id==song_id) return temp->song_id;
        temp=temp->next;
    }
    return 0;
}
//used to check if a song is part of the album already

void add_song_album_direct_append(Album *album, Song *s){
    if (album == NULL || s == NULL) return;

    if (album_dup_check(album, s->id)) return;

    SongIDNode *node = malloc(sizeof(SongIDNode));
    if (!node) return;

    node->song_id = s->id;
    node->ref = s;
    node->next = NULL;

    if (album->song_node == NULL) {
        album->song_node = node;
        return;
    }

    SongIDNode *t = album->song_node;
    while (t->next != NULL) t = t->next;
    t->next = node;
}


//directly adds song to the album using the Song pointer
void add_song_album_direct(Album *album,Song *s,int boot_check){
    if(album == NULL||s == NULL) return;
    if(album_dup_check(album,s->id)){
        printf("Song '%s' already exists in album %s.\n",s->name,album->name);
        return;
    }
    SongIDNode *node=(SongIDNode*)malloc(sizeof(SongIDNode));
    if(node == NULL){
        if(!boot_check) printf(RED"Failed to allocate memory for song node.\n"RESET);
        if(boot_check) printf(RED"Failed to BOOT\n" RESET);
        return;
    }
    node->song_id=s->id;
    node->ref=s;
    node->next=album->song_node;
    album->song_node=node;
    if(!boot_check) printf("Added song '%s' by %s to album '%s'.\n",s->name,s->singer,album->name);
}

//gets the song pointer from id and then uses direct add
void add_song_album_id(Album *album,int song_id,Song *song_library){
    if(album == NULL) return;
    Song* s=find_by_id(song_library,song_id);
    if(s == NULL){
        printf("Song id %d not found in library\n",song_id);
        return;
    }
    add_song_album_direct(album,s,0);
}



//remove works similar to add
void remove_song_album_direct(Album *album,Song *s){
    if(album == NULL||album->song_node == NULL||s == NULL){
        return;
    }
    SongIDNode *temp=album->song_node,*prev=NULL;
    while(temp != NULL){
        if(temp->song_id==s->id){
            if(prev == NULL) album->song_node=temp->next; //changing head to temp.next
            else prev->next=temp->next; //skipping temp that has to be removed
            free(temp);
            printf("Removed song %s by %s from album %s\n",s->name,s->singer,album->name);
            return;
        }
        prev=temp;
        temp=temp->next;
    }
    printf("Song %s not found in album %s\n",s->name,album->name);
}

void remove_song_album_id(Album *album,int serial,Song *lib){
    if(album==NULL){
        printf(RED"Invalid album\n"RESET);
        return;
    }
    SongIDNode *t=album->song_node;
    int k=serial-1;
    if (serial <= 0) {
        printf(RED"Invalid song serial number %d\n"RESET, serial);
        return;
    }
    while(t && k--) t=t->next;
    if(t) {
        remove_song_album_direct(album,t->ref);
        printf("Removed song serial %d from album '%s'\n", serial, album->name);
    } else {
        printf("Song serial %d not found in album '%s'\n", serial, album->name);
    }
}



void remove_album_serial(AlbumNode **head, AlbumNode **tail, int serial){
    if(*head == NULL){
        printf(RED"\nCould not find the album with SNo.%d\n"RESET,serial);
        return;
    }
    AlbumNode *node = find_album_serial(head, serial);
    if(node == NULL) {
        printf(RED"\nCould not find the album with SNo.%d\n"RESET,serial);
        return;
    }

    if(node->prev != NULL) node->prev->next = node->next;
    else *head = node->next;

    if(node->next != NULL) node->next->prev = node->prev;
    else *tail = node->prev;
    printf("Removed '%s' album\n",node->album_ptr->name);

    free_album_songs(node->album_ptr->song_node);
    free(node->album_ptr->name);
    free(node->album_ptr);
    free(node);
}


void print_album(AlbumNode** head,Album *album,Song *song_library,int val,int single){
    if(album == NULL) return;
    int k=1;
    printf("\n");
    if(!single) printf("(%d) ",val);
    printf("%s\n",album->name);
    SongIDNode *temp=album->song_node;
    while(temp != NULL){
        Song* s=temp->ref;
        if(s != NULL) printf("  %d. '%s' by %s %d:%02d\n",k++,s->name,s->singer,s->duration_min,s->duration_sec);
        else printf("\tMissing songID of the song: %d\t(chefck library files)\n",temp->song_id);
        temp=temp->next;
    }
    if(k==1) printf("  No Songs in album!\n");
}

void print_all_albums(AlbumNode **head){
    if(*head == NULL){
        printf(RED"No albums Found!\n"RESET);
        return;
    }
    printf("\n");
    int val = 1;

    for(AlbumNode* temp=*head;temp!=NULL;temp=temp->next){
        printf("%d. %s\n",val,temp->album_ptr->name);
        val++;
    }
}


//adds the album from head to tail (but if a new album is added it goes to the top)
void save_album(AlbumNode **head,char *filename){
    FILE *file=fopen(filename,"w");
    if(file == NULL){
        printf("Cannot open %s\n",filename);
        return;
    }
    for(AlbumNode* temp=*head;temp!=NULL;temp=temp->next){
        Album* a=temp->album_ptr;
        fprintf(file,"%d|%s|",a->id,a->name);
        SongIDNode *s=a->song_node;
        while(s != NULL){
            fprintf(file,"%d",s->song_id);
            if(s->next != NULL) fprintf(file,",");
            s=s->next;
        }
        fprintf(file,"\n");
    }
    fclose(file);
    printf("Album saved to %s\n", filename);
}


AlbumNode* load_album(char *fname, Song *lib){
    FILE *f=fopen(fname,"r");
    if(f == NULL){
        printf("Cannot open %s\n",fname);
        return NULL;
    }
    AlbumNode *head=NULL,*tail=NULL;
    char line[512];
    while(fgets(line,sizeof(line),f)){
        int id;
        char name[256],lst[256];
        lst[0]='\0';
        if(sscanf(line,"%d|%255[^|]|%255[^\n]",&id,name,lst)<2) continue;
        Album *a=create_album(id,name);
        char *t=strtok(lst,",");
        while(t){
            int sid=atoi(t);
            Song *s=find_by_id(lib,sid);
            if (s) add_song_album_direct_append(a, s);
            t=strtok(NULL,",");
        }
        AlbumNode *n=create_album_node(a);
        tail=insert_end(&head,&tail,n);
    }
    fclose(f);
    return head;
}



void free_album_songs(SongIDNode *head){
    SongIDNode *temp=head;
    while(temp != NULL){
        SongIDNode *next=temp->next;
        free(temp);
        temp=next;
    }
}

void free_albums(AlbumNode *head){
    while(head != NULL){
        AlbumNode *next=head->next;
        free_album_songs(head->album_ptr->song_node);
        free(head->album_ptr->name);
        free(head->album_ptr);
        free(head);
        head=next;
    }
}
