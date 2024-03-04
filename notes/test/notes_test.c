// general test for notes library
// generic libraries
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
// local libraries
#include "../notes.h"

// generic variables
#define FILENAME "test.txt"
#define DEL '-'

void test_notes_create() {
    // create a new note
    Note *note = notes_create("Hello World!", 1);
    // check if the note was created
    assert(note != NULL);
    // check if the note has the correct data
    assert(strcmp(note->data, "Hello World!") == 0);
    assert(note->prt == 1);
    assert(note->id == 0);
    // delete the note
    notes_delete(note);
}

void test_notes_update() {
    // create a new note
    Note *note = notes_create("Hello World!", 1);
    // update the note
    notes_update(note, "Hello World! Updated", 2);
    // check if the note was updated
    assert(strcmp(note->data, "Hello World! Updated") == 0);
    assert(note->prt == 2);
    // delete the note
    notes_delete(note);
}

void test_notes_read() {
    // create a new note
    Note *note = notes_create("Hello World!", 1);
    // read the note
    char *str = notes_read(note, ' ');
    // check if the note was read
    assert(strcmp(str, "0 Hello World! 1") == 0);
    // free the string
    free(str);
    // delete the note
    notes_delete(note);
}

void test_notes_delete() {
    // create a new note
    Note *note = notes_create("Hello World!", 1);
    // delete the note
    notes_delete(note);
    // check if the note was deleted
    assert(note == NULL);
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
    notes_list_add(list, notes_create("Hello World!", 1));
    // check if the note was added
    assert(list->size == 1);
    assert(list->list[0] != NULL);
    assert(strcmp(list->list[0]->data, "Hello World!") == 0);
    // delete the list
    notes_list_delete(list);
}

void test_notes_list_get() {
    // create a new note list
    NoteList *list = notes_list_create();
    // add a note to the list
    notes_list_add(list, notes_create("Hello World!", 1));
    // get a note from the list
    Note *note = notes_list_get(list, 0);
    // check if the note was gotten
    assert(note != NULL);
    assert(strcmp(note->data, "Hello World!") == 0);
    assert(note->prt == 1);
    // delete the list
    notes_list_delete(list);
}

void test_notes_list_update() {
    // create a new note list
    NoteList *list = notes_list_create();
    // add a note to the list
    notes_list_add(list, notes_create("Hello World!", 1));
    // update a note from the list
    notes_list_update(list, 0, "Hello World! Updated", 2);
    // check if the note was updated
    assert(strcmp(list->list[0]->data, "Hello World! Updated") == 0);
    assert(list->list[0]->prt == 2);
    // delete the list
    notes_list_delete(list);
}

void test_notes_list_remove() {
    // create a new note list
    NoteList *list = notes_list_create();
    // add a note to the list
    notes_list_add(list, notes_create("Hello World!", 1));
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
    notes_list_add(list, notes_create("Hello World!", 1));
    // read the list
    char *str = notes_list_read(list);
    // check if the list was read
    assert(strcmp(str, "0 Hello World! 1\n") == 0);
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
    // check if the list was deleted
    assert(list == NULL);
}

void test_notes_list_save() {
    // create a new note list
    NoteList *list = notes_list_create();
    // add a note to the list
    notes_list_add(list, notes_create("Hello World!", 1));
    // save the list
    notes_list_save(list, FILENAME, DEL);
    // read the file
    FILE *file = fopen(FILENAME, "r");
    char str[255];
    fscanf(file, "%s", str);
    // check if the list was saved
    assert(strcmp(str, "0-Hello World!") == 0);
    // close the file
    fclose(file);
    // delete the list
    notes_list_delete(list);
}

void test_notes_list_load() {
    // create a new note list
    NoteList *list = notes_list_create();
    // add a note to the list
    notes_list_add(list, notes_create("Hello World!", 1));
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
    // delete the list
    notes_list_delete(list);
}

void test_notes_list_foreach() {
    // create a new note list
    NoteList *list = notes_list_create();
    // add a note to the list
    notes_list_add(list, notes_create("Hello World!", 1));
    // apply a function to each note in the list
    notes_list_foreach(list, notes_read);
    // delete the list
    notes_list_delete(list);
}

void test_notes_group_create() {
    // create a new note group
    Group *group = notes_group_create("Hello World!");
    // check if the group was created
    assert(group != NULL);
    // check if the group has the correct name
    assert(strcmp(group->name, "Hello World!") == 0);
    // check if the group has the correct list
    assert(group->list != NULL);
    // delete the group
    notes_group_delete(group);
}

void test_notes_group_add() {
    // create a new note group
    Group *group = notes_group_create("Hello World!");
    // add a note to the group
    notes_group_add(group, notes_create("Hello World!", 1));
    // check if the note was added
    assert(group->size == 1);
    assert(group->list[0] != NULL);
    assert(strcmp(((Note*)group->list[0])->data, "Hello World!") == 0);
    // delete the group
    notes_group_delete(group);
}

void test_notes_group_remove() {
    // create a new note group
    Group *group = notes_group_create("Hello World!");
    // add a note to the group
    notes_group_add(group, notes_create("Hello World!", 1));
    // remove a note from the group
    notes_group_remove_item(group, 0);
    // check if the note was removed
    assert(group->size == 0);
    assert(group->list[0] == NULL);
    // delete the group
    notes_group_delete(group);
}

void test_notes_group_read();

int main() {
    // run the tests
    test_notes_create();
    test_notes_update();
    test_notes_read();
    test_notes_delete();
    test_notes_list_create();
    test_notes_list_add();
    test_notes_list_get();
    test_notes_list_update();
    test_notes_list_remove();
    test_notes_list_read();
    test_notes_list_delete();
    test_notes_list_save();
    test_notes_list_load();
    test_notes_list_foreach();
    test_notes_group_create();
    test_notes_group_add();
    test_notes_group_remove();
    test_notes_group_read();
    // print the results
    printf("All tests passed\n");
    // return success
    return 0;
}


