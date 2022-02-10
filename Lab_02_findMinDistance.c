#include <stdio.h>
#include <string.h>

void printResultsForFindMinDistance(int minDistance, size_t stringLength, char minDistanceRepeatedChar) {
    if (stringLength > minDistance) {
        printf("Repeated char = %c, min distance = %d\n", minDistanceRepeatedChar, minDistance);
        return;
    }
    printf("No repeats Found!\n");
}

void findMinDistance(char *inputString) {
    int i, j;
    int currentDistance;
    size_t minDistance = strlen(inputString);
    char minDistanceRepeatedChar;
    for (i = 0; inputString[i] != '\0'; i++) {
        currentDistance = 0;
        for (j = i + 1; inputString[j] != '\0'; j++) {
            if (inputString[i] == inputString[j] && minDistance > currentDistance) {
                minDistanceRepeatedChar = inputString[i];
                minDistance = currentDistance;
                break;
            }
            currentDistance++;
        }
    }
    printResultsForFindMinDistance(minDistance, strlen(inputString), minDistanceRepeatedChar);
}


int main() {
    char * test1 = " "; // No repeats found!
    char * test2 = "abc"; // No repeats found!
    char * test3 = "babbab"; // Repeated char = b, min distance = 0
    char * test4 = "baeiob"; // Repeated char = b, min distance = 4
    char * test5 = "bahba"; // a or b, 2
    char * test6 = " e "; // Repeated char = , min distance = 1
    findMinDistance(test1);
    findMinDistance(test2);
    findMinDistance(test3);
    findMinDistance(test4);
    findMinDistance(test5);
    findMinDistance(test6);
    return 0;
}