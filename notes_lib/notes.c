
// External includes
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Local includes
#include "notes.h"

// local functions
static int digitNum(int n) { // returns the number of digits in a number
    if (n < 0) return digitNum(-n);
    if (n < 10) return 1;
    return 1 + digitNum(n / 10);
}

// Notes Implementation
Note *notes_create(char *data, int prt) {
	// validate the priority and data
	if (prt < 0 || prt > NOTE_PRIORITY_MAX) return NULL;
	if (data == NULL) return NULL;
	// alocate memory for the new note
	Note *note = (Note *)malloc(sizeof note);
	// set the note data
	// allocate memory for the data
	note->data = (char *)malloc(sizeof(char) * (strlen(data) + 1));
	// copy the data to the note
	strcpy(note->data, data);
	// set the priority
	note->prt = prt;
	// set the id
	note->id = 0;
	// return the note
	return note;
}

char *notes_read(Note *note, char sepator) {
	// alocate memory acording to the note data size, priority and id
	// calculate the size of the id string knowing that the id is an integer that can have more than 1 digit
	int id_size = digitNum(note->id);
	// aloocate memory for the string in the format: id + separator + priority + separator + data
	char *str = (char *)malloc(sizeof(char) * (id_size + 1 + 1 + 1 + strlen(note->data) + 1));
	// convert the note to a string and save it in the given string pointer
	sprintf(str, "%d%c%d%c%s", note->id, sepator, note->prt, sepator, note->data);
	// return the string
	return str;
}

void notes_update(Note *note, char *data, int prt) {
	// update the note data if the new data is not NULL
	if (data != NULL) {
		// free the old data
		free(note->data);
		note->data = NULL;
		// allocate memory for the new data
		note->data = (char *)malloc(sizeof(char) * (strlen(data) + 1));
		// copy the data to the note
		strcpy(note->data, data);
	}
	// update the note priority if the new priority is not NULL
	if (prt >= NOTE_PRIORITY_MIN && prt <= NOTE_PRIORITY_MAX) {
		note->prt = prt;
	}
}

void notes_delete(Note *note) {
	// free the note data
	free(note->data);
	note->data = NULL;
	// free the note
	free(note);
	note = NULL;
}

// Notes List Implementation
NoteList *notes_list_create() {
	// create a new NoteList object with a min capacity of 10
	// alocate memory for the new note list
	NoteList *list = (NoteList *)malloc(sizeof list);
	// set the list data
	list->list = (Note **)malloc(sizeof(Note *) * NOTE_LIST_INITIAL_CAPACITY);
	list->last_id = 0;
	list->size = 0;
	list->capacity = NOTE_LIST_INITIAL_CAPACITY;
	// return the list
	return list;
}

char *notes_list_read(NoteList *list) {
    // allocate memory for the string and add the note strings to it at the same time through realloc
    char *str = (char *)malloc(sizeof(char));
    // check if malloc was successful
    if (str == NULL) {
        // handle malloc failure
        return NULL;
    }
    // set the string to an empty string
    str[0] = '\0';
    // iterate through the list
    for (int i = 0; i < list->size; i++) {
        // get the note string
        char *note_str = notes_read(list->list[i], ' ');
        // check if notes_read was successful
        if (note_str == NULL) {
            // handle notes_read failure
            free(str);
			str = NULL;
            return NULL;
        }
        // reallocate memory for the string
        char *temp = (char *)realloc(str, sizeof(char) * (strlen(str) + strlen(note_str) + 2));
        // check if realloc was successful
        if (temp == NULL) {
            // handle realloc failure
            free(str);
			str = NULL;
            free(note_str);
			note_str = NULL;
            return NULL;
        }
        str = temp;
        // add a end of line character if it is not the first note
        if (i != 0) strcat(str, "\n");
        // add the note string to the string
        strcat(str, note_str);
        // free the note string
        free(note_str);
		note_str = NULL;
    }
    // return the string
    return str;
}

Note *notes_list_get(NoteList *list, int id) {
	// get the note from the list
	// find the note
	int index = -1;
	for (int i = 0; i < list->size; i++) {
		if (list->list[i]->id == id) {
			index = i;
			break;
		}
	}
	// if the note was not found, return
	if (index == -1) return NULL;
	// return the note
	return list->list[index];
}

void notes_list_add(NoteList *list, Note *note) {
	// add the note to the list
	// check if the list is full
	if (list->size == list->capacity) {
		// if it is, double the capacity
		list->capacity *= 2;
		// realocate memory for the list
		list->list = (Note **)realloc(list->list, sizeof(Note *) * list->capacity);
	}
	// add the note to the list
	list->list[list->size] = note;
	// set the note id
	list->list[list->size]->id = list->last_id;
	list->last_id++;
	list->size++;
}

void notes_list_remove(NoteList *list, int id) {
	// remove the note from the list
	// find the note
	int index = -1;
	for (int i = 0; i < list->size; i++) {
		if (list->list[i]->id == id) {
			index = i;
			break;
		}
	}
	// if the note was not found, return
	if (index == -1) return;
	// remove the note
	notes_delete(list->list[index]);
	// move the notes after the removed note to the left
	for (int i = index; i < list->size - 1; i++) {
		list->list[i] = list->list[i + 1];
	}
	// set the last note to NULL
	list->list[list->size - 1] = NULL;
	// decrease the size
	list->size--;
}

void notes_list_update(NoteList *list, int id, char *data, int prt) {
	// check if the id is valid
	if (id < 0) return;
	// update the note from the list
	// find the note with the given id
	int index = -1;
	for (int i = 0; i < list->size; i++) {
		if (list->list[i]->id == id) {
			index = i;
			break;
		}
	}
	// if the note was not found, return
	if (index == -1) return;
	// update the note
	notes_update(list->list[index], data, prt);
}

void notes_list_delete(NoteList *list) {
	// delete the list
	// delete all the notes
	for (int i = 0; i < list->size; i++) {
		notes_delete(list->list[i]);
		// set the note to NULL
		list->list[i] = NULL;
	}
	// free the list data
	free(list->list);
	list->list = NULL;
	// free the list
	free(list);
	list = NULL;
}

void notes_list_foreach(NoteList *list, void (*func)(Note *)) {
	// apply the function to each note in the list
	for (int i = 0; i < list->size; i++) {
		func(list->list[i]);
	}
}

// File Management
void notes_list_save(NoteList *list, char *path, char delimiter) {
	// save the list to the file
	// open the file
	FILE *file = fopen(path, "w");
	// write the list to the file
	for (int i = 0; i < list->size; i++) {
		fprintf(file, "%d%c%s\n", list->list[i]->prt, delimiter, list->list[i]->data);
	}
	// close the file
	fclose(file);
}

NoteList *notes_list_load(char *path, char delimiter) {
	// load the list from the file
	// open the file
	FILE *file = fopen(path, "r");
	// create a new list
	NoteList *list = notes_list_create();
	// read the file
	int prt;
	char data[255];
	char *format = "%d%c%[^\n]";
	while (fscanf(file, format, &prt, &delimiter, data) != EOF) {
		// add the note to the list
		// create the note
		Note *note = notes_create(data, prt);
		// check if the note was created, if not, continue
		if (note == NULL) continue;
		// add the note to the list
		notes_list_add(list, note);
	}
	// close the file
	fclose(file);
	// return the list
	return list;
}