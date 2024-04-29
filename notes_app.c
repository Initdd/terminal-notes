
/**
 * Notes app
 * 
 * This app is used to create, edit and delete notes.
 * It is a "graphical" wrapper for the notes library.
 * 
 * The notes are stored in a file, each note being separated by a delimiter.
 * 
*/

// Standard libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <signal.h>

// Local libraries
#include "./notes_manager/notes_manager.h"
#include "./window_manager/window_manager.h"
#include "./input_handler/main_input_handler.c"

// Constants
// file path for the notes
#define DATA "./data"
// format of the notes in the file
#define DEL '-'

// Global variables
volatile sig_atomic_t quit_flag = 1;
NoteList *note_list;

// utility variables
int priority = 0;
char *data = NULL; // will be used to make mallocs/reallocs/frees
char *group = NULL; // will be used to make mallocs/reallocs/frees
int note_id = 0;
char **group_list = NULL;
int group_list_size = 0;
NoteList *group_notes_list = NULL;

// Windows
WINDOW *menu_win;
WINDOW *main_note_win;

// debug 
#define TERMINAL "/dev/pts/4"
void print_to_terminal(const char* pat, void *msg) {
    FILE* term = fopen(TERMINAL, "w");
    if (term == NULL) {
        perror("Failed to open terminal");
        return;
    }

    fprintf(term, pat, msg);
    fclose(term);
}

// handle signal ctrl c
void end_app() {
    notes_list_save(note_list, DATA, DEL);
    notes_list_delete(note_list);
    // free the data
    if (data != NULL) {
        free(data);
        data = NULL;
    }
    // free the group
    if (group != NULL) {
        free(group);
        group = NULL;
    }
    // free the group list
    if (group_list != NULL) {
        for (int i = 0; i < group_list_size; ++i) {
            free(group_list[i]);
            group_list[i] = NULL;
        }
        free(group_list);
        group_list = NULL;
    }
    // free the group notes list
    if (group_notes_list != NULL) {
        // Free the notes. Only need to free the list
        // The notes are freed when deleting the note list
        free(group_notes_list->list);
        free(group_notes_list);
        group_notes_list = NULL;
    }
    // close the windows
    delwin(menu_win);
    delwin(main_note_win);
    // close ncurses
    curs_set(1);
    endwin();
    exit(0);
}
void sigintHandler(int sig_num) {
    signal(SIGINT, sigintHandler);
    quit_flag = 0;
    end_app();
}

// Basic display functions
void init_display() {
    signal(SIGINT, sigintHandler);
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    start_color();
    use_default_colors();
    // Define color pairs
    // id, foreground, background
    // priorities
    init_pair(1, COLOR_WHITE, -1); // 1
    init_pair(2, COLOR_YELLOW, -1); // 2
    init_pair(3, COLOR_RED, -1); // 3
    init_pair(4, COLOR_BLACK, COLOR_WHITE); // 1 selected
    init_pair(5, COLOR_BLACK, COLOR_YELLOW); // 2 selected
    init_pair(6, COLOR_WHITE, COLOR_RED); // 3 selected
    // buttons
    init_pair(7, COLOR_WHITE, COLOR_RED); // delete button selected
    init_pair(8, COLOR_RED, -1); // delete button not selected
    init_pair(9, COLOR_WHITE, COLOR_GREEN); // submit button selected
    init_pair(10, COLOR_GREEN, -1); // submit button not selected
    init_pair(11, COLOR_WHITE, COLOR_RED); // exit button selected
    init_pair(12, COLOR_RED, -1); // exit button not selected
}

/**
 * Arguments:
 *  -l: list notes
*/

// Main function
int main(int argc, char *argv[]) {
    // Notes
    note_list = notes_list_load(DATA, DEL);
    // check if the notes were loaded successfully
    if (note_list == NULL) {
        printf("Error: Failed to load the notes: Data file not found\n");
        return 1;
    }

    // check if the user wants to list the notes
    if (argc > 1) {
        if (strcmp(argv[1], "-l") == 0) {
            for (int i = 0; i < note_list->size; ++i) {
                char *color_bgn;
                char *color_end;
                switch (note_list->list[i]->prt) {
                    case 1:
                        color_bgn = "\033[0;37m";
                        color_end = "\033[0m";
                        break;
                    case 2:
                        color_bgn = "\033[0;33m";
                        color_end = "\033[0m";
                        break;
                    case 3:
                        color_bgn = "\033[0;31m";
                        color_end = "\033[0m";
                        break;
                    default:
                        color_bgn = "\033[0;37m";
                        color_end = "\033[0m";
                        break;
                }
                printf("%s%d: %s%s\n", color_bgn, note_list->list[i]->prt, note_list->list[i]->data, color_end);
            }
            notes_list_delete(note_list);
            return 0;
        }
    }
    
    // Initialize the display
    init_display();

    // selected window (0 - menu, 1 - main window)
    int selected_window = 0;
    // when in main window mode 0, this is the selected item
    int selected_idx = 0;
    // window modes:
    // 0 - list notes, 1 - new note, 2 - edit note
    int main_window_mode = 0;
    // Menu
    // options: list groups, list notes, new note, exit
    char *menu_options[] = {"List Groups", "List Notes", "New Note", "Exit"};
    int selected_option = 0;

    // Windows
    menu_win = newwin(MENU_WINDOW_HEIGHT, MENU_WINDOW_WIDTH, MARGIN, MARGIN);
    main_note_win = newwin(MAIN_WINDOW_HEIGHT, MAIN_WINDOW_WIDTH, MARGIN, MENU_WINDOW_WIDTH + (MARGIN * 2));
    refresh();

    // Main loop
    while (quit_flag) {
        draw_menu_window(menu_win, selected_option, menu_options, sizeof(menu_options) / sizeof(menu_options[0]), selected_window == 0);
        draw_main_window(main_note_win, note_list, &selected_idx, &main_window_mode, selected_window == 1, &priority, &data, &group, group_list_size, group_list, group_notes_list);
        quit_flag = handle_input(&selected_option, sizeof(menu_options) / sizeof(menu_options[0]), &selected_window,
                    2, &main_window_mode, &selected_idx, note_list, &priority, &data, &note_id, &group, &group_list, &group_list_size, &group_notes_list);
    }

    // close windows
    delwin(menu_win);
    delwin(main_note_win);

    end_app();
}
