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



int main(int argc, char *argv[]) {
    if (argc == 1) {
    print_data(); // print data
    } else {
        char *opt = argv[1];

        if (strcasecmp(opt, "-H") == 0 || strcasecmp(opt, "-h") == 0) {
            printf("%s\n", helpString); // help menu
        } else if (strcasecmp(opt, "-A") == 0 || strcasecmp(opt, "-a") == 0) {
            if (argc < 3) {
                printf("No data supplied\n");
                return 1;
            } else {
                insert_note(argv[2]); // add note
            }
        } else if (strcasecmp(opt, "-R") == 0 || strcasecmp(opt, "-r") == 0) {
            if (argc < 3) {
                printf("No index Supplied\n");
                return 1;
            } else {
                remove_data(atoi(argv[2])); // remove note
            }
        } else {
            printf("Option not Recognized\n");
            return 1;
        }
    }

    return 0;

};
