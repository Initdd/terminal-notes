
typedef struct group {
    char *name;         // name of the group
    void *list;    // list of notes in the group
} Group;

// Create a new group
Group* createGroup(const char* name);

// Add a note to a group
void addNoteToGroup(Group* group, void* note);

// Remove a note from a group
void removeNoteFromGroup(Group* group, void* note);

// Delete a group
void deleteGroup(Group* group);

