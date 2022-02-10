#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define TEST_ARRAY_SIZE 5

bool intCompare(void *a, void *b) {
    int firstValue = *(int *)a;
    int secondValue = *(int *)b;
    return (firstValue > secondValue);
}

bool floatCompare(void *a, void *b) {
    float firstValue = *(float *)a;
    float secondValue = *(float *)b;
    return (firstValue > secondValue);
}

bool longCompare(void *a, void *b) {
    long firstValue = *(long *)a;
    long secondValue = *(long *)b;
    return (firstValue > secondValue);
}

bool doubleCompare(void *a, void *b) {
    double firstValue = *(double *)a;
    double secondValue = *(double *)b;
    return (firstValue > secondValue);
}

bool charCompare(void *a, void *b) {
    char firstValue = *(char *)a;
    char secondValue = *(char *)b;
    return (firstValue > secondValue);
}

int compareValueOfTwoGenerics(void *a, void *b, bool (*compare)(void *, void*)) {
    if (compare(a, b)) {
        return 1;
    }
    return 0;
}

void genericSwap(void *first, void *second, size_t numberOfBytes) {
    char temp[numberOfBytes];
    memcpy(temp, first, numberOfBytes);
    memcpy(first, second, numberOfBytes);
    memcpy(second, temp, numberOfBytes);
}

void *findNthElement(void *array, int n, size_t sizeOfEachElementInBytes) {
    return (char *)array + n * sizeOfEachElementInBytes;
}

void genericBubbleSort(void *array, size_t size, size_t typeSize, bool (*compare)(void *, void*)) {
    while (true) {
        bool swapped = false;
        for (int i = 1; i < size; i++) {
            char* lastElement = findNthElement(array, i - 1, typeSize);
            char* currentElement = findNthElement(array, i, typeSize);
            if (compareValueOfTwoGenerics(lastElement, currentElement, compare)) {
                swapped = true;
                genericSwap(lastElement, currentElement, typeSize);
            }
        }
        if (!swapped) {
            return;
        }
    }
}

void printInt(void* number) {
    int *toPrint = (int *)number;
    printf("%d\n", *toPrint);
}

void printFloat(void* number) {
    float *toPrint = (float *)number;
    printf("%f\n", *toPrint);
}

void printLong(void* number) {
    long *toPrint = (long *)number;
    printf("%ld\n", *toPrint);
}

void printDouble(void* number) {
    double *toPrint = (double *)number;
    printf("%f\n", *toPrint);
}

void printChar(void* character) {
    char *toPrint = (char *)character;
    printf("%c\n", *toPrint);
}

void printGenericArray(void* array, int arraySize, size_t sizeOfEachElementInBytes, void(*printType)(void*)) {
    for (int i = 0; i != arraySize; i++) {
        printType((char *)array + (i) * sizeOfEachElementInBytes);
    }
    printf("\n");
}

void testSortOnIntArray() {
    const int intTypeSize = sizeof(int);
    int array[TEST_ARRAY_SIZE] = {10, 1, -2, 5, 7};
    genericBubbleSort(array, TEST_ARRAY_SIZE, intTypeSize, intCompare);
    printGenericArray(array, TEST_ARRAY_SIZE, intTypeSize, printInt);
}

void testSortOnFloatArray() {
    const int floatTypeSize = sizeof(float);
    float floatArray[TEST_ARRAY_SIZE] = {3.544, 5.544, 6.544, 6.544, 1.1};
    genericBubbleSort(floatArray, TEST_ARRAY_SIZE, floatTypeSize, floatCompare);
    printGenericArray(floatArray, TEST_ARRAY_SIZE, floatTypeSize, printFloat);
}

void testSortOnLongArray() {
    const int longTypeSize = sizeof(long);
    long longArray[TEST_ARRAY_SIZE] = {10, 1, -2, 5, 7};
    genericBubbleSort(longArray, TEST_ARRAY_SIZE, longTypeSize, longCompare);
    printGenericArray(longArray, TEST_ARRAY_SIZE, longTypeSize, printLong);
}

void testSortOnDoubleArray() {
    const int doubleTypeSize = sizeof(double);
    double doubleArray[TEST_ARRAY_SIZE] = {10, 1, -2, 5, 7};
    genericBubbleSort(doubleArray, TEST_ARRAY_SIZE, doubleTypeSize, doubleCompare);
    printGenericArray(doubleArray, TEST_ARRAY_SIZE, doubleTypeSize, printDouble);
}

void testSortOnCharArray() {
    const int charTypeSize = sizeof(char);
    char charArray[TEST_ARRAY_SIZE] = "abcde";
    genericBubbleSort(charArray, TEST_ARRAY_SIZE, charTypeSize, charCompare);
    printGenericArray(charArray, TEST_ARRAY_SIZE, charTypeSize, printChar);
}

void TestGenericSort() {
    testSortOnIntArray();
    testSortOnFloatArray();
    testSortOnLongArray();
    testSortOnDoubleArray();
    testSortOnCharArray();
}
int main() {
    TestGenericSort();
    return 0;
}
