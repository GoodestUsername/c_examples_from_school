#include <stdio.h>
#include <stdlib.h>

#define COULD_NOT_MALLOC_IN_CREATE_NODE 11
#define NEXT_IS_NULL_IN_ADD_AS_HEAD 12
#define EMPTY_LIST_PASSED 13
#define NODE_TO_MOVE_IS_NULL 14

typedef struct MMU* Link;

struct MMU { // memory management unit
    char nodeType;
    int processNumber;
    int base;
    int limit;
    Link next;
};

void freeLinkedList(Link *head) {
    if (!*head) {
        return;
    }

    Link oldHead = *head;
    *head = (*head)->next;
    free(oldHead);
    freeLinkedList(head);
}

void printMemory(Link head) {
    for (int headNodeNumber = 1; head; head = head->next, headNodeNumber++) {
        if (head->nodeType == 'P') {
            printf("Node %d: %c%d, base = %d, limit = %d\n",
                   headNodeNumber, head->nodeType, head->processNumber, head->base, head->limit);
        }
        else {
            printf("Node %d: %c (Hole), base = %d, limit = %d\n",
                   headNodeNumber, head->nodeType, head->base, head->limit);
        }
        printf("\n");
    }
}

Link createNode(char nodeType, int processNumber, int base, int limit) {
    Link link = (Link) malloc(sizeof(struct MMU));
    if (!link) {
        perror("Could note malloc new Node in CreateNode");
        exit(COULD_NOT_MALLOC_IN_CREATE_NODE);
    }
    link->nodeType = nodeType;
    link->processNumber = processNumber;
    link->base = base;
    link->limit = limit;
    return link;
}

Link createNodeWithNextNode(char nodeType, int processNumber, int base, int limit,  Link next) {
    Link link = createNode(nodeType, processNumber, base, limit);
    link->next = next;
    return link;
}

void push(Link *head, char nodeType, int processNumber, int base, int limit) {
    Link newHead = createNodeWithNextNode(nodeType, processNumber, base, limit, *head);
    *head = newHead;
}

void pop(Link head) {
    if (!head) {
        perror("Empty list is passed to pop function\n");
        exit(EMPTY_LIST_PASSED);
    }

    Link oldHead = head->next;
    head->next = head->next->next;
    free(oldHead);
}

void combineCurrentAndNextFreeBlock(Link head) {
    if (!head) {
        perror("Empty list is passed to combine combineCurrentAndNextFreeBlock function\n");
        exit(EMPTY_LIST_PASSED);
    }

    head->limit = head->limit + head->next->limit;
    pop(head);
}

void mergeFreeBlocks(Link head) {
    if (!head || !head->next) {
        return;
    }

    if (head->nodeType != 'H' || head->next->nodeType != 'H') {
        mergeFreeBlocks(head->next);
    }

    if (head->nodeType == 'H' && head->next->nodeType == 'H') {
        combineCurrentAndNextFreeBlock(head);
        mergeFreeBlocks(head);
    }
}

Link addAsHead(Link head, Link next) {
    if (!next) {
        perror("Next cannot be null");
        exit(NEXT_IS_NULL_IN_ADD_AS_HEAD);
    }
    next->next = head;
    return next;
}

void moveNodeToOtherHead(Link *nodeToMove, Link *head) {
    if (!head) {
        perror("nodeToMove cannot be null");
        exit(NODE_TO_MOVE_IS_NULL);
    }

    Link oldHead = (*nodeToMove);
    (*nodeToMove) = (*nodeToMove)->next;
    (*head) = addAsHead((*head), oldHead);
}

void extractHolesFromFront(Link *headToExtractFrom, Link *headToExtractTo) {
    if (!(*headToExtractFrom) || (*headToExtractFrom)->nodeType != 'H') {
        return;
    }
    moveNodeToOtherHead(headToExtractFrom, headToExtractTo);
    extractHolesFromFront(headToExtractFrom, headToExtractTo);
}

Link extractHoles(Link *head, Link *startOfFreeBlocks) {
    extractHolesFromFront(head, startOfFreeBlocks);

    if ((*startOfFreeBlocks)) {
        (*startOfFreeBlocks)->base = 0;
    }

    Link newHead = (*head);

    while (*head && (*head)->nodeType == 'P' && (*head)->next) {
        if ((*head)->next->nodeType == 'H') {
            moveNodeToOtherHead(&(*head)->next, startOfFreeBlocks);
        }
        else {
            (*head)->next->base = (*head)->base + (*head)->limit;
            *head = (*head)->next;
        }
    }

    return newHead;
}

void compaction(Link *head) {
    if (!*head || !(*head)->next) {
        return;
    }

    Link startOfFreeBlocks = NULL;

    Link newHead = extractHoles(head, &startOfFreeBlocks);

    mergeFreeBlocks(startOfFreeBlocks);

    if (startOfFreeBlocks && *head) {
        startOfFreeBlocks->base = (*head)->base + (*head)->limit;
        (*head)->next = startOfFreeBlocks;
    }

    (*head) = newHead;

    if (!*head) {
        (*head) = startOfFreeBlocks;
        (*head)->base = 0;
    }
}

