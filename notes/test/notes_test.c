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

void test_notes_list_add_full() {
    // create a new note list
    NoteList *list = notes_list_create();
    // set the list size 
    int size = NOTE_LIST_INITIAL_CAPACITY+1;
    // add notes to the list
    for (int i = 0; i < size; i++) {
        notes_list_add(list, notes_create("Hello World!", 1, "test"));
    }
    // check if the list was resized
    assert(list->size == size);
    assert(list->capacity == NOTE_LIST_INITIAL_CAPACITY * 2);
    // check if the notes were added
    for (int i = 0; i < NOTE_LIST_INITIAL_CAPACITY; i++) {
        assert(list->list[i] != NULL);
        assert(strcmp(list->list[i]->data, "Hello World!") == 0);
        assert(list->list[i]->prt == 1);
        assert(strcmp(list->list[i]->group, "test") == 0);
    }
    // delete the list
    notes_list_delete(list);
}

void test_notes_list_add_null() {
    // create a new note list
    NoteList *list = notes_list_create();
    // add a note to the list
    notes_list_add(list, NULL);
    // check if the note was added
    assert(list->size == 0);
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

void test_notes_list_get_by_id_not_found() {
    // create a new note list
    NoteList *list = notes_list_create();
    // add a note to the list
    notes_list_add(list, notes_create("Hello World!", 1, "test"));
    // get a note from the list
    Note *note = notes_list_get_by_id(list, 1);
    // check if the note was gotten
    assert(note == NULL);
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

void test_notes_list_get_by_idx_not_found() {
    // create a new note list
    NoteList *list = notes_list_create();
    // add a note to the list
    notes_list_add(list, notes_create("Hello World!", 1, "test"));
    // get a note from the list
    Note *note = notes_list_get_by_idx(list, 1);
    // check if the note was gotten
    assert(note == NULL);
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

void test_notes_list_get_all_by_group_not_found() {
    // create a new note list
    NoteList *list = notes_list_create();
    // add a note to the list
    notes_list_add(list, notes_create("Hello World!", 1, "test"));
    // get a note from the list
    NoteList *group = notes_list_get_all_by_group(list, "test2");
    // check if the note was gotten
    assert(group != NULL);
    assert(group->size == 0);
    // delete the list
    notes_list_delete(list);
}

void test_notes_list_get_groups() {
    // create a new note list
    NoteList *list = notes_list_create();
    // add some notes to the list
    notes_list_add(list, notes_create("Hello World!", 1, "test"));
    notes_list_add(list, notes_create("Hello World!2", 2, "test2"));
    // get a note from the list
    int size;
    char **groups = notes_list_get_groups(list, &size);
    // check if the note was gotten
    assert(groups != NULL);
    assert(strcmp(groups[0], "test") == 0);
    assert(strcmp(groups[1], "test2") == 0);
    assert(size == 2);
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

void test_notes_list_update_data_empty() {
    // create a new note list
    NoteList *list = notes_list_create();
    // add a note to the list
    notes_list_add(list, notes_create("Hello World!", 1, "test"));
    // update a note from the list
    notes_list_update(list, 0, "", 2, "test Updated");
    // check if the note was updated
    assert(strcmp(list->list[0]->data, "Hello World!") == 0);
    assert(list->list[0]->prt == 2);
    assert(strcmp(list->list[0]->group, "test Updated") == 0);
    // delete the list
    notes_list_delete(list);
}

void test_notes_list_update_group_empty() {
    // create a new note list
    NoteList *list = notes_list_create();
    // add a note to the list
    notes_list_add(list, notes_create("Hello World!", 1, "test"));
    // update a note from the list
    notes_list_update(list, 0, "Hello World! Updated", 2, "");
    // check if the note was updated
    assert(strcmp(list->list[0]->data, "Hello World! Updated") == 0);
    assert(list->list[0]->prt == 2);
    assert(strcmp(list->list[0]->group, "test") == 0);
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

void test_notes_list_remove_not_found() {
    // create a new note list
    NoteList *list = notes_list_create();
    // remove a note from the list
    notes_list_remove(list, 0);
    // check if the note was removed
    assert(list->size == 0);
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
    assert(strcmp(str, "1-test-Hello World!") == 0);
    // close the file
    fclose(file);
    // delete the list
    notes_list_delete(list);
}

void test_notes_list_load() {
    // create a new note list
    NoteList *list = notes_list_create();
    // add a note to the list
    notes_list_add(list, notes_create("Hello World!", 1, "test group"));
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
    assert(strcmp(list->list[0]->group, "test group") == 0);
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
    test_notes_list_create();
    test_notes_list_add();
    test_notes_list_add_full();
    test_notes_list_add_null();
    test_notes_list_get_by_id();
    test_notes_list_get_by_id_not_found();
    test_notes_list_get_by_idx();
    test_notes_list_get_by_idx_not_found();
    test_notes_list_get_all_by_group();
    test_notes_list_get_all_by_group_not_found();
    test_notes_list_get_groups();
    test_notes_list_update();
    test_notes_list_update_data_empty();
    test_notes_list_update_group_empty();
    test_notes_list_remove();
    test_notes_list_remove_not_found();
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

