
// general includes
#include <stdlib.h>
#include <string.h>
// local includes
#include "group.h"

// local functions
// ? no for now

// Groups Implementation
Group *group_create(const char *name) {
    // validate the name
    if (name == NULL) return NULL;
    // alocate memory for the new group
    Group *group = (Group *)malloc(sizeof group);
    // set the group name
    // allocate memory for the name
    group->name = (char *)malloc(sizeof(char) * (strlen(name) + 1));
    // copy the name to the group
    strcpy(group->name, name);
	// set the group size and capacity
	group->size = 0;
	group->capacity = GROUP_INITIAL_CAPACITY;
	// allocate memory for the list
	group->list = (void **)malloc(sizeof(void *) * group->capacity);
    // return the group
    return group;
}

// add the item to the group list of data
void group_add_item(Group *group, void *item) {
	// add the item to the list
	// check if the list is full
	if (group->size == group->capacity) {
		// if it is, double the capacity
		group->capacity *= 2;
		// realocate memory for the list
		group->list = (void **)realloc(group->list, sizeof(void *) *group->capacity);
	}
	// add the item to the list
	group->list[group->size] = item;
	// set the item id
	group->size++;
}

// Remove a item from a group by a given checker function
void group_remove_item(Group *group, void (*deleter)(void *item), void *item, int (*cmp)(void *a, void *b)) {
	// remove the item from the list
	// find the item index in the list
	int index = -1;
	for (int i = 0; i < group->size; i++) {
		if (cmp(group->list[i], item)) {
			index = i;
			break;
		}
	}
	// if the item was not found, return
	if (index == -1) return;
	// remove the item
	deleter(group->list[index]);
	// move the items after the removed item to the left
	for (int i = index; i < group->size - 1; i++) {
		group->list[i] = group->list[i + 1];
	}
	// set the last item to NULL
	group->list[group->size - 1] = NULL;
	// decrease the size
	group->size--;
}

// Update a group
void group_update_name(Group *group, const char *name) {
	// validate the name
	if (name == NULL) return;
	// free the old name
	free(group->name);
	// set the group name
	// allocate memory for the name
	group->name = (char *)malloc(sizeof(char) * (strlen(name) + 1));
	// copy the name to the group
	strcpy(group->name, name);
}

// Delete a group and all its items
void group_delete(Group *group, void (*deleter)(void *item)) {
	// delete the group
	// delete all the items
	for (int i = 0; i < group->size; i++) {
		deleter(group->list[i]);
	}
	// free the name
	free(group->name);
	// free the list
	free(group->list);
	// free the group
	free(group);
}





