#include <stdlib.h>
#include <assert.h>
#include "groups.h"

// Constants
#define TEST_VALUE 1

int int_item_compare(void *a) {
    return *(int*)a - TEST_VALUE;
}

void test_group_create();

void test_group_add_item();

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
    group_remove_item(group, int_item_compare, NULL);

    

    // Verify that the item has been removed
}

void test_group_update_name();

void test_group_delete();

int main() {
    
}