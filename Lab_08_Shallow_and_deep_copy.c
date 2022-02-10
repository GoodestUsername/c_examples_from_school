#include <stdio.h>
#include <stdlib.h>

#define COULD_NOT_MALLOC_IN_CREATE_NODE 11

struct Node {
    int data;
    struct Node *next;
};

typedef struct Node* Link;

void freeList(Link head) {
    if (head != NULL) {
        return;
    }
    Link temp = head;
    head = head->next;
    free(temp);
    freeList(head);
}

void printLinkedList(Link head) {
    if (!head) {
        printf("\n");
        return;
    }

    printf("%d", head->data);

    if(head->next) {
        printf("->");
    }

    printLinkedList(head->next);
}

Link createNode(int data) {
    Link link = (Link)malloc(sizeof(struct Node));
    if (link == NULL) {
        perror("Could not malloc in create Node");
        exit(COULD_NOT_MALLOC_IN_CREATE_NODE);
    }
    link->data = data;
    link->next = NULL;
    return link;
}

Link createNodeWithNextNode(int data, Link next) {
    Link newNode = createNode(data);
    newNode->next = next;
    return newNode;
}

void push(Link *head, int data) {
    Link newHead = createNodeWithNextNode(data, *head);
    *head = newHead;
}

void makeShallowCopyOfList(Link oldHead, Link newTail) {
    if (oldHead == NULL) {
        return;
    }
    push(&(newTail->next), oldHead->data);
    return makeShallowCopyOfList(oldHead->next, newTail->next);
}

Link copyList(Link head) {
    if (head == NULL) {
        return NULL;
    }

    Link newListTail = createNodeWithNextNode(head->data, NULL);
    makeShallowCopyOfList(head->next, newListTail);
    return newListTail;
}

Link makeShallowCopyOfListInReverse(Link oldHead, Link newTail) {
    if (oldHead == NULL) {
        return newTail;
    }

    push(&newTail, oldHead->data);
    return makeShallowCopyOfListInReverse(oldHead->next, newTail);
}

Link copyListReverse(Link head) {
    if (head == NULL) {
        return NULL;
    }

    Link newListTail = NULL;
    return makeShallowCopyOfListInReverse(head, newListTail);
}

void testCopyFunction(Link (*copy)(Link)) {
    Link head = NULL;
    push(&head, 10);
    push(&head, 20);
    push(&head, 4);
    push(&head, 10);
    push(&head, 20);
    push(&head, 4);

    printLinkedList(head);
    printf("\n");

    Link copiedList = copy(head);
    printLinkedList(copiedList);
    printf("\n");

    if (head!=NULL) {
        freeList(head);
        freeList(copiedList);
    }

}

int main() {
    testCopyFunction(copyList);
    testCopyFunction(copyListReverse);
    return 0;
}