int main() {
    Link head = NULL;

    push(&head, 'H', 0, 26, 6);
    push(&head, 'P', 3, 16,10);
    push(&head, 'H', 0, 15, 1);
    push(&head, 'H', 0, 11, 4);
    push(&head, 'H', 0, 7, 4);
    push(&head, 'P', 2, 6,1);
    push(&head, 'P', 1, 0,6);
    printMemory(head);
    mergeFreeBlocks(head);
    compaction(&head);
    printf("\n");
    printMemory(head);

    printf("COMPACTION TESTS BEGIN HERE\n");
    printf("empty list test\n");
    printMemory(head);
    printf("\n");
    compaction(&head);
    printMemory(head);

    printf("\nOne process test\n");
    head = NULL;
    push(&head, 'P', 1, 0, 10);
    printMemory(head);
    printf("\n");
    compaction(&head);
    printMemory(head);

    printf("\ntwo process test\n");
    head = NULL;
    push(&head, 'P', 2, 10, 12);
    push(&head, 'P', 1, 0, 10);
    printMemory(head);
    printf("\n");
    compaction(&head);
    printMemory(head);

    printf("\nOne hole test\n");
    head = NULL;
    push(&head, 'H', 0, 0, 10);
    printMemory(head);
    printf("\n");
    compaction(&head);
    printMemory(head);

    printf("\ntwo hole test\n");
    head = NULL;
    push(&head, 'H', 0, 10, 12);
    push(&head, 'H', 0, 0, 10);
    printMemory(head);
    printf("\n");
    compaction(&head);
    printMemory(head);


    printf("\nlist starting with hole test\n");
    head = NULL;
    push(&head, 'H', 0, 47, 2);
    push(&head, 'P', 5, 46, 1);
    push(&head, 'P', 4, 37, 9);
    push(&head, 'H', 0, 35, 2);
    push(&head, 'H', 0, 34, 1);
    push(&head, 'H', 0, 28, 6);
    push(&head, 'P', 3, 18, 10);
    push(&head, 'H', 0, 17, 1);
    push(&head, 'P', 2, 16, 1);
    push(&head, 'P', 1, 10, 6);
    push(&head, 'H', 0, 0, 10);
    printMemory(head);
    printf("\n");
    compaction(&head);
    printf("\n");
    printMemory(head);

    printf("\nlist starting with process test\n");
    head = NULL;
    push(&head, 'H', 0, 47, 2);
    push(&head, 'P', 6, 46, 1);
    push(&head, 'P', 5, 37, 9);
    push(&head, 'H', 0, 35, 2);
    push(&head, 'H', 0, 34, 1);
    push(&head, 'H', 0, 28, 6);
    push(&head, 'P', 4, 18, 10);
    push(&head, 'H', 0, 17, 1);
    push(&head, 'P', 3, 16, 1);
    push(&head, 'P', 2, 10, 6);
    push(&head, 'P', 1, 0, 10);
    printMemory(head);
    printf("\n");
    compaction(&head);
    printMemory(head);

    printf("\nmergeFreeBlocks TESTS BEGIN HERE\n");
    printf("empty list test\n");
    head = NULL;
    printMemory(head);
    printf("\n");
    mergeFreeBlocks(head);
    printMemory(head);

    printf("\nOne process test\n");
    head = NULL;
    push(&head, 'P', 1, 0, 10);
    printMemory(head);
    printf("\n");
    mergeFreeBlocks(head);
    printMemory(head);

    printf("\ntwo process test\n");
    head = NULL;
    push(&head, 'P', 2, 10, 12);
    push(&head, 'P', 1, 0, 10);
    printMemory(head);
    printf("\n");
    mergeFreeBlocks(head);
    printMemory(head);

    printf("\nOne hole test\n");
    head = NULL;
    push(&head, 'H', 0, 0, 10);
    printMemory(head);
    printf("\n");
    mergeFreeBlocks(head);
    printMemory(head);

    printf("\ntwo hole test\n");
    head = NULL;
    push(&head, 'H', 0, 10, 12);
    push(&head, 'H', 0, 0, 10);
    printMemory(head);
    printf("\n");
    mergeFreeBlocks(head);
    printMemory(head);

    printf("\nlist starting with hole test\n");
    head = NULL;
    push(&head, 'H', 0, 49, 4);
    push(&head, 'H', 0, 47, 2);
    push(&head, 'P', 5, 46, 1);
    push(&head, 'P', 4, 37, 9);
    push(&head, 'H', 0, 35, 2);
    push(&head, 'H', 0, 34, 1);
    push(&head, 'H', 0, 28, 6);
    push(&head, 'P', 3, 18, 10);
    push(&head, 'H', 0, 17, 1);
    push(&head, 'P', 2, 16, 1);
    push(&head, 'P', 1, 10, 6);
    push(&head, 'H', 0, 0, 10);
    printMemory(head);
    printf("\n");
    mergeFreeBlocks(head);
    printMemory(head);

    printf("\nlist starting with process test\n");
    head = NULL;
    push(&head, 'H', 0, 49, 4);
    push(&head, 'H', 0, 47, 2);
    push(&head, 'P', 6, 46, 1);
    push(&head, 'P', 5, 37, 9);
    push(&head, 'H', 0, 35, 2);
    push(&head, 'H', 0, 34, 1);
    push(&head, 'H', 0, 28, 6);
    push(&head, 'P', 4, 18, 10);
    push(&head, 'H', 0, 17, 1);
    push(&head, 'P', 3, 16, 1);
    push(&head, 'P', 2, 10, 6);
    push(&head, 'P', 1, 0, 10);
    printMemory(head);
    printf("\n");
    mergeFreeBlocks(head);
    printMemory(head);
    freeLinkedList(&head);
    printf("\nIf freeLinkedList works, next print is empty\n");
    printMemory(head);
    freeLinkedList(&head);
    return 0;
}