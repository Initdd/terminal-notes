
/**
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
 *  Version: 0.1
 */

/**
 * Note Data Structure
 * 
*/

typedef struct note {
	int id;         // identifier for the node
	int prt;        // level of priority (1 to 5(most important))
	char *data;     // data on the note
} Note;


/**
 *  Note List Data Structure
 * 
 *  An Array of Note pointers
 */

typedef struct noteList {
	Note **list;    // array of Note pointers
	int last_id;    // last id used
	int size;       // number of elements in the array
	int capacity;   // maximum number of elements in the array (not user defined)
} NoteList;


/*
 *  Note Structure managing functions
 *
 *  Following the CRUD principle
 */

// create a new Note object with the specified atributes (data and priority)
Note *notes_create(char *data, int prt);
// convert the given Note to a string
char *notes_read(Note *note, char separator);
// update the existing Note with the new values
void notes_update(Note *note, char *data, int prt);
// delete the refered Note
void notes_delete(Note *note);


/**
 *  Notes List specific utilities
 */

// create a new NoteList object
NoteList *notes_list_create();
// read the NoteList object
char *notes_list_read(NoteList *list);
// get a Note from the NoteList
Note *notes_list_get(NoteList *list, int id);
// add a Note to the NoteList
void notes_list_add(NoteList *list, Note *note);
// remove a Note from the NoteList
void notes_list_remove(NoteList *list, int id);
// update a Note from the NoteList with new values (can be NULL)
void notes_list_update(NoteList *list, int id, char *data, int prt);
// delete the NoteList object
void notes_list_delete(NoteList *list);
// iterate through the NoteList and call the given function for each Note
void notes_list_foreach(NoteList *list, void (*func)(Note *));


/**
 *  File Specific Utilities
 *  
 *  Related to Notes
 */

// load a list of Notes from a formated (with save) file
NoteList *notes_list_load(char *filename, char delimiter);
// save a list of Notes to a file
void notes_list_save(NoteList *list, char *filename, char delimiter);


