
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
#include "./notes/notes.h"

// Constants
// file path for the notes
#define DATA "./data"
// format of the notes in the file
#define DEL '-'

// Sizes
// general sizes
#define PAD 4
#define MARGIN 4
// main window sizes
#define MAIN_WINDOW_WIDTH 45
#define MAIN_WINDOW_HEIGHT (MAIN_WINDOW_WIDTH / 2)
// menu window sizes
#define MENU_WINDOW_WIDTH 18
#define MENU_WINDOW_HEIGHT MAIN_WINDOW_HEIGHT
// number of entries in the windows
// priority, data, group, submit
#define NEW_NOTE_WINDOW_NUM_ENTRIES 4
#define EDIT_NOTE_WINDOW_NUM_ENTRIES 5


// Global variables
volatile sig_atomic_t quit_flag = 0;
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
    quit_flag = 1;
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

// Input handling
void handle_input_main_window_list_notes(int key, int *selected_idx, int *main_window_mode, NoteList *note_list, int *selected_window, int *priority, char **data, int *note_id, char **group) {
    switch (key) {
        case KEY_UP:
            // check if the list is empty
            if (note_list->size == 0) break;
            *selected_idx = (*selected_idx - 1 + note_list->size) % note_list->size;
            break;
        case KEY_DOWN:
            // check if the list is empty
            if (note_list->size == 0) break;
            *selected_idx = (*selected_idx + 1) % note_list->size;
            break;
        case 113: // q
            end_app();
            break;
        case 10:  // Enter
            // go to the edit note mode with the selected note
            *main_window_mode = 2;
            *selected_window = 1;
            // set the priority
            *priority = note_list->list[*selected_idx]->prt;
            // set the data
            // start by allocating memory for the data (if not already allocated)
            if (*data == NULL) *data = (char *)malloc(sizeof(char) * 256);
            strcpy(*data, note_list->list[*selected_idx]->data);
            // set the note id
            *note_id = note_list->list[*selected_idx]->id;
            // set the group
            // start by allocating memory for the group (if not already allocated)
            if (*group == NULL) *group = (char *)malloc(sizeof(char) * 256);
            strcpy(*group, note_list->list[*selected_idx]->group);
            // reset the idx
            *selected_idx = 0;
            break;
        default:
            break;
    }

}

void handle_input_main_window_new_note(int key, int *selected_idx, int *main_window_mode, NoteList *note_list, int *selected_window, int *priority, char **data, char **group) {
    switch (key) {
        case KEY_UP:
            *selected_idx = (*selected_idx - 1 + NEW_NOTE_WINDOW_NUM_ENTRIES) % NEW_NOTE_WINDOW_NUM_ENTRIES;
            break;
        case KEY_DOWN:
            *selected_idx = (*selected_idx + 1) % NEW_NOTE_WINDOW_NUM_ENTRIES;
            break;
        case 10:  // Enter
            switch (*selected_idx) {
                case 0:  // priority
                case 1:  // data
                    *selected_idx = (*selected_idx + 1) % NEW_NOTE_WINDOW_NUM_ENTRIES;
                    break;
                case 2:  // group
                    *selected_idx = (*selected_idx + 1) % NEW_NOTE_WINDOW_NUM_ENTRIES;
                    break;
                case 3:  // submit
                    // validate the provided data and group
                    // check if the data is empty
                    // if it is, ignore the note
                    if (strlen(*data) == 0) break;
                    // check if the group is empty
                    // if it is, set the group to the default group
                    if (strlen(*group) == 0) strcpy(*group, NOTE_GROUP_DEFAULT);
                    // create a new note and add it to the list
                    Note *new_note = notes_create(*data, *priority, *group);
                    // check if the note was created successfully, if not, ignore the note
                    if (new_note != NULL) {
                        // add the note to the list
                        notes_list_add(note_list, new_note);
                    }
                    // go back to the main window mode 0 (list notes)
                    *main_window_mode = 0;
                    *selected_window = 1;
                    // reset the idx
                    *selected_idx = 0;
                    // reset the priority and data
                    *priority = 0;
                    // free the data
                    if (*data != NULL) {
                        free(*data);
                        *data = NULL;
                    }
                    // free the group
                    if (*group != NULL) {
                        free(*group);
                        *group = NULL;
                    }
                    break;
            }
            break;
        case 113: // q
            switch (*selected_idx) {
                case 0:  // priority
                case 2:  // submit
                    end_app();
                    break;
                default:
                    break;
            }
            break;
        default: // other keys: add the key to the data/priority depending on the selected index
            switch (*selected_idx) {
                case 0:  // priority
                    // check if the key is a number between 0 and 5
                    if (key >= '1' && key <= '5') {
                        *priority = key - '0';
                    }
                    break;
                case 1:  // data
                    // check if the key is a printable character
                    if (key >= 32 && key <= 126) {
                        // add the key to the data
                        int len = strlen(*data);
                        (*data)[len] = key;
                        // add an end of string character
                        (*data)[len+1] = '\0';
                    } else if (key == KEY_BACKSPACE) {
                        // delete the last character
                        int len = strlen(*data);
                        if (len > 0)
                            (*data)[len - 1] = '\0';
                    }
                    break;
                case 2:  // group
                    // check if the key is a printable character
                    if (key >= 32 && key <= 126) {
                        // add the key to the group
                        int len = strlen(*group);
                        (*group)[len] = key;
                        // add an end of string character
                        (*group)[len+1] = '\0';
                    } else if (key == KEY_BACKSPACE) {
                        // delete the last character
                        int len = strlen(*group);
                        if (len > 0)
                            (*group)[len - 1] = '\0';
                    }
                    break;
                default:
                    // q to quit
                    if (key == 113) end_app();
                    break;
            break;
        }
    }
}

