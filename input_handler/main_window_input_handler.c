
#include <ncurses.h>
#include "../notes_manager/notes_manager.h"

// Constants
// number of entries in the windows
// priority, data, group, submit
#define NEW_NOTE_WINDOW_NUM_ENTRIES 4
#define EDIT_NOTE_WINDOW_NUM_ENTRIES 5


// Input handling
int handle_input_main_window_list_notes(int key, int *selected_idx, int *main_window_mode, NoteList *note_list, int *selected_window, int *priority, char **data, int *note_id, char **group) {
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
            return 0; // quit
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
    return 1;
}

int handle_input_main_window_new_note(int key, int *selected_idx, int *main_window_mode, NoteList *note_list, int *selected_window, int *priority, char **data, char **group) {
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
                    return 0; // quit
                    break;
                default:
                    break;
            }
            break;
        default: // other keys: add the key to the data/priority depending on the selected index
            switch (*selected_idx) {
                case 0:  // priority
                    // check if the key is a number between 0 and 5
                    if (key >= '1' && key <= '3') {
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
                    if (key == 113)
                        return 0; // quit
                    break;
            break;
        }
    }
    return 1;
}

int handle_input_main_window_edit_note(int key, int *selected_idx, int *main_window_mode, NoteList *note_list, int *selected_window, int *priority, char **data, int *note_id, char **group) {
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
                    return 0; // quit
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
                    if (key == 113) return 0; // quit
                    break;
            break;
        }
    }
    return 1;
}

int handle_input_main_window_list_notes_group(int key, int *selected_idx, int *main_window_mode, NoteList *note_list, int *selected_window, char ***group_list, int group_list_size, NoteList **group_notes_list) {
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
            return 0; // quit
            break;
        case 10:  // Enter
            // list the notes from the selected group
            // go to the group notes mode
            *main_window_mode = 4;
            *selected_window = 1;
            // get the group notes list
            if (*group_notes_list != NULL) {
                free((*group_notes_list)->list);
                free(*group_notes_list);
                *group_notes_list = NULL;
            }
            // set the group_notes_list to the result of the function
            *group_notes_list = notes_list_get_all_by_group(note_list, (*group_list)[*selected_idx]);
            // reset the idx
            *selected_idx = 0;
            break;
        default:
            break;
    }
    return 1;
}

int handle_input_main_window_group_notes(int key, int *selected_idx, int *main_window_mode, int *selected_window, NoteList *group_notes_list, int *priority, char **data, int *note_id, char **group) {
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
            return 0; // quit
            break;
        case 10:  // Enter
            // go to the edit note mode with the selected note
            *main_window_mode = 2;
            *selected_window = 1;
            // set the priority
            *priority = group_notes_list->list[*selected_idx]->prt;
            // set the data
            // start by allocating memory for the data (if not already allocated)
            if (*data == NULL) *data = (char *)malloc(sizeof(char) * 256);
            strcpy(*data, group_notes_list->list[*selected_idx]->data);
            // set the note id
            *note_id = group_notes_list->list[*selected_idx]->id;
            // set the group
            // start by allocating memory for the group (if not not already allocated)
            if (*group == NULL) *group = (char *)malloc(sizeof(char) * 256);
            strcpy(*group, group_notes_list->list[*selected_idx]->group);
            // reset the idx
            *selected_idx = 0;
            break;
        default:
            break;
    }
    return 1;
}


int handle_input_main_window(int key, int *selected_idx, int *main_window_mode, NoteList *note_list, int *selected_window, int *priority, char **data, int *note_id, char **group, char ***group_list, int group_list_size, NoteList **group_notes_list) {
    // main window modes:
    // 0 - list notes, 1 - new note, 2 - edit note
    int sig = 1;
    switch (key) {
        // keys for main window mode only
        default:
            switch (*main_window_mode) {
                case 0:  // list notes
                    sig = handle_input_main_window_list_notes(key, selected_idx, main_window_mode, note_list, selected_window, priority, data, note_id, group);
                    break;
                case 1:  // new note
                    sig = handle_input_main_window_new_note(key, selected_idx, main_window_mode, note_list, selected_window, priority, data, group);
                    break;
                case 2:  // edit note
                    sig = handle_input_main_window_edit_note(key, selected_idx, main_window_mode, note_list, selected_window, priority, data, note_id, group);
                    break;
                case 3: // group list
                    sig = handle_input_main_window_list_notes_group(key, selected_idx, main_window_mode, note_list, selected_window, group_list, group_list_size, group_notes_list);
                    break;
                case 4: // group notes
                    sig = handle_input_main_window_group_notes(key, selected_idx, main_window_mode, selected_window, *group_notes_list, priority, data, note_id, group);
                    break;
            }
            break;
    }
    return sig;
}
