
/*
 *  Group Management Tools
 *
 *  Includes:
 *
 *  - Group Data Structure
 *  - Group Management Functions
 * 
 *  Author: Initdd
*/

/**
 * Group Constants
*/

#define GROUP_INITIAL_CAPACITY 10

/*
 * Group Data Structure
 * 
 * A group is a collection of data.
 * It is identified by a name.
 * 
*/

typedef struct group {
    char *name;     // name of the group
	void **list;    // array of data pointers
	int size;       // number of elements in the array
	int capacity;   // maximum number of elements in the array (not user defined)
} Group;

/*
 * Group Management Functions
*/

// Create a new group with a specified name
Group *group_create(const char *name);

// Add a item to a group
void group_add_item(Group *group, void *item);

// Remove a item from a group
void group_remove_item(Group *group, void (*deleter)(void *item), void *item, int (*cmp)(void *a, void *b));

// Update a group name
void group_update_name(Group *group, const char *name);

// Delete a group
void group_delete(Group *group, void (*deleter)(void *item));