void handle_input_main_window_edit_note(int key, int *selected_idx, int *main_window_mode, NoteList *note_list, int *selected_window, int *priority, char **data, int *note_id, char **group) {
    switch (key) {
        case KEY_UP:
            // if the selected index is on the first option, go to the submit option
            if (*selected_idx == 0) *selected_idx = EDIT_NOTE_WINDOW_NUM_ENTRIES - 2;
            // if not, just go to the previous option
            else *selected_idx = (*selected_idx - 1 + EDIT_NOTE_WINDOW_NUM_ENTRIES) % (EDIT_NOTE_WINDOW_NUM_ENTRIES);
            break;
        case KEY_DOWN:
            *selected_idx = (*selected_idx + 1) % EDIT_NOTE_WINDOW_NUM_ENTRIES;
            break;
        case 10:  // Enter
            switch (*selected_idx) {
                case 0:  // priority
                case 1:  // data
                    *selected_idx = (*selected_idx + 1) % EDIT_NOTE_WINDOW_NUM_ENTRIES;
                    break;
                case 2:  // group
                    *selected_idx = (*selected_idx + 1) % EDIT_NOTE_WINDOW_NUM_ENTRIES;
                    break;
                case 3:  // submit
                    // edit the note
                    // validate the provided data and group
                    if (strlen(*group) == 0) strcpy(*group, NOTE_GROUP_DEFAULT);
                    if (strlen(*data) == 0) data = NULL;
                    // update the note
                    Note *note = notes_list_get_by_id(note_list, *note_id);
                    notes_update(note, *data, *priority, *group);
                    // go back to the main window mode 0 (list notes)
                    *main_window_mode = 0;
                    *selected_window = 1;
                    // reset the priority and data
                    *priority = 0;
                    // free the data
                    if (*data != NULL) {
                        free(*data);
                        *data = NULL;
                    }
                    // free the group
                    if (*group != NULL) {
                        free(*group);
                        *group = NULL;
                    }
                    // reset the idx
                    *selected_idx = 0;
                    break;
                case 4:  // delete note
                    // delete the note
                    notes_list_remove(note_list, *note_id);
                    // go back to the main window mode 0 (list notes)
                    *main_window_mode = 0;
                    *selected_window = 1;
                    // reset the priority and data
                    *priority = 0;
                    // free the data
                    if (*data != NULL) {
                        free(*data);
                        *data = NULL;
                    }
                    // free the group
                    if (*group != NULL) {
                        free(*group);
                        *group = NULL;
                    }
                    // reset the idx
                    *selected_idx = 0;
                    break;
            }
            break;
        case 113: // q
            switch (*selected_idx) {
                case 0:  // priority
                case 2:  // submit
                case 3:  // delete note
                    end_app();
                    break;
                default:
                    break;
            }
            break;
        default: // other keys: add the key to the data/priority depending on the selected index
            switch (*selected_idx) {
                case 0:  // priority
                    // check if the key is a number between 0 and 5
                    if (key >= '1' && key <= '5') {
                        *priority = key - '0';
                    }
                    break;
                case 1:  // data
                    // check if the key is a printable character
                    if (key >= 32 && key <= 126) {
                        // add the key to the data
                        int len = strlen(*data);
                        (*data)[len] = key;
                        // add an end of string character
                        (*data)[len+1] = '\0';
                    } else if (key == KEY_BACKSPACE) {
                        // delete the last character
                        int len = strlen(*data);
                        if (len > 0)
                            (*data)[len - 1] = '\0';
                    }
                    break;
                case 2:  // group
                    // check if the key is a printable character
                    if (key >= 32 && key <= 126) {
                        // add the key to the group
                        int len = strlen(*group);
                        (*group)[len] = key;
                        // add an end of string character
                        (*group)[len+1] = '\0';
                    } else if (key == KEY_BACKSPACE) {
                        // delete the last character
                        int len = strlen(*group);
                        if (len > 0)
                            (*group)[len - 1] = '\0';
                    }
                    break;
                default:
                    // q to quit
                    if (key == 113) end_app();
                    break;
            break;
        }
    }
}

