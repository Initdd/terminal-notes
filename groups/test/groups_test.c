
// general includes
#include <stdlib.h>
#include <assert.h>
#include <string.h>
// local includes
#include "../groups.h"

// Constants
#define TEST_VALUE 1

int int_item_compare(void *a) {
    return *(int*)a - TEST_VALUE;
}

void int_item_delete(void *a) {
    // Nothing to do here
    return;
}

void test_group_create() {
    // Create a group
    Group *group = group_create("Test Group");
    // Verify that the group has been created
    assert(group != NULL);
    // Verify that the group has the correct name
    assert(strcmp(group->name, "Test Group") == 0);
    // Verify that the group has no items
    assert(group->size == 0);
    // Verify that the group list is not NULL
    assert(group->list != NULL);
    // Remove the group
    group_delete(group, int_item_delete);
}

void test_group_add_item() {
    // Create the group
    Group *group = group_create("Test Group");
    // Add an item to the group
    int item = 1;
    group_add_item(group, &item);
    // Verify that the item has been added
    assert(group->size == 1);
    // Verify that the item has been added to the list
    assert(group->list[0] == &item);
    // Remove the group
    group_delete(group, int_item_delete);
}

void test_group_remove_item() {
    // Create the group
    Group *group = (Group *)malloc(sizeof group);
    group = group_create("Test Group");
    // Add some items to the group
    int item = 1;
    group_add_item(group, &item);
    int item2 = 2;
    group_add_item(group, &item2);
    int item3 = 3;
    group_add_item(group, &item3);
    // Remove an item from the group using the remove_item function
    // group, int compare function, item delete function
    group_remove_item(group, int_item_compare, int_item_delete);
    // Verify that the item has been removed
    assert(group->size == 2);
    // Remove the group
    group_delete(group, int_item_delete);
}

void test_group_update_name() {
    // Create the group
    Group *group = group_create("Test Group");
    // Update the group name
    group_update_name(group, "New Group");
    // Verify that the group name has been updated
    assert(strcmp(group->name, "New Group") == 0);
    // Remove the group
    group_delete(group, int_item_delete);
}

void test_group_delete() {
    // Create the group
    Group *group = group_create("Test Group");
    // Save the address of the group
    int group_address = (long)group;
    // Add an item to the group
    int item = 1;
    group_add_item(group, &item);
    // Delete the group
    group_delete(group, int_item_delete);
    // Verify that the group has been deleted
    assert((long)group != group_address);
}

int main() {
    test_group_create();
    test_group_add_item();
    test_group_remove_item();
    test_group_update_name();
    test_group_delete();
    return 0;
}