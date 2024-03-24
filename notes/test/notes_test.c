// general test for notes library
// generic libraries
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
// local libraries
#include "../notes.h"

// generic variables
#define FILENAME "test.txt"
#define DEL '-'

void test_notes_create() {
    // create a new note
    Note *note = notes_create("Hello World!", 1, "test");
    // check if the note was created
    assert(note != NULL);
    // check if the note has the correct data
    assert(strcmp(note->data, "Hello World!") == 0);
    assert(note->prt == 1);
    assert(note->id == 0);
    assert(strcmp(note->group, "test") == 0);
    // delete the note
    notes_delete(note);
}

void test_notes_create_data_empty() {
    // create a new note
    Note *note = notes_create("", 1, "test");
    // check if the note was created
    assert(note == NULL);
}

void test_notes_create_data_null() {
    // create a new note
    Note *note = notes_create(NULL, 1, "test");
    // check if the note was created
    assert(note == NULL);
}

void test_notes_create_group_empty() {
    // create a new note
    Note *note = notes_create("Hello World!", 1, "");
    // check if the note was created
    assert(note == NULL);
}

void test_notes_create_group_null() {
    // create a new note
    Note *note = notes_create("Hello World!", 1, NULL);
    // check if the note was created
    assert(note == NULL);
}

void test_notes_create_prt_less_than_zero() {
    // create a new note
    Note *note = notes_create("Hello World!", -1, "test");
    // check if the note was created
    assert(note == NULL);
}

void test_notes_create_prt_greater_than_max() {
    // create a new note
    Note *note = notes_create("Hello World!", 6, "test");
    // check if the note was created
    assert(note == NULL);
}

void test_notes_update() {
    // create a new note
    Note *note = notes_create("Hello World!", 1, "test");
    // update the note
    notes_update(note, "Hello World! Updated", 2, "test Updated");
    // check if the note was updated
    assert(strcmp(note->data, "Hello World! Updated") == 0);
    assert(note->prt == 2);
    assert(strcmp(note->group, "test Updated") == 0);
    // delete the note
    notes_delete(note);
}

void test_notes_update_data_empty() {
    // create a new note
    Note *note = notes_create("Hello World!", 1, "test");
    // update the note
    notes_update(note, "", 2, "test Updated");
    // check if the note was updated
    assert(strcmp(note->data, "Hello World!") == 0);
    assert(note->prt == 2);
    assert(strcmp(note->group, "test Updated") == 0);
    // delete the note
    notes_delete(note);
}

void test_notes_update_group_empty() {
    // create a new note
    Note *note = notes_create("Hello World!", 1, "test");
    // update the note
    notes_update(note, "Hello World! Updated", 2, "");
    // check if the note was updated
    assert(strcmp(note->data, "Hello World! Updated") == 0);
    assert(note->prt == 2);
    assert(strcmp(note->group, "test") == 0);
    // delete the note
    notes_delete(note);
}

void test_notes_read() {
    // create a new note
    Note *note = notes_create("Hello World!", 1, "test");
    // read the note
    char *str = notes_read(note, ' ');
    // check if the note was read
    assert(strcmp(str, "0 1 test Hello World!") == 0);
    // free the string
    free(str);
    // delete the note
    notes_delete(note);
}

void test_notes_delete() {
    // create a new note
    Note *note = notes_create("Hello World!", 1, "test");
    // delete the note
    notes_delete(note);
}

// void test_notes_list_foreach() {
//     // create a new note list
//     NoteList *list = notes_list_create();
//     // add a note to the list
//     notes_list_add(list, notes_create("Hello World!", 1));
//     // apply a function to each note in the list
//     notes_list_foreach(list, notes_read);
//     // delete the list
//     notes_list_delete(list);
// }

int main() {
    // run the tests
    test_notes_create();
    test_notes_create_data_empty();
    test_notes_create_data_null();
    test_notes_create_group_empty();
    test_notes_create_group_null();
    test_notes_create_prt_less_than_zero();
    test_notes_create_prt_greater_than_max();
    test_notes_update();
    test_notes_update_data_empty();
    test_notes_update_group_empty();
    test_notes_read();
    test_notes_delete();
    //test_notes_list_foreach();
    // print the results
    printf("All tests passed\n");
    // return success
    return 0;
}

