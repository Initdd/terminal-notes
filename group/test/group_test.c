
// general includes
#include <stdlib.h>
#include <assert.h>
#include <string.h>
// local includes
#include "../group.h"

// Constants
#define TEST_VALUE 1

int int_item_compare(void *a, void *b) {
    return *(int*)a - *(int*)b;
}

void int_item_delete(void *a) {
    // Free the memory allocated for the item
    free((int*)a);
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
    int *item = (int *)malloc(sizeof item);
    *item = TEST_VALUE;
    group_add_item(group, item);
    // Verify that the item has been added
    assert(group->size == 1);
    // Verify that the item has been added to the list
    assert(group->list[0] == item);
    // Remove the group
    group_delete(group, int_item_delete);
}

void test_group_remove_item() {
    // Create the group
    Group *group = group_create("Test Group");
    // Add some items to the group
    int *item = (int *)malloc(sizeof item);
    *item = TEST_VALUE;
    group_add_item(group, item);
    item = (int *)malloc(sizeof item);
    *item = TEST_VALUE;
    group_add_item(group, item);
    item = (int *)malloc(sizeof item);
    *item = TEST_VALUE;
    group_add_item(group, item);
    // Remove an item from the group using the remove_item function
    // group, int compare function, item delete function
    group_remove_item(group, int_item_delete, &item, int_item_compare);
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
    int *item = (int *)malloc(sizeof item);
    *item = TEST_VALUE;
    group_add_item(group, item);
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