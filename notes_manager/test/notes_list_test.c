
// general test for notes library
// generic libraries
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
// local libraries
#include "../notes_manager.h"

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

int main() {
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
    printf("All tests passed!\n");
	return 0;
}