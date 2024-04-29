#include <ncurses.h>
#include "../notes_manager/notes_manager.h"

#ifndef WINDOW_MANAGER_H
#define WINDOW_MANAGER_H

// Constants
// main window sizes
#define MAIN_WINDOW_WIDTH 45
#define MAIN_WINDOW_HEIGHT (MAIN_WINDOW_WIDTH / 2)
// menu window sizes
#define MENU_WINDOW_WIDTH 18
#define MENU_WINDOW_HEIGHT MAIN_WINDOW_HEIGHT
// general sizes
#define PAD 4
#define MARGIN 4

// Windows functions
void draw_menu_window(WINDOW *menu_win, int selected_option, char *menu_options[], int option_count, int selected);

void draw_main_window(WINDOW *main_note_win, NoteList *note_list, int *selected_idx, int *main_window_mode, int selected, int *priority, char **data, char **group, int group_list_size, char **group_list, NoteList *group_notes_list);

#endif // WINDOW_MANAGER_H