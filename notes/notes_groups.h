
/*
 * Notes Groups Header
 * 
 * This file contains the function prototypes for the notes groups
 * 
 * Author: Initdd
*/

// local libraries
#include "../group/group.h"
#include "notes.h"

/*
 * Group Functions
 * 
 * These functions are used to create, update, and delete groups of notes
 * 
 * Author: Initdd
*/

// Create a new group with a specified name
Group *notes_group_create(const char *name);
// Add a item to a group
void notes_group_add_item(Group *group, Note *item);
// Remove a item from a group
void notes_group_remove_item(Group *group, int id);
// Update a group name
void notes_group_update_name(Group *group, const char *name);
// Delete a group
void notes_group_delete(Group *group, Note (*deleter)(Note *item));

