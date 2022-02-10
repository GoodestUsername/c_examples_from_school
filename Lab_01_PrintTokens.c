#include <stdio.h>

#define WHITESPACE ' '


int indexOfNextToken(const char* string, int currentIndex) {
    for (int i = currentIndex; string[i] != '\0'; i++) {
        if (string[i] != WHITESPACE) { return i; }
    }
    return 0;
}

void printTokens(const char *inputString) {

    for (int i = indexOfNextToken(inputString, 0); inputString[i] != '\0'; ++i) {

        char currentChar = inputString[i];
        char nextChar = inputString[i+1];

        if (!currentChar) {
            printf("\n");
            break;
        }

        if (currentChar == WHITESPACE && nextChar != WHITESPACE && nextChar != '\0') {
            printf("\n");
        }

        else if (currentChar != WHITESPACE) {
            printf("%c", currentChar);
        }
        printf("\n");
    }
}

int main() {
//    printTokens(" Hi there!  How is it  going?     ");
    printTokens("   s ");
    printTokens(" 2");
    printTokens("2 ");
    printTokens("  2");
    printTokens("2  ");
    printTokens("2");
//    printTokens("");

    return 0;
}