void handle_input_main_window_list_notes_group(int key, int *selected_idx, int *main_window_mode, NoteList *note_list, int *selected_window) {
    switch (key) {
        case KEY_UP:
            // check if the list is empty
            if (note_list->size == 0) break;
            *selected_idx = (*selected_idx - 1 + group_list_size) % group_list_size;
            break;
        case KEY_DOWN:
            // check if the list is empty
            if (note_list->size == 0) break;
            *selected_idx = (*selected_idx + 1) % group_list_size;
            break;
        case 113: // q
            end_app();
            break;
        case 10:  // Enter
            // list the notes from the selected group
            // go to the group notes mode
            *main_window_mode = 4;
            *selected_window = 1;
            // get the group notes list
            if (group_notes_list != NULL) {
                free(group_notes_list->list);
                free(group_notes_list);
                group_notes_list = NULL;
            }
            group_notes_list = notes_list_get_all_by_group(note_list, group_list[*selected_idx]);
            // reset the idx
            *selected_idx = 0;
            break;
        default:
            break;
    }
}

void handle_input_main_window_group_notes(int key, int *selected_idx, int *main_window_mode, NoteList *note_list, int *selected_window) {
    switch (key) {
        case KEY_UP:
            // check if the list is empty
            if (group_notes_list->size == 0) break;
            *selected_idx = (*selected_idx - 1 + group_notes_list->size) % group_notes_list->size;
            break;
        case KEY_DOWN:
            // check if the list is empty
            if (group_notes_list->size == 0) break;
            *selected_idx = (*selected_idx + 1) % group_notes_list->size;
            break;
        case 113: // q
            end_app();
            break;
        case 10:  // Enter
            // go to the edit note mode with the selected note
            *main_window_mode = 2;
            *selected_window = 1;
            // set the priority
            priority = group_notes_list->list[*selected_idx]->prt;
            // set the data
            // start by allocating memory for the data (if not already allocated)
            if (data == NULL) data = (char *)malloc(sizeof(char) * 256);
            strcpy(data, group_notes_list->list[*selected_idx]->data);
            // set the note id
            note_id = group_notes_list->list[*selected_idx]->id;
            // set the group
            // start by allocating memory for the group (if not not already allocated)
            if (group == NULL) group = (char *)malloc(sizeof(char) * 256);
            strcpy(group, group_notes_list->list[*selected_idx]->group);
            // reset the idx
            *selected_idx = 0;
            break;
        default:
            break;
    }
}

