
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
#include <ncurses.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

// Local libraries
#include "./input_handler/main_input_handler.c"
#include "./notes_manager/notes_manager.h"
#include "./window_manager/window_manager.h"

// Constants
// file path for the notes
#ifdef DEVMODE
#define DATA_DIR "./"
#define DATA DATA_DIR "data"
#else
#define DATA_DIR "%s/.local/share/notes_app/"
#define DATA "data"
#endif

// format of the notes in the file
#define DEL '-'

// Global variables
volatile sig_atomic_t quit_flag = 1;
NoteList *note_list;
char *data_dir;

// utility variables
int priority = 0;
char *data = NULL;   // will be used to make mallocs/reallocs/frees
char *group = NULL;  // will be used to make mallocs/reallocs/frees
int note_id = 0;
char **group_list = NULL;
int group_list_size = 0;
NoteList *group_notes_list = NULL;

// Windows
WINDOW *menu_win;
WINDOW *main_note_win;

// debug
#define TERMINAL "/dev/pts/4"
void print_to_terminal(const char *pat, void *msg) {
    FILE *term = fopen(TERMINAL, "w");
    if (term == NULL) {
        perror("Failed to open terminal");
        return;
    }

    fprintf(term, pat, msg);
    fclose(term);
}

// handle signal ctrl c
void end_app() {
    notes_list_save(note_list, data_dir, DEL);
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
    // free the data_dir
    if (data_dir != NULL) {
        free(data_dir);
        data_dir = NULL;
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
    init_pair(1, COLOR_WHITE, -1);            // 1
    init_pair(2, COLOR_YELLOW, -1);           // 2
    init_pair(3, COLOR_RED, -1);              // 3
    init_pair(4, COLOR_BLACK, COLOR_WHITE);   // 1 selected
    init_pair(5, COLOR_BLACK, COLOR_YELLOW);  // 2 selected
    init_pair(6, COLOR_WHITE, COLOR_RED);     // 3 selected
    // buttons
    init_pair(7, COLOR_WHITE, COLOR_RED);    // delete button selected
    init_pair(8, COLOR_RED, -1);             // delete button not selected
    init_pair(9, COLOR_WHITE, COLOR_GREEN);  // submit button selected
    init_pair(10, COLOR_GREEN, -1);          // submit button not selected
    init_pair(11, COLOR_WHITE, COLOR_RED);   // exit button selected
    init_pair(12, COLOR_RED, -1);            // exit button not selected
}

/**
 * Arguments:
 *  -l: list notes
 */

// Main function
int main(int argc, char *argv[]) {
    /***********************************
     *     Setup the data directory
     ***********************************/

    char *notes_dir_path;
#ifdef DEVMODE
    data_dir = malloc(strlen(DATA_DIR) + strlen(DATA) + 1);
    sprintf(data_dir, "%s%s", DATA_DIR, DATA);
    notes_dir_path = malloc(strlen(DATA_DIR) + 1);
#else
    char *home = (char *)getenv("HOME");
    notes_dir_path = (char *)malloc(strlen(home) + strlen(DATA_DIR) + 1);
    sprintf(notes_dir_path, DATA_DIR, home);
    data_dir = (char *)malloc(strlen(notes_dir_path) + strlen(DATA) + 1);
    sprintf(data_dir, "%s%s", notes_dir_path, DATA);
#endif

    /***************************************
     *     Setup the data file
     ***************************************/

    FILE *data_file = fopen(data_dir, "r");
    if (data_file == NULL) {
#ifndef DEVMODE
        // create directory notes_app in ~/.local/share if does not exist yet
        struct stat st = {0};
        if (stat(notes_dir_path, &st) == -1) {
            mkdir(notes_dir_path, 0600);  // permissions: -rw-------
        }
#endif
        // create the data file
        data_file = fopen(data_dir, "w");
        if (data_file == NULL) {
            printf("Error: Failed to create the data file\n");
            free(notes_dir_path);
            return 1;
        }
        fclose(data_file);
    } else {
        fclose(data_file);
    }

    // free the notes_dir_path
    free(notes_dir_path);

    // Get the notes
    note_list = notes_list_load(data_dir, DEL);
    // check if the notes were loaded successfully
    if (note_list == NULL) {
        printf("Error: Failed to load the notes: Data file not found\n");
        return 1;
    }

    // check if the user wants to list the notes
    if (argc > 1) {
        if (strcmp(argv[1], "-l") == 0) {
            int max_note_length = 0;

            // Find the longest note
            for (int i = 0; i < note_list->size; ++i) {
                int len = strlen(note_list->list[i]->data);
                if (len > max_note_length) {
                    max_note_length = len;
                }
            }

            // Calculate total width: "X: " (3 chars max) + space + note
            // + space + frame chars
            int total_width =
                max_note_length + 6;  // 6 = "X: " + 2 spaces + │ │

            // Draw top header (same width)
            printf("\n\033[1;44m");
            for (int i = 0; i < total_width + 2; ++i) printf(" ");
            printf("\033[0m\n");

            // Title line, centered
            const char *title = "Notes Listing";
            int title_len = strlen(title);
            int padding_left = (total_width - title_len) / 2;
            int padding_right = total_width - title_len - padding_left;
            printf("\033[1;44m");
            for (int i = 0; i < padding_left + 1; ++i) printf(" ");
            printf("%s", title);
            for (int i = 0; i < padding_right + 1; ++i) printf(" ");
            printf("\033[0m\n");

            // Separator line
            printf("\033[1;44m");
            for (int i = 0; i < total_width + 2; ++i) printf("-");
            printf("\033[0m\n");

            // Print each note with padding
            for (int i = 0; i < note_list->size; ++i) {
                char *color_bgn;
                char *color_end = "\033[0m";
                char *bg_colour;

                switch (note_list->list[i]->prt) {
                    case 1:
                        color_bgn = "\033[1;30m";  // Bold grey
                        bg_colour = "\033[47m";    // White background
                        break;
                    case 2:
                        color_bgn = "\033[1;33m";  // Bold yellow
                        bg_colour = "\033[46m";    // Cyan background
                        break;
                    case 3:
                        color_bgn = "\033[1;37m";  // Bold white
                        bg_colour = "\033[41m";    // Red background
                        break;
                    default:
                        color_bgn = "\033[0;37m";  // Default grey
                        bg_colour = "\033[40m";    // Black background
                        break;
                }

                // Compose note line
                char buffer[1024];
                snprintf(buffer, sizeof(buffer), "%d: %s",
                         note_list->list[i]->prt, note_list->list[i]->data);

                int line_len = strlen(buffer);
                int pad_spaces =
                    total_width - line_len - 2;  // 2 for the frame pipes
                printf("%s%s│ %s", bg_colour, color_bgn, buffer);
                for (int s = 0; s < pad_spaces; ++s) printf(" ");
                printf(" │%s\n", color_end);
            }

            // Footer line
            printf("\033[1;44m");
            for (int i = 0; i < total_width + 2; ++i) printf("=");
            printf("\033[0m\n\n");

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
    main_note_win = newwin(MAIN_WINDOW_HEIGHT, MAIN_WINDOW_WIDTH, MARGIN,
                           MENU_WINDOW_WIDTH + (MARGIN * 2));
    refresh();

    // Main loop
    while (quit_flag) {
        draw_menu_window(menu_win, selected_option, menu_options,
                         sizeof(menu_options) / sizeof(menu_options[0]),
                         selected_window == 0);
        draw_main_window(main_note_win, note_list, &selected_idx,
                         &main_window_mode, selected_window == 1, &priority,
                         &data, &group, group_list_size, group_list,
                         group_notes_list);
        quit_flag = handle_input(
            &selected_option, sizeof(menu_options) / sizeof(menu_options[0]),
            &selected_window, 2, &main_window_mode, &selected_idx, note_list,
            &priority, &data, &note_id, &group, &group_list, &group_list_size,
            &group_notes_list);
    }

    end_app();
}
