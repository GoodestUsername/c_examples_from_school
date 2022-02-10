#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#define TOO_MANY_ARGS_ERROR                     1
#define NOT_ENOUGH_ARGS_ERROR                   2
#define CANT_OPEN_FILE 1234
#define MAXIMUM_CHAR_PER_LINE 500

int doesStringStartWith(char *string, char *pattern) {
    if (strlen(pattern) == 0) {
        return true;
    }
    return (*string == *pattern) && doesStringStartWith(string+1,pattern+1);
}

int doesStringContainPattern(char*string,char*pattern) {
    int charsProcessed = 0;
    if (strlen(pattern) == 0) {
        return true;
    }
    while (*string) {
        charsProcessed++;
        if(doesStringStartWith(string++, pattern)) {
            return charsProcessed;
        }
    }
    return 0;
}

FILE *processFile(char* fileName) {
    FILE *file = fopen(fileName, "r");
    if (file == NULL) {
        perror("Could not open file for reading;.");
        exit(CANT_OPEN_FILE);
    }
    return file;
}

void findWordInFile(char* fileName, char *word) {
    FILE *file = processFile(fileName);
    char line[MAXIMUM_CHAR_PER_LINE];
    int charPosition;
    int linePosition;
    for (linePosition = 1; fgets(line, MAXIMUM_CHAR_PER_LINE, file) != NULL; linePosition++) {
        charPosition = doesStringContainPattern(line, word);
        if (charPosition) {
            printf("Line: %d, character: %d", linePosition, charPosition);
            return;
        }
    }
    printf("Not Found!");
    fclose(file);
}



int main(int argc, char *argv[]) {
    if (argc > 2) {
        perror("Too many arguments, must have only file name.\n");
        exit(TOO_MANY_ARGS_ERROR);
    }
    if (2 > argc) {
        perror("Not enough arguments error, must have the file name.\n");
        exit(NOT_ENOUGH_ARGS_ERROR);
    }
    findWordInFile(argv[1], argv[2]);
    return 0;
}

