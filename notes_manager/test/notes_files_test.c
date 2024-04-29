// general test for notes library
// generic libraries
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
// local libraries
#include "../notes_manager.h"

// generic variables
#define FILENAME "test.txt"
#define DEL '-'

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

int main() {
	// Run the tests
    test_notes_list_save();
    test_notes_list_load();
	printf("All tests passed!\n");
	return 0;
}