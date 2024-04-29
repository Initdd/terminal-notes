#include <ncurses.h>
#include <stddef.h>
#include "../notes_manager/notes_manager.h"

int handle_input_menu(int key, int *selected_option, int option_count, int *selected_window, int *main_window_mode, int *selected_idx, int *priority, char **data, char **group, int *group_list_size, char ***group_list, NoteList *note_list, NoteList **group_notes_list) {
    switch (key) {
        case KEY_UP:
            *selected_option = (*selected_option - 1 + option_count) % option_count;
            break;
        case KEY_DOWN:
            *selected_option = (*selected_option + 1) % option_count;
            break;
        case 113: // q
            return 0; // quit
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
            if (*group_list != NULL) {
                for (int i = 0; i < *group_list_size; ++i) {
                    free((*group_list)[i]);
                    ((*group_list)[i]) = NULL;
                }
                free(*group_list);
                *group_list = NULL;
            }
            // free the group notes list
            if (*group_notes_list != NULL) {
                // free the list
                free((*group_notes_list)->list);
                free(*group_notes_list);
                *group_notes_list = NULL;
            }
            // check the selected button
            switch (*selected_option) {
                case 0:  // list groups
                    *main_window_mode = 3;
                    *selected_window = 1;
                    // start by getting the group list with notes_list_get_groups
                    if (*group_list == NULL) *group_list = notes_list_get_groups(note_list, group_list_size);
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
                    return 0; // quit
                    break;
            }
            break;
        default:
            break;
    }
    return 1; // continue
}
