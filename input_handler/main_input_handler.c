
#include <ncurses.h>
#include "../notes_manager/notes_manager.h"
#include "./main_window_input_handler.c"
#include "./menu_window_input_handler.c"

int handle_input(int *selected_option, int option_count, int *selected_window, int selected_window_count, int *main_window_mode, int *selected_idx, NoteList *note_list, int *priority, char **data, int *note_id, char **group, char ***group_list, int *group_list_size, NoteList **group_notes_list) {
    int key = getch();
    int sig = 1;
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
                    sig = handle_input_menu(key, selected_option, option_count, selected_window, main_window_mode, selected_idx, priority, data, group, group_list_size, group_list, note_list, group_notes_list);
                    break;
                case 1:  // main window
                    sig = handle_input_main_window(key, selected_idx, main_window_mode, note_list, selected_window, priority, data, note_id, group, group_list, *group_list_size, group_notes_list);
                    break;
            }
            break;
    }
    return sig;
}