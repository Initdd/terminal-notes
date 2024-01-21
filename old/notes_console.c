#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DATA "/home/Initdd/.bootexec/notes/data"
#define TMP "/home/Initdd/.bootexec/notes/tmp"

char *helpString =  "Usage: %s [Options... [data...]]\n\n"
                    "No options: show notes\n\n"
                    "Options:\n"
                    "-h             - show help menu\n"
                    "-a '[note]'    - add note\n"
                    "-r [IDX]     - remove note of index IDX\n";

char *headerString =    "\nCurrent notes:\n"
                        "------------------\n";


typedef struct note {
    int idx;
    char *data;
} Note;


void print_data() {
    FILE *file = fopen(DATA, "r");

    int num;
    char note[255];

    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    printf("%s\n", headerString);

    while (fscanf(file, "%d - %[^\n]", &num, note)==2) {
        printf("[%d] => %s\n", num, note);
    }

    printf("\n");

    fclose(file);
}

void insert_data(char *data) {
    FILE *file;

    file = fopen(DATA, "a");

    if (file == NULL) {
        perror("Error: Couldn't open the file");
    }

    fprintf(file, "%s\n", data);

    fclose(file);
}


void insert_note(char data[]) {
    FILE *file = fopen(DATA, "rw");

    if (file == NULL) {
        perror("Error: Couldn't open the file");
        return;
    }

    int ch;
    int n = 0;
    int lineCounter = 0;

    // Count lines
    while ((ch = fgetc(file)) != EOF) {
        if (ch != '\n') lineCounter++;
        if (ch == '\n') n++;
    }
    if (lineCounter!=0) n++;

    // get string to print
    int maxDigits = snprintf(NULL, 0, "%d - ", n);
    char *nStr = (char *)malloc(maxDigits + 1);
    if (nStr == NULL) {
        perror("Memory allocation failed");
        return;
    }
    snprintf(nStr, maxDigits + 1, "%d - ", n);

    size_t data_size = strlen(data);
    char fdata[data_size+maxDigits+1];

    strcpy(fdata, nStr);
    strcat(fdata, data);

    insert_data(fdata);

    fclose(file);

}

void remove_data(int idx) {
    FILE *file = fopen(DATA, "r");
    FILE *tempFile = fopen(TMP, "w");

    if (file == NULL || tempFile == NULL) {
        perror("Error: Couldn't open the file");
        return;
    }

    char *line = NULL;
    size_t len = 0;
    int currentIdx = 0;

    // Read each line from the file
    while (getline(&line, &len, file) != -1) {
        int n;

        // Use sscanf to extract the number 'n' from the line
        if (sscanf(line, "%d - ", &n) == 1) {
            if (n != idx) {
                // If the current index is not the one to be removed, write the line to the temp file
                fprintf(tempFile, "%s", line);
            }
            currentIdx++;
        }
    }

    fclose(file);
    fclose(tempFile);

    // Remove the original file and rename the temporary file
    remove(DATA);
    rename(TMP, DATA);
}



char *get_user_input() {
    char *input = NULL;
    size_t bufsize = 0;
    getline(&input, &bufsize, stdin);

    // Remove newline character from input
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }

    return input;
}


int main(int argc, char *argv[]) {
    if (argc == 2 && (strcasecmp(argv[1], "-p") == 0)) {
        print_data();
        return 0;
    }

    while (1) {  // Infinite loop until user decides to exit
        printf("Enter command: ");
        char *input = get_user_input();

        if (strcasecmp(input, "exit") == 0 || strcasecmp(input, "quit") == 0) {
            printf("Exiting the program...\n");
            break;  // Exit the loop
        } else if (strcasecmp(input, "help") == 0) {
            printf("%s\n", helpString); // Display help menu
        } else if (strcasecmp(input, "print") == 0) {
            print_data(); // Print data
        } else if (strcasecmp(input, "add") == 0) {
            char note[256];
            printf("Enter note to add: ");
            fgets(note, sizeof(note), stdin);
            insert_note(note); // Add note
        } else if (strcasecmp(input, "remove") == 0) {
            int index;
            printf("Enter index to remove: ");
            scanf("%d", &index);
            remove_data(index); // Remove note
        } else {
            printf("Command not recognized. Type 'help' for help.\n");
        }

        free(input);
    }

    return 0;
};
