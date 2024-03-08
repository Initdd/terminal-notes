// general test for notes library
// generic libraries
#include <stdio.h>
#include <stdlib.h>
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

void test_notes_list_create() {
    // create a new note list
    NoteList *list = notes_list_create();
    // check if the list was created
    assert(list != NULL);
    // check if the list has the correct size
    assert(list->size == 0);
    // check if the list has the correct capacity
    assert(list->capacity == NOTE_LIST_INITIAL_CAPACITY);
    // check if the list has the correct list
    assert(list->list != NULL);
    // delete the list
    notes_list_delete(list);
}

void test_notes_list_add() {
    // create a new note list
    NoteList *list = notes_list_create();
    // add a note to the list
    notes_list_add(list, notes_create("Hello World!", 1, "test"));
    // check if the note was added
    assert(list->size == 1);
    assert(list->list[0] != NULL);
    assert(strcmp(list->list[0]->data, "Hello World!") == 0);
    assert(list->list[0]->prt == 1);
    assert(strcmp(list->list[0]->group, "test") == 0);
    // delete the list
    notes_list_delete(list);
}

void test_notes_list_get_by_id() {
    // create a new note list
    NoteList *list = notes_list_create();
    // add a note to the list
    notes_list_add(list, notes_create("Hello World!", 1, "test"));
    // get a note from the list
    Note *note = notes_list_get_by_id(list, 0);
    // check if the note was gotten
    assert(note != NULL);
    assert(strcmp(note->data, "Hello World!") == 0);
    assert(note->prt == 1);
    assert(strcmp(note->group, "test") == 0);
    // delete the list
    notes_list_delete(list);
}

void test_notes_list_get_by_idx() {
    // create a new note list
    NoteList *list = notes_list_create();
    // add a note to the list
    notes_list_add(list, notes_create("Hello World!", 1, "test"));
    // get a note from the list
    Note *note = notes_list_get_by_idx(list, 0);
    // check if the note was gotten
    assert(note != NULL);
    assert(strcmp(note->data, "Hello World!") == 0);
    assert(note->prt == 1);
    assert(strcmp(note->group, "test") == 0);
    // delete the list
    notes_list_delete(list);
}

void test_notes_list_get_all_by_group() {
    // create a new note list
    NoteList *list = notes_list_create();
    // add a note to the list
    notes_list_add(list, notes_create("Hello World!", 1, "test"));
    // get a note from the list
    NoteList *group = notes_list_get_all_by_group(list, "test");
    // check if the note was gotten
    assert(group != NULL);
    assert(group->size == 1);
    assert(strcmp(group->list[0]->data, "Hello World!") == 0);
    assert(group->list[0]->prt == 1);
    assert(strcmp(group->list[0]->group, "test") == 0);
    // delete the list
    notes_list_delete(list);
}

void test_notes_list_get_groups() {
    // create a new note list
    NoteList *list = notes_list_create();
    // add a note to the list
    notes_list_add(list, notes_create("Hello World!", 1, "test"));
    // get a note from the list
    char **groups = notes_list_get_groups(list);
    // check if the note was gotten
    assert(groups != NULL);
    assert(strcmp(groups[0], "test") == 0);
    assert(groups[1] == NULL);
    // delete the list
    notes_list_delete(list);
    // free the groups
    free(groups);
}

void test_notes_list_update() {
    // create a new note list
    NoteList *list = notes_list_create();
    // add a note to the list
    notes_list_add(list, notes_create("Hello World!", 1, "test"));
    // update a note from the list
    notes_list_update(list, 0, "Hello World! Updated", 2, "test Updated");
    // check if the note was updated
    assert(strcmp(list->list[0]->data, "Hello World! Updated") == 0);
    assert(list->list[0]->prt == 2);
    assert(strcmp(list->list[0]->group, "test Updated") == 0);
    // delete the list
    notes_list_delete(list);
}

void test_notes_list_remove() {
    // create a new note list
    NoteList *list = notes_list_create();
    // add a note to the list
    notes_list_add(list, notes_create("Hello World!", 1, "test"));
    // remove a note from the list
    notes_list_remove(list, 0);
    // check if the note was removed
    assert(list->size == 0);
    assert(list->list[0] == NULL);
    // delete the list
    notes_list_delete(list);
}

void test_notes_list_read() {
    // create a new note list
    NoteList *list = notes_list_create();
    // add a note to the list
    notes_list_add(list, notes_create("Hello World!", 1, "test"));
    // read the list
    char *str = notes_list_read(list);
    // check if the list was read
    assert(strcmp(str, "0 1 test Hello World!") == 0);
    // free the string
    free(str);
    // delete the list
    notes_list_delete(list);
}

void test_notes_list_delete() {
    // create a new note list
    NoteList *list = notes_list_create();
    // delete the list
    notes_list_delete(list);
}

void test_notes_list_save() {
    // create a new note list
    NoteList *list = notes_list_create();
    // add a note to the list
    notes_list_add(list, notes_create("Hello World!", 1, "test"));
    // save the list
    notes_list_save(list, FILENAME, DEL);
    // read the file
    FILE *file = fopen(FILENAME, "r");
    char str[255];
    fscanf(file, "%[^\n]", str);
    // check if the list was saved
    assert(strcmp(str, "1-test Hello World!") == 0);
    // close the file
    fclose(file);
    // delete the list
    notes_list_delete(list);
}

void test_notes_list_load() {
    // create a new note list
    NoteList *list = notes_list_create();
    // add a note to the list
    notes_list_add(list, notes_create("Hello World!", 1, "test"));
    // save the list
    notes_list_save(list, FILENAME, DEL);
    // delete the list
    notes_list_delete(list);
    // load the list
    list = notes_list_load(FILENAME, DEL);
    // check if the list was loaded
    assert(list != NULL);
    assert(list->size == 1);
    assert(strcmp(list->list[0]->data, "Hello World!") == 0);
    assert(list->list[0]->prt == 1);
    assert(strcmp(list->list[0]->group, "test") == 0);
    // delete the list
    notes_list_delete(list);
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
    test_notes_update();
    test_notes_read();
    test_notes_delete();
    test_notes_list_create();
    test_notes_list_add();
    test_notes_list_get_by_id();
    test_notes_list_get_by_idx();
    test_notes_list_get_all_by_group();
    test_notes_list_get_groups();
    test_notes_list_update();
    test_notes_list_remove();
    test_notes_list_read();
    test_notes_list_delete();
    test_notes_list_save();
    test_notes_list_load();
    //test_notes_list_foreach();
    // print the results
    printf("All tests passed\n");
    // return success
    return 0;
}

