
typedef struct group {
    char *name;         // name of the group
    void *list;    // list of notes in the group
} Group;

// Function to create a new group
Group* createGroup(const char* name);

// Function to add a note to a group
void addNoteToGroup(Group* group, void* note);

// Function to remove a note from a group
void removeNoteFromGroup(Group* group, void* note);

// Function to delete a group
void deleteGroup(Group* group);

