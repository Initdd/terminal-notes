// general test for notes library
// generic libraries
#include <stdio.h>
#include <stdlib.h>
// local libraries
#include "../notes.h"

// test flags
#define TEST_NOTE 0
#define TEST_NOTELIST 0

// generic variables
#define FILENAME "test.txt"
#define DEL '-'


int main() {
    #if(TEST_NOTE)
    // Note test
    // create a new note
    Note *note = notes_create("Hello World!", 1);
    // print the note
    char *str1 = notes_read(note, ' ');
    printf("Note: %s\n", str1);
    free(str1);
    // update the note
    notes_update(note, "Hello World! Updated", 2);
    // print the note
    char *str2 = notes_read(note, ' ');
    printf("Note: %s\n", str2);
    free(str2);
    // delete the note
    notes_delete(note);
    #endif
    #if(TEST_NOTELIST)

    // NoteList test
    // create a new note list
    NoteList *list = notes_list_create();
    // add a note to the list
    notes_list_add(list, notes_create("Hello World!", 1));
    // print the list
    char *str1 = notes_list_read(list);
    printf("List: %s\n", str1);
    free(str1);
    // add a note to the list
    Note *note = notes_create("Hello World!", 1);
    notes_list_add(list, note);
    // print the list
    char *str2 = notes_list_read(list);
    printf("List: %s\n", str2);
    free(str2);
    // update a note from the list
    notes_list_update(list, 0, "Hello World! Updated", 3);
    // print the list
    char *str3 = notes_list_read(list);
    printf("List: %s\n", str3);
    free(str3);
    // remove a note from the list
    notes_list_remove(list, 0);
    // print the list
    char *str4 = notes_list_read(list);
    printf("List: %s\n", str4);
    free(str4);
    // delete the list
    notes_list_delete(list);
    // check if the list was deleted
    printf("List: %s\n", notes_list_read(list));
    #else
    // NoteList file test
    // create a new note list
    NoteList *list = notes_list_create();
    // add a note to the list
    notes_list_add(list, notes_create("Hello World!", 1));
    // save the list to a file
    notes_list_save(list, "test.txt", DEL);
    // delete the list
    notes_list_delete(list);
    // load the list from the file
    NoteList *list2 = notes_list_load("test.txt", DEL);
    // print the list
    char *str1 = notes_list_read(list2);
    printf("List: %s\n", str1);
    free(str1);
    // delete the list
    notes_list_delete(list2);
    #endif
}


