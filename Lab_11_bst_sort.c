#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define COULD_NOT_MALLOC_IN_CREATE_TREE_NODE 1
#define TEST_ARRAY_SIZE 5

bool intCompare(void *a, void *b) {
    int firstValue = *(int *)a;
    int secondValue = *(int *)b;
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

bool stringCompare(void* first, void* second) {
    return strcmp(*(char**) first, *(char**) second) > 0;
}

void printInt(void* number) {
    int *toPrint = (int *)number;
    printf(" %d", *toPrint);
}

void printDouble(void *data) {
    printf(" %.2lf", *(double *)data);
}

void printChar(void* character) {
    char *toPrint = (char *)character;
    printf(" %c", *toPrint);
}

void printString(void *data) {
    printf(" %s", *(char **) data);
}

void *findNthElement(void *array, int n, size_t sizeOfEachElementInBytes) {
    return (char *)array + n * sizeOfEachElementInBytes;
}

struct Node {
    void* data;
    struct Node *left;
    struct Node *right;
};

typedef struct Node* TreeNode;

void freeBinaryTree(TreeNode root) {
    if (!root) {
        return;
    }

    freeBinaryTree(root->left);
    freeBinaryTree(root->right);
    free(root);
}

void printTreeInOrder(TreeNode root, void (*print)(void*)) {
    if (!root) {
        return;
    }

    printTreeInOrder(root->left, print);
    print(root->data);
    printTreeInOrder(root->right, print);
}

TreeNode createTreeNode(void* data) {
    TreeNode treeNode = (TreeNode) malloc(sizeof(struct Node));
    if (treeNode == NULL) {
        perror("Memory issues!");
        exit(COULD_NOT_MALLOC_IN_CREATE_TREE_NODE);
    }

    treeNode->data = data;
    treeNode->left = NULL;
    treeNode->right = NULL;
    return treeNode;
}

TreeNode insert(TreeNode root, void* data, bool (*compareMethod)(void* , void* )) {
    if (!root) {
        return createTreeNode(data);
    }

    if (!compareMethod(data, root->data)) {
        root->left = insert(root->left, data, compareMethod);
    } else {
        root->right = insert(root->right, data, compareMethod);
    }

    return root;
}

void bstSort(void* genericArray, size_t size, size_t typeSize, bool (*compareMethod)(void* , void* ), void (*printMethod)(void*)) {
    TreeNode head = NULL;
    for (int i = 0; i < size; i ++) {
        char* currentElement = findNthElement(genericArray, i, typeSize);
        head = insert(head, currentElement, compareMethod);
    }

    printTreeInOrder(head, printMethod);

    freeBinaryTree(head);
}

int main() {
    int integerNumbers[] = {1, 3, 4, 2, 5};
    bstSort(integerNumbers, TEST_ARRAY_SIZE, sizeof(int), intCompare, printInt);

    printf("\n");

    double doubleNumbers[] = {1.0, 3.0, 5.0, 2.0, 4.0};
    bstSort(doubleNumbers, TEST_ARRAY_SIZE, sizeof(double), doubleCompare, printDouble);

    printf("\n");

    char letters[] = {'e', 'b', 'a', 'd', 'c'};
    bstSort(letters, TEST_ARRAY_SIZE, sizeof(char), charCompare, printChar);

    printf("\n");

    char* words[] = {"Seyed", "Jack", "Vicky", "123", "????"};
    bstSort(words, TEST_ARRAY_SIZE, sizeof(char *), stringCompare, printString);

    return 0;
}