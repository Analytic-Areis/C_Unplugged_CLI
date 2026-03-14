#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "song.h"
#include "album.h"
#include "playlist.h"
#include "cli.h"




void display_main_menu() {
    printf(BOLD MAGENTA"\n==================================\n");
    printf("        🎵 MAIN MENU 🎵\n");
    printf("==================================\n");
    printf("1. 📚 Song Library Management\n");
    printf("2. 💿 Album Management\n");
    printf("3. ▶️  Playlist Controls\n");
    printf("4. 📝 Log\n");
    printf("5. 💾 Save and Exit\n");
    printf("Enter your choice: "RESET);
}

void display_song_menu() {
    printf(YELLOW BOLD"\n--- Song Library Menu ---\n"RESET YELLOW);
    printf("1. Show All Songs\n");
    printf("2. Search Song by ID\n");
    printf("3. Back to Main Menu\n");
    printf("Enter your choice: "RESET);
}

void display_album_menu() {
    printf(YELLOW BOLD"\n--- Album Management Menu ---\n"RESET YELLOW);
    printf("1. Create New Album\n");
    printf("2. Show All Albums\n");
    printf("3. Show Album Details (by Serial)\n");
    printf("4. Add Song to Album (by Album Serial)\n");
    printf("5. Remove Song from Album (by Album Serial)\n");
    printf("6. Remove Album (by Serial)\n");
    printf("7. Back to Main Menu\n");
    printf("Enter your choice: "RESET);
}

void display_playlist_menu(int playback_unlocked) {
    printf(YELLOW BOLD"\n--- Playlist Control Menu ---\n"RESET YELLOW);

    printf("1. Show Current Playlist\n");

    printf(YELLOW BOLD"\n--- Playback ---\n"RESET YELLOW);
    printf("2. Play Current Song (or Start/Resume)\n");

    if (playback_unlocked) {
        printf("3. Next Song\n");
        printf("4. Previous Song\n");
        printf("5. Pause Song\n");
        printf("6. Stop Playlist\n");
    }

    if (!playback_unlocked) {
        printf(YELLOW BOLD"\n--- Editing ---\n"RESET YELLOW);
        printf("7. Add Song to Playlist (Next or End)\n");
        printf("8. Add Album to Playlist (Next or End)\n");
        printf("9. Remove Song (by Playlist ID)\n");
        printf("10. Clear Playlist\n");
    }
    printf(YELLOW BOLD"\n--- Exit ---\n"RESET YELLOW);
    printf("11. Back to Main Menu\n");
    printf("Enter your choice: "RESET);
}


void display_log_menu() {
    printf(YELLOW BOLD"\n--- Log Menu ---\n"RESET YELLOW);
    printf("1. Show Logs\n");
    printf("2. Clear Logs\n");
    printf("3. Back to Main Menu\n");
    printf("Enter your choice: "RESET);
}

int get_int_input() {
    int input;
    if (scanf("%d", &input) != 1){
        while (getchar() != '\n');
        return -1;
    }
    while (getchar() != '\n');
    return input;
}



