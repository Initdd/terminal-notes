
// global libraries

// local libraries
#include "../group/group.h"
#include "notes.h"

// Constants

// Functions
void notes_group_deleter(void *item) {
    notes_delete((Note *)item);
}

int notes_compare_id(void *a, void *b) {
    return ((Note *)a)->id == ((Note *)b)->id;
}

// Notes Groups
// Create a new group with a specified name
Group *notes_group_create(const char *name) {
    return group_create(name);
}

// Add a item to a group
void notes_group_add_item(Group *group, Note *item) {
    group_add_item(group, item);
}

// Remove a item from a group
void notes_group_remove_item(Group *group, int id) {
    group_remove_item(group, notes_group_deleter, &id, notes_compare_id);
}

// Update a group name
void notes_group_update_name(Group *group, const char *name) {
    group_update_name(group, name);
}

// Delete a group
void notes_group_delete(Group *group, Note (*deleter)(Note *item)) {
    group_delete(group, notes_group_deleter);
}