void handle_input_main_window(int key, int *selected_idx, int *main_window_mode, NoteList *note_list, int *selected_window, int *priority, char **data, int *note_id, char **group) {
    // main window modes:
    // 0 - list notes, 1 - new note, 2 - edit note
    switch (key) {
        // keys for main window mode only
        default:
            switch (*main_window_mode) {
                case 0:  // list notes
                    handle_input_main_window_list_notes(key, selected_idx, main_window_mode, note_list, selected_window, priority, data, note_id, group);
                    break;
                case 1:  // new note
                    handle_input_main_window_new_note(key, selected_idx, main_window_mode, note_list, selected_window, priority, data, group);
                    break;
                case 2:  // edit note
                    handle_input_main_window_edit_note(key, selected_idx, main_window_mode, note_list, selected_window, priority, data, note_id, group);
                    break;
                case 3: // group list
                    handle_input_main_window_list_notes_group(key, selected_idx, main_window_mode, note_list, selected_window);
                    break;
                case 4: // group notes
                    handle_input_main_window_group_notes(key, selected_idx, main_window_mode, note_list, selected_window);
                    break;
            }
            break;
    }
}

void handle_input_menu(int key, int *selected_option, int option_count, int *selected_window, int *main_window_mode, int *selected_idx, int *priority, char **data, char **group) {
    switch (key) {
        case KEY_UP:
            *selected_option = (*selected_option - 1 + option_count) % option_count;
            break;
        case KEY_DOWN:
            *selected_option = (*selected_option + 1) % option_count;
            break;
        case 113: // q
            end_app();
            break;
        case 10:  // Enter
            *selected_idx = 0;
            // reset the priority and data
            *priority = 0;
            // free the data
            if (*data != NULL) {
                free(*data);
                *data = NULL;
            }
            // free the group
            if (*group != NULL) {
                free(*group);
                *group = NULL;
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
                free(group_notes_list->list);
                free(group_notes_list);
                group_notes_list = NULL;
            }
            // check the selected button
            switch (*selected_option) {
                case 0:  // list groups
                    *main_window_mode = 3;
                    *selected_window = 1;
                    // start by getting the group list with notes_list_get_groups
                    if (group_list == NULL) group_list = notes_list_get_groups(note_list, &group_list_size);
                    break;
                case 1:  // list notes
                    *main_window_mode = 0;
                    *selected_window = 1;
                    break;
                case 2:  // new note
                    *main_window_mode = 1;
                    *selected_window = 1;
                    // start by allocating memory for the data and group (if not already allocated)
                    if (*data == NULL) *data = (char *)malloc(sizeof(char) * 256);
                    if (*group == NULL) *group = (char *)malloc(sizeof(char) * 256);
                    // set default values for the priority and data
                    *priority = 1;
                    strcpy(*data, "");
                    strcpy(*group, "");
                    break;
                case 3:  // exit
                    end_app();
                    break;
            }
            break;
        default:
            break;
    }
}

void handle_input(int *selected_option, int option_count, int *selected_window, int selected_window_count, int *main_window_mode, int *selected_idx, NoteList *note_list, int *priority, char **data, int *note_id, char **group, char ***group_list) {
    int key = getch();
    switch (key) {
        case KEY_LEFT:
            // go to the previous window
            *selected_window = (*selected_window - 1 + selected_window_count) % selected_window_count;
            break;
        case KEY_RIGHT:
            // go to the next window
            *selected_window = (*selected_window + 1) % selected_window_count;
            break;
        default:
            switch (*selected_window) {
                case 0:  // menu
                    handle_input_menu(key, selected_option, option_count, selected_window, main_window_mode, selected_idx, priority, data, group);
                    break;
                case 1:  // main window
                    handle_input_main_window(key, selected_idx, main_window_mode, note_list, selected_window, priority, data, note_id, group);
                    break;
            }
            break;
    }
}