int main() {
    printf("\033[2J\033[1;1H\n\n"); 
    Song *song_library_head = NULL;
    song_library_head = load_songs("songs.txt");
    if (song_library_head == NULL) {
        printf(BOLD YELLOW"--- Library is empty or failed to load. Start fresh. ---\n");
    } 
    else {
        Song *temp = song_library_head;
        while (temp->next != NULL) temp = temp->next;
        printf(BOLD YELLOW"--- Song Library loaded successfully! ---\n");
    }

    AlbumNode *album_head = load_album("albums.txt", song_library_head);
    AlbumNode *album_tail = album_head;
    if (album_head == NULL) {
        printf("--- No albums loaded. ---\n");
    } else {
        while (album_tail->next != NULL) album_tail = album_tail->next;
        printf("--- Albums loaded successfully! ---\n");
    }


    playlist *p_head = NULL;
    playlist *p_tail = NULL;
    playlist *p_current = NULL;
    int last_play_id = 0;
    
    p_head = load_playlist(&p_tail, &p_current, "playlist.txt", song_library_head, &last_play_id);
    if (p_head == NULL) {
        p_tail = NULL;
        p_current = NULL;
        last_play_id = 0;
        printf("--- No saved playlist loaded ---\n");
    } else {
        if (p_current == NULL) {
            p_current = p_head;
        }
        if (p_tail == NULL) {
            p_tail = p_head;
            while (p_tail->next) p_tail = p_tail->next;
        }
        printf("--- Playlist ready"WHITE"(last_play_id=%d)"YELLOW" ---\n", last_play_id);
    }


    printf(MAGENTA BOLD"\nWelcome to the Music Player CLI!\n"RESET);
    
    int playback_unlocked = 0;
    int choice;
    do{
        // printf(YELLOW BOLD);
        display_main_menu();
        choice = get_int_input();

        if (choice == 1) {
            int song_choice;
            do {
                display_song_menu();
                song_choice = get_int_input();

                if (song_choice == 1) cli_show_all_songs(song_library_head);
                else if (song_choice == 2) cli_search_song_by_id(song_library_head);
                else if (song_choice == 3) printf("Returning to Main Menu...\n");
                else printf(RED"Invalid choice. Try again.\n"RESET);

            } while (song_choice != 3);
        }

        else if (choice == 2) {
            int album_choice;
            do {
                display_album_menu();
                album_choice = get_int_input();

                if (album_choice == 1) cli_create_album(&album_head, &album_tail);
                else if (album_choice == 2) cli_show_all_albums(&album_head, song_library_head);
                else if (album_choice == 3) cli_show_album(&album_head, song_library_head);
                else if (album_choice == 4) cli_album_add_song(&album_head, song_library_head);
                else if (album_choice == 5) cli_album_remove_song(&album_head, song_library_head);
                else if (album_choice == 6) cli_remove_album(&album_head, &album_tail);
                else if (album_choice == 7) printf("Returning to Main Menu...\n");
                else printf(RED"Invalid choice. Try again.\n"RESET);

            } while (album_choice != 7);
        }

        else if (choice == 3) {
            int playlist_choice;
            do {
                display_playlist_menu(playback_unlocked);
                playlist_choice = get_int_input();

                if (playlist_choice == 1) {
                    cli_playlist_show_all(p_head, p_current);
                } 
                else if (playlist_choice == 2) {
                    if (!p_current && p_head) p_current = p_head;
                    if (p_current) {
                        if (cli_playlist_play(&p_current)) playback_unlocked = 1;
                    } 
                    else printf(RED"Playlist empty or current cannot be set.\n"RESET);
                }
                else if (!playback_unlocked && (playlist_choice == 3 ||playlist_choice == 4 ||playlist_choice == 5 ||playlist_choice == 6)) {
                    printf("Start playback first using option 2!\n");
                }
                else if (playlist_choice == 3) cli_playlist_play_next(&p_current, p_head);
                else if (playlist_choice == 4) cli_playlist_play_prev(&p_current, p_tail);
                else if (playlist_choice == 5) cli_playlist_pause(p_current);
                else if (playlist_choice == 6) {playback_unlocked=0;cli_playlist_stop(&p_current);}
                else if (!playback_unlocked && playlist_choice == 7) cli_playlist_song(&p_head, &p_tail, &p_current, song_library_head, &last_play_id);
                else if (!playback_unlocked && playlist_choice == 8) cli_playlist_add_album(&p_head, &p_tail, &p_current, song_library_head, &album_head, &last_play_id);
                else if (!playback_unlocked && playlist_choice == 9) cli_playlist_remove_song(&p_head, &p_tail);
                else if (!playback_unlocked && playlist_choice == 10) cli_playlist_clear(&p_head,&p_tail,&p_current,&last_play_id);

                else if (playlist_choice >= 7 && playlist_choice <= 10 && playback_unlocked) {
                    printf(RED"Cannot edit playlist while playing!"RESET"Stop playback first.\n");
                }

                else if (playlist_choice == 11) printf("Returning to Main Menu...\n");
                else printf(RED "Invalid choice. Try again.\n"RESET);
            } while (playlist_choice != 11);
        }

        else if (choice == 4) {
            int log_choice;
            do {
                display_log_menu();
                log_choice = get_int_input();

                if (log_choice == 1) {
                    log_msg("User viewed logs\n");
                    show_logs();
                }
                else if (log_choice == 2) {
                    log_msg("Logs cleared by user\n");
                    clear_logs();
                    printf("Logs cleared.\n");
                }
                else if (log_choice == 3) {
                    printf("Returning to Main Menu...\n");
                }
                else {
                    printf(RED "Invalid choice. Try again.\n"RESET);
                }

            } while (log_choice != 3);
        }

        else if (choice == 5) {
            printf(MAGENTA BOLD"\n--- Saving data and shutting down... ---\n");
        }

        else {
            printf(RED "Invalid choice. Enter a number between 1 and 5.\n"RESET);
        }

    } while (choice != 5);

    save_playlist(p_head, p_current, "playlist.txt");
    clear_playlist(&p_head);
    p_tail = NULL;
    p_current = NULL;
    save_album(&album_head, "albums.txt");
    free_albums(album_head);
    free_songs(song_library_head);

    printf("Goodbye!\n"RESET);
    return 0;
}
