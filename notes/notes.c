
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

/*
 * Note Structure managing functions
 * 
 * Following the CRUD principle
*/

Note *notes_create(char *data, int prt, char *group) {
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
	// set the group
	char *group_str = (group == NULL) ? NOTE_GROUP_DEFAULT : group;
	note->group = (char *)malloc(sizeof(char) * (strlen(group_str) + 1));
	strcpy(note->group, group_str);
	// return the note
	return note;
}

char *notes_read(Note *note, char separator) {
	// alocate memory acording to the note data size, priority and id
	// calculate the size of the id string knowing that the id is an integer that can have more than 1 digit
	int id_size = digitNum(note->id);
	int sep_size = 1;
	int priority_size = 1;
	int data_size = strlen(note->data);
	int group_size = strlen(note->group);
	// calculate the size of the resulting string
	// in the format: id:priority:group:data
	int size = id_size + sep_size + priority_size + sep_size + group_size + sep_size + data_size + 1;
	// aloocate memory for the resulting string
	char *str = (char *)malloc(sizeof(char) * (size));
	// convert the note to a string and save it in the given string pointer
	sprintf(str, "%d%c%d%c%s%c%s", note->id, separator, note->prt, separator, note->group, separator, note->data);
	// return the string
	return str;
}

void notes_update(Note *note, char *data, int prt, char *group) {
	// update the note data if the new data is not NULL
	if (data != NULL) {
		// free the old data
		free(note->data);
		// allocate memory for the new data
		note->data = (char *)malloc(sizeof(char) * (strlen(data) + 1));
		// copy the data to the note
		strcpy(note->data, data);
	}
	// update the note priority if the new priority is not NULL
	if (prt >= NOTE_PRIORITY_MIN && prt <= NOTE_PRIORITY_MAX) {
		note->prt = prt;
	}
	// update the note group if the new group is not NULL
	if (group != NULL) {
		// free the old group
		free(note->group);
		note->group = NULL;
		// allocate memory for the new group
		note->group = (char *)malloc(sizeof(char) * (strlen(group) + 1));
		// copy the group to the note
		strcpy(note->group, group);
	}
}

void notes_delete(Note *note) {
	// free the note data
	free(note->data);
	note->data = NULL;
	// free the group data
	free(note->group);
	note->group = NULL;
	// free the note
	free(note);
	note = NULL;
}

/*
 * Notes List Implementation
 * 
 * Functions to manage a list of notes
*/

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

Note *notes_list_get_by_id(NoteList *list, int id) {
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

Note *notes_list_get_by_idx(NoteList *list, int index) {
	// get the note from the list by its index
	// check if the index is valid
	if (index < 0 || index >= list->size) return NULL;
	// return the note
	return list->list[index];
}

NoteList *notes_list_get_all_by_group(NoteList *list, char *group) {
	// get all the notes from the list by their group
	// create a new list
	NoteList *new_list = notes_list_create();
	// iterate through the list
	for (int i = 0; i < list->size; i++) {
		// check if the note has the same group
		if (strcmp(list->list[i]->group, group) == 0) {
			// add the note to the new list
			notes_list_add(new_list, list->list[i]);
		}
	}
	// return the new list
	return new_list;
}

char **notes_list_get_groups(NoteList *list, int *list_size) {
	// get all the groups from the list
	// create a new list
	char **groups = (char **)malloc(sizeof(char *) * list->size);
	// iterate through the list
	int size = 0;
	for (int i = 0; i < list->size; i++) {
		//printf("%s\n", list->list[i]->group);
		// check if the group is already in the list
		int found = 0;
		for (int j = 0; j < size; j++) {
			if (strcmp(groups[j], list->list[i]->group) == 0) {
				found = 1;
				break;
			}
		}
		// if the group was not found, add it to the list
		if (!found) {
			groups[size] = (char *)malloc(sizeof(char) * (strlen(list->list[i]->group) + 1));
			strcpy(groups[size], list->list[i]->group);
			size++;
		}
	}
	// realocate memory for the list
	groups = (char **)realloc(groups, sizeof(char *) * size);
	// set the list size
	*list_size = size;
	// return the list
	return groups;
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

void notes_list_update(NoteList *list, int id, char *data, int prt, char *group) {
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
	notes_update(list->list[index], data, prt, group);
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

/**
 * File Management Tools for Notes
 * 
 * Save and load notes from a file
*/

// Save the list to the file
void notes_list_save(NoteList *list, char *path, char delimiter) {
	// save the list to the file
	// open the file
	FILE *file = fopen(path, "w");
	// write the list to the file
	for (int i = 0; i < list->size; i++) {
		// write the note to the file
		// format: priority:group:data
		fprintf(file, "%d%c%s%c%s\n", list->list[i]->prt, delimiter, list->list[i]->group, delimiter, list->list[i]->data);
	}
	// close the file
	fclose(file);
}

// Load a list of notes from a file
NoteList *notes_list_load(char *path, char delimiter) {
    // Open the file
    FILE *file = fopen(path, "r");
    if (file == NULL) {
        perror("Error opening file");
        return NULL;
    }

    // Create a new list
    NoteList *list = notes_list_create();

    // Read the file line by line
    char *line = NULL;
    size_t bufsize = 0;
    size_t linelen;
    while ((linelen = getline(&line, &bufsize, file)) != -1) {
        // Check if the line is empty or too short
        if (linelen <= 1) continue;
        
        // Remove the end of line character
        line[linelen - 1] = '\0';

        // Split the line into the note data
        char *token = strtok(line, &delimiter);
        if (token == NULL) continue; // Empty line
        
        // Get the priority
        int prt = atoi(token);

        // Get the group
        token = strtok(NULL, &delimiter);
        if (token == NULL) continue; // Missing group
        char group[255];
        strncpy(group, token, sizeof(group) - 1);
        group[sizeof(group) - 1] = '\0'; // Ensure null-terminated
        
        // Get the data
        token = strtok(NULL, &delimiter);
        if (token == NULL) continue; // Missing data
        char data[255];
        strncpy(data, token, sizeof(data) - 1);
        data[sizeof(data) - 1] = '\0'; // Ensure null-terminated

        // Create the note
        Note *note = notes_create(data, prt, group);
        if (note == NULL) continue; // Error creating note

        // Add the note to the list
        notes_list_add(list, note);
    }

    // Free dynamically allocated memory
    free(line);

    // Close the file
    fclose(file);

    // Return the list
    return list;
}