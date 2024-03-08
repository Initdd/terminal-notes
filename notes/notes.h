
/*
 *  Notes Managing Tools
 *
 *  Includes:
 *
 *  - Note Data Structure
 *  - Note management functions
 *  - Notes list specific functions
 *  - File specific utilities to save and retreive Notes list
 * 
 *  Author: Initdd
*/

/*
 * Macros for the Note Data Structure
*/

#define NOTE_GROUP_DEFAULT "default"

/**
 * @brief Note Data Structure
 * 
 * @note A note with an identifier, priority level, data and group
*/
typedef struct note {
	int id;         // identifier for the note
	int prt;        // level of priority (0 to 2(most important))
	char *data;     // data on the note
	char *group;    // group of the note
} Note;

/**
 * @brief NoteList Data Structure
 * 
 * @note A list of Note pointers
*/
typedef struct noteList {
	Note **list;    // array of Note pointers
	int last_id;    // last id used
	int size;       // number of elements in the array
	int capacity;   // maximum number of elements in the array (not user defined)
} NoteList;

/*
 * macros for the NoteList
*/

#define NOTE_LIST_INITIAL_CAPACITY 10
#define NOTE_PRIORITY_MIN 1
#define NOTE_PRIORITY_MAX 3

/*
 *  Note Structure managing functions
 *
 *  Following the CRUD principle
*/

/**
 * @brief Create a new Note object
 * @param data the data to be stored in the note
 * @param prt the priority level of the note
 * @param group the group of the note (can be NULL)
 * @return a new Note object
 * @note the group will be set to "default" if NULL
*/
Note *notes_create(char *data, int prt, char *group);
/**
 * @brief Read the Note object
 * @param note the Note object to be read
 * @param separator the separator to be used in the string
 * @return a string with the Note data
 * @note the string must be freed after use
 * @note the string will be formated as "id prt data"
*/
char *notes_read(Note *note, char separator);
/**
 * @brief Update the Note object
 * @param note the Note object to be updated
 * @param data the new data to be stored in the note (can be NULL)
 * @param prt the new priority level of the note (can be 0 if don't want to update it)
 * @param group the new group of the note (can be NULL)
 * @note the group will be set to "default" if NULL
*/
void notes_update(Note *note, char *data, int prt, char *group);
/**
 * @brief Delete the Note object
 * @param note the Note object to be deleted
*/
void notes_delete(Note *note);


/*
 *  Notes List specific utilities
*/

/**
 * @brief Create a new NoteList object
 * @return a new NoteList object
*/
NoteList *notes_list_create();
/**
 * @brief Read the NoteList object
 * @param list the NoteList object to be read
 * @return a string with the NoteList data
 * @note the string must be freed after use
 * @note the string will be formated as "id prt data\n"
*/
char *notes_list_read(NoteList *list);
/**
 * @brief Get a Note from the NoteList by its id
 * @param list the NoteList object
 * @param id the id of the Note
 * @return the Note object with the given id
 * @note returns NULL if the Note is not found
*/
Note *notes_list_get_by_id(NoteList *list, int id);
/**
 * @brief Get a Note from the NoteList by its index
 * @param list the NoteList object
 * @param index the index of the Note
 * @return the Note object with the given index
 * @note returns NULL if the Note is not found
*/
Note *notes_list_get_by_idx(NoteList *list, int index);
/**
 * @brief Get all Notes from the NoteList by its group
 * @param list the NoteList object
 * @param group the group of the Notes
 * @return the Note object with the given group
 * @note returns NULL if the Note is not found
*/
NoteList *notes_list_get_all_by_group(NoteList *list, char *group);
/**
 * @brief Get all groups from the NoteList
 * @param list the NoteList object
 * @return a list of strings with the groups
 * @note the list must be freed after use
*/
char **notes_list_get_groups(NoteList *list);
/**
 * @brief Add a Note to the NoteList
 * @param list the NoteList object
 * @param note the Note object to be added
*/
void notes_list_add(NoteList *list, Note *note);
/**
 * @brief Remove a Note from the NoteList by its id
 * @param list the NoteList object
 * @param id the id of the Note
*/
void notes_list_remove(NoteList *list, int id);
/**
 * @brief Update a Note from the NoteList by its id
 * @param list the NoteList object
 * @param id the id of the Note
 * @param data the new data to be stored in the note (can be NULL)
 * @param prt the new priority level of the note (can be 0 if don't want to update it)
 * @param group the new group of the note (can be NULL)
 * @note the group will be set to "default" if NULL
*/
void notes_list_update(NoteList *list, int id, char *data, int prt, char *group);
/**
 * @brief Delete the NoteList object
 * @param list the NoteList object to be deleted
*/
void notes_list_delete(NoteList *list);
/**
 * @brief Free the NoteList object
 * @param list the NoteList object to be freed
 * @param func the function to be used on each Note
*/
void notes_list_foreach(NoteList *list, void (*func)(Note *));


/*
 *  File Specific Utilities
 *  
 *  Related to Notes
*/

/**
 * @brief Load a list of Notes from a file
 * @param filename the name of the file to be loaded
 * @param delimiter the delimiter used in the file
 * @return a new NoteList object
 * @note the file must be formated as "id prt data\n"
*/
NoteList *notes_list_load(char *filename, char delimiter);
/**
 * @brief Save the list to the file
 * @param list the NoteList object to be saved
 * @param filename the name of the file to be saved
 * @param delimiter the delimiter to be used in the file
 * @note the file will be formated as "id prt data\n"
*/
void notes_list_save(NoteList *list, char *filename, char delimiter);