// Windows functions
void draw_menu_window(WINDOW *menu_win, int selected_option, char *menu_options[], int option_count, int selected) {
    wclear(menu_win);  // Clear the window

    // draw the window border
    box(menu_win, 0, 0);

    // Print each menu option
    for (int i = 0; i < option_count; ++i) {
        if (i == selected_option && selected) {
            // highlight the selected option (red on exit, white on the others)
            if (i == option_count - 1) wattron(menu_win, COLOR_PAIR(11));
            else wattron(menu_win, COLOR_PAIR(4));
        }
        else {
            // highlight the not selected option (red on exit, white on the others)
            if (i == option_count - 1) wattron(menu_win, COLOR_PAIR(12));
            else wattron(menu_win, COLOR_PAIR(1));
        }

        // Print the option at a specific position within the window
        mvwprintw(menu_win, i + 1, 2, menu_options[i]);

        if (i == selected_option && selected) {
            // turn off highlighting
            if (i == option_count - 1) wattroff(menu_win, COLOR_PAIR(11));
            else wattroff(menu_win, COLOR_PAIR(4));
        }
        else {
            // turn off highlighting
            if (i == option_count - 1) wattroff(menu_win, COLOR_PAIR(12));
            else wattroff(menu_win, COLOR_PAIR(1));
        }
    }
    wrefresh(menu_win);
}

