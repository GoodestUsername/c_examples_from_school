#include <stdio.h>
#include <stdlib.h>

#define OPERAND_IS_NULL_ERROR 1
#define COULD_NOT_MALLOC_IN_CREATE_NODE 2

struct Node {
    int data;
    struct Node *next;
    struct Node *prev;
};

typedef struct Node* DLink;

DLink createNode(int data, DLink next, DLink prev) {
    DLink link = (DLink) malloc(sizeof(struct Node));
    if (link == NULL) {
        perror("Could not malloc in createNode!");
        exit(COULD_NOT_MALLOC_IN_CREATE_NODE);
    }

    link->data = data;
    link->next = next;
    link->prev = prev;
    return link;
}

void printLinkedList(DLink head) {
    if (!head) {
        printf("\n");
        return;
    }

    printf("%d", head->data);
    if (head->next) {
        printf("<->");
    }

    printLinkedList(head->next);
}

void push(DLink *head, int data) {
    DLink newHead = createNode(data, *head, NULL);
    if (*head) {
        (*head)->prev = newHead;
    }
    *head = newHead;
}

int addTwoSingleDigitNodes(DLink nodeOne, DLink nodeTwo, int carry, DLink *sum) {
    if (!nodeOne && !nodeTwo) {
        return 0;
    }

    if (!nodeOne) {
        push(sum, nodeTwo->data + carry);
        return 0;
    }

    if (!nodeTwo) {
        push(sum, nodeOne->data + carry);
        return 0;
    }

    int dataSum = nodeOne->data + nodeTwo->data + carry;

    push(sum, dataSum % 10);

    return dataSum / 10;
}

void moveTwoLinkedListsToLastNode(DLink *firstHead, DLink *secondHead) {
    while ((*firstHead)->next || (*secondHead)->next) {

        if ((*firstHead)->next) (*firstHead) = (*firstHead)->next;
        if ((*secondHead)->next) (*secondHead) = (*secondHead)->next;
    }
}

void sumFromLastNodeToFirst(DLink firstNumber, DLink secondNumber, DLink *sum) {
    int carry = 0;

    while (firstNumber || secondNumber) {
        carry = addTwoSingleDigitNodes(firstNumber, secondNumber, carry, sum);

        if (firstNumber) firstNumber = firstNumber->prev;
        if (secondNumber) secondNumber = secondNumber->prev;
    }

    if (carry) push(sum, carry);
}

DLink sum(DLink firstNumber, DLink secondNumber) {
    if (!firstNumber || !secondNumber) {
        perror("One of the numbers is null in sum");
        exit(OPERAND_IS_NULL_ERROR);
    }
    
    DLink sum = NULL;

    moveTwoLinkedListsToLastNode(&firstNumber, &secondNumber);
    sumFromLastNodeToFirst(firstNumber, secondNumber, &sum);

    return sum;
}
void testCaseBothZero() {
    printf("**Test case where both numbers are zero**\n");
    DLink first = NULL;
    push(&first, 0);
    printf("First Number:\n");
    printLinkedList(first);
    DLink second = NULL;
    push(&second, 0);
    printf("Second Number:\n");
    printLinkedList(second);
    DLink sumOfLists = sum(first, second);
    printf("Sum of lists:\n");
    printLinkedList(sumOfLists);
    printf("\n");
}
void testCaseRemainderCarryDigit() {
    printf("**Test case where resulting sum is zero with remainder carry digit**\n");
    DLink first = NULL;
    push(&first, 1);
    push(&first, 0);
    push(&first, 9);
    printf("First Number:\n");
    printLinkedList(first);
    DLink second = NULL;
    push(&second, 9);
    push(&second, 9);
    printf("Second Number:\n");
    printLinkedList(second);
    DLink sumOfLists = sum(first, second);
    printf("Sum of lists:\n");
    printLinkedList(sumOfLists);
    printf("\n");
}
void testCaseSecondNumberLongerThanFirstNumber() {
    printf("**Test case where second list is longer than first list**\n");
    DLink first = NULL;
    push(&first, 3);
    printf("First Number:\n");
    printLinkedList(first);
    DLink second = NULL;
    push(&second, 0);
    push(&second, 1);
    printf("Second Number:\n");
    printLinkedList(second);
    DLink sumOfLists = sum(first, second);
    printf("Sum of lists:\n");
    printLinkedList(sumOfLists);
    printf("\n");
}
void testCaseFirstNumberLongerThanSecondNumber() {
    printf("**Test case where first list is longer than second list**\n");
    DLink first = NULL;
    push(&first, 1);
    push(&first, 2);
    push(&first, 3);
    printf("First Number:\n");
    printLinkedList(first);
    DLink second = NULL;
    push(&second, 5);
    push(&second, 1);
    printf("Second Number:\n");
    printLinkedList(second);
    DLink sumOfLists = sum(first, second);
    printf("Sum of lists:\n");
    printLinkedList(sumOfLists);
    printf("\n");
}
void testCaseExpectedUseCase() {
    printf("**Test case where both numbers are expected use case size**\n");
    DLink first = NULL;
    push(&first, 1);
    push(&first, 2);
    push(&first, 3);
    push(&first, 4);
    printf("First Number:\n");
    printLinkedList(first);
    DLink second = NULL;
    push(&second, 4);
    push(&second, 3);
    push(&second, 2);
    push(&second, 1);
    printf("Second Number:\n");
    printLinkedList(second);
    DLink sumOfLists = sum(first, second);
    printf("Sum of lists:\n");
    printLinkedList(sumOfLists);
    printf("\n");
}
void testCaseLargeAddLarge() {
    printf("**Test case where a large number to added with a large number**\n");
    DLink first = NULL;
    push(&first, 1);
    push(&first, 2);
    push(&first, 3);
    push(&first, 4);
    push(&first, 5);
    push(&first, 6);
    push(&first, 7);
    push(&first, 8);
    push(&first, 9);
    push(&first, 1);
    push(&first, 2);
    push(&first, 3);
    push(&first, 4);
    push(&first, 5);
    push(&first, 6);
    push(&first, 7);
    push(&first, 8);
    push(&first, 9);
    printf("First Number:\n");
    printLinkedList(first);
    DLink second = NULL;
    push(&second, 9);
    push(&second, 8);
    push(&second, 7);
    push(&second, 6);
    push(&second, 5);
    push(&second, 4);
    push(&second, 3);
    push(&second, 2);
    push(&second, 1);
    push(&second, 9);
    push(&second, 8);
    push(&second, 7);
    push(&second, 6);
    push(&second, 5);
    push(&second, 4);
    push(&second, 3);
    push(&second, 2);
    push(&second, 1);
    printf("Second Number:\n");
    printLinkedList(second);
    DLink sumOfLists = sum(first, second);
    printf("Sum of lists:\n");
    printLinkedList(sumOfLists);
    printf("\n");
}

int main() {
    testCaseBothZero();
    testCaseRemainderCarryDigit();
    testCaseSecondNumberLongerThanFirstNumber();
    testCaseFirstNumberLongerThanSecondNumber();
    testCaseExpectedUseCase();
    testCaseLargeAddLarge();
    return 0;
}
