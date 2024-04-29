#include "window_manager.h"
#include <string.h>

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

void draw_main_window(WINDOW *main_note_win, NoteList *note_list, int *selected_idx, int *main_window_mode, int selected, int *priority, char **data, char **group, int group_list_size, char **group_list, NoteList *group_notes_list) {
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