void draw_main_window(WINDOW *main_note_win, NoteList *note_list, int *selected_idx, int *main_window_mode, int selected, int *priority, char **data, char **group) {
    wclear(main_note_win);  // Clear the window

    // draw the window border
    box(main_note_win, 0, 0);

    // for each mode, print the corresponding window
    switch (*main_window_mode) {
    case 0:  // list notes
        // Print the title
        mvwprintw(main_note_win, 0, 2, " Notes ");
        // Print each note
        for (int i = 0; i < note_list->size; ++i) {
            // highlight the selected note
            if (i == *selected_idx && selected) wattron(main_note_win, COLOR_PAIR(note_list->list[i]->prt + NOTE_PRIORITY_MAX));
            else wattron(main_note_win, COLOR_PAIR(note_list->list[i]->prt));
            // Print the note at a specific position within the window
            // if the note is larger than the window, print only the first part
            if (strlen(note_list->list[i]->data) >= MAIN_WINDOW_WIDTH) {
                mvwprintw(main_note_win, i + 1, 2, "%.*s...", MAIN_WINDOW_WIDTH - (PAD * 2), note_list->list[i]->data);
            } else {
                mvwprintw(main_note_win, i + 1, 2, "%s", note_list->list[i]->data);
            }
            // turn off highlighting
            if (i == *selected_idx && selected) wattroff(main_note_win, COLOR_PAIR(note_list->list[i]->prt + NOTE_PRIORITY_MAX));
            else wattroff(main_note_win, COLOR_PAIR(note_list->list[i]->prt));
        }
        break;
    case 1:  // new note
        // print the title
        mvwprintw(main_note_win, 0, 2, " New note ");

        // priority
        if (*selected_idx == 0 && selected) wattron(main_note_win, A_REVERSE);
        mvwprintw(main_note_win, 1, 2, "Priority: %d", *priority);
        if (*selected_idx == 0 && selected) wattroff(main_note_win, A_REVERSE);

        // data
        if (*selected_idx == 1 && selected) wattron(main_note_win, A_REVERSE);
        mvwprintw(main_note_win, 2, 2, "Data: %s", *data);
        if (*selected_idx == 1 && selected) wattroff(main_note_win, A_REVERSE);

        // group
        if (*selected_idx == 2 && selected) wattron(main_note_win, A_REVERSE);
        mvwprintw(main_note_win, 3, 2, "Group: %s", *group);
        if (*selected_idx == 2 && selected) wattroff(main_note_win, A_REVERSE);

        // submit
        if (*selected_idx == 3 && selected) wattron(main_note_win, COLOR_PAIR(9));
        else wattron(main_note_win, COLOR_PAIR(10));
        mvwprintw(main_note_win, 4, 2, "Submit");
        if (*selected_idx == 3 && selected) wattroff(main_note_win, COLOR_PAIR(9));
        else wattroff(main_note_win, COLOR_PAIR(10));
        
        break;

    case 2:  // edit note
        // print the title
        mvwprintw(main_note_win, 0, 2, " Edit note ");

        // priority
        if (*selected_idx == 0 && selected) wattron(main_note_win, A_REVERSE);
        mvwprintw(main_note_win, 1, 2, "Priority: %d", *priority);
        if (*selected_idx == 0 && selected) wattroff(main_note_win, A_REVERSE);

        // data
        if (*selected_idx == 1 && selected) wattron(main_note_win, A_REVERSE);
        mvwprintw(main_note_win, 2, 2, "Data: %s", *data);
        if (*selected_idx == 1 && selected) wattroff(main_note_win, A_REVERSE);

        // group
        if (*selected_idx == 2 && selected) wattron(main_note_win, A_REVERSE);
        mvwprintw(main_note_win, 3, 2, "Group: %s", *group);
        if (*selected_idx == 2 && selected) wattroff(main_note_win, A_REVERSE);

        // submit
        if (*selected_idx == 3 && selected) wattron(main_note_win, COLOR_PAIR(9));
        else wattron(main_note_win, COLOR_PAIR(10));
        mvwprintw(main_note_win, 4, 2, "Submit");
        if (*selected_idx == 3 && selected) wattroff(main_note_win, COLOR_PAIR(9));
        else wattroff(main_note_win, COLOR_PAIR(10));

        // button to delete the note
        if (*selected_idx == 4 && selected) wattron(main_note_win, COLOR_PAIR(7));
        else wattron(main_note_win, COLOR_PAIR(8));
        mvwprintw(main_note_win, 6, 2, "Delete note");
        if (*selected_idx == 4 && selected) wattroff(main_note_win, COLOR_PAIR(7));
        else wattroff(main_note_win, COLOR_PAIR(8));

        break;

    case 3: // group list
        // print the title
        mvwprintw(main_note_win, 0, 2, " Groups ");

        // Print each group from the group_list
        for (int i = 0; i < group_list_size; ++i) {
            // highlight the selected group
            if (i == *selected_idx && selected) wattron(main_note_win, A_REVERSE);
            // Print the group at a specific position within the window
            mvwprintw(main_note_win, i + 1, 2, "%s", group_list[i]);
            // turn off highlighting
            if (i == *selected_idx && selected) wattroff(main_note_win, A_REVERSE);
        }
        break;
    case 4: // group notes
        // print the title
        mvwprintw(main_note_win, 0, 2, " Group notes ");

        // Print each note from the group_notes_list
        for (int i = 0; i < group_notes_list->size; ++i) {
            // highlight the selected note
            if (i == *selected_idx && selected) wattron(main_note_win, COLOR_PAIR(group_notes_list->list[i]->prt + NOTE_PRIORITY_MAX));
            else wattron(main_note_win, COLOR_PAIR(group_notes_list->list[i]->prt));
            // Print the note at a specific position within the window
            // if the note is larger than the window, print only the first part
            if (strlen(group_notes_list->list[i]->data) >= MAIN_WINDOW_WIDTH) {
                mvwprintw(main_note_win, i + 1, 2, "%.*s...", MAIN_WINDOW_WIDTH - (PAD * 2), group_notes_list->list[i]->data);
            } else {
                mvwprintw(main_note_win, i + 1, 2, "%s", group_notes_list->list[i]->data);
            }
            // turn off highlighting
            if (i == *selected_idx && selected) wattroff(main_note_win, COLOR_PAIR(group_notes_list->list[i]->prt + NOTE_PRIORITY_MAX));
            else wattroff(main_note_win, COLOR_PAIR(group_notes_list->list[i]->prt));
        }
        break;
    default:
        // print a warning message
        mvwprintw(main_note_win, 1, 2, "This mode is not implemented yet.");
        break;
    }

    wrefresh(main_note_win);
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
    while (!quit_flag) {
        draw_menu_window(menu_win, selected_option, menu_options, sizeof(menu_options) / sizeof(menu_options[0]), selected_window == 0);
        draw_main_window(main_note_win, note_list, &selected_idx, &main_window_mode, selected_window == 1, &priority, &data, &group);
        handle_input(&selected_option, sizeof(menu_options) / sizeof(menu_options[0]), &selected_window,
                    2, &main_window_mode, &selected_idx, note_list, &priority, &data, &note_id, &group, &group_list);
    }

    // close windows
    delwin(menu_win);
    delwin(main_note_win);

    end_app(); 
}
