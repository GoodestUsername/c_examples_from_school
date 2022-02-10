#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct Node {
    int data;
    struct Node *parent;
    struct Node *left;
    struct Node *right;
};

typedef struct Node* TreeNode;

TreeNode createTreeNode(int data, TreeNode parent, TreeNode left, TreeNode right) {
    TreeNode node = (TreeNode) malloc(sizeof(struct Node));
    if (node == NULL) {
        perror("Memory issues");
        exit(1);
    }

    node->data = data;
    node->parent = parent;
    node->left = left;
    node->right = right;
    return node;
}

void freeBinaryTree(TreeNode root) {
    if (!root) {
        return;
    }

    freeBinaryTree(root->left);
    freeBinaryTree(root->right);
    free(root);
}

void printTreeInOrder(TreeNode root) {
    if (!root) {
        return;
    }

    printTreeInOrder(root->left);
    printf("%d ", root->data);
    printTreeInOrder(root->right);
}

TreeNode insert(TreeNode root, int data) {
    if (!root) {
        return createTreeNode(data, NULL, NULL, NULL);
    }

    if (data <= root->data) {
        root->left = insert(root->left, data);
        root->left->parent = root;
    } else {
        root->right = insert(root->right, data);
        root->right->parent = root;
    }

    return root;
}

bool isLeaf(TreeNode root) {
    if (!root) {
        return false;
    }

    return (!root->left && !root->right);
}

bool equals(TreeNode firstRoot, TreeNode secondRoot) {
    if (!firstRoot && !secondRoot) {
        return true;
    }

    if (firstRoot && secondRoot) {
        return firstRoot->data == secondRoot->data &&
        equals(firstRoot->right, secondRoot->right) && equals(firstRoot->left, secondRoot->left);
    }

    return false;
}

bool containsPathWithSum(TreeNode root, int sum) {
    if (!root) {
        return false;
    }

    sum -= root->data;

    if (!sum && isLeaf(root)) {
        return true;
    }

    return containsPathWithSum(root->left, sum) + containsPathWithSum(root->right, sum);
}

void mirror(TreeNode root) {
    if (!root) {
        return;
    }

    mirror(root->left);
    mirror(root->right);
    TreeNode temp = root->right;
    root->right = root->left;
    root->left = temp;
}

void testEqualsForBothNull() {
    printf("TESTING EQUALS FOR BOTH NULL: \n");
    TreeNode root0 = NULL;
    TreeNode root1 = NULL;
    printf("%s\n\n", equals(root0, root1) ? "true" : "false");

    freeBinaryTree(root0);
    freeBinaryTree(root1);
}

void testEqualsForOnlyOneNull() {
    TreeNode root0 = NULL;
    TreeNode root1 = NULL;

    root1 = insert(root1,2);
    root1 = insert(root1,4);

    printf("TESTING EQUALS FOR ONE NULL, ONE NOT NULL: \n");
    printf("%s\n\n", equals(root0, root1) ? "true" : "false");

    freeBinaryTree(root0);
    freeBinaryTree(root1);
}

void testEqualsForTreesAreEqual() {
    TreeNode root0 = NULL;
    TreeNode root1 = NULL;

    root1 = insert(root1,2);
    root1 = insert(root1,4);

    root0 = insert(root0, 2);
    root0 = insert(root0, 4);

    printf("TESTING EQUALS FOR TREES ARE EQUAL: \n");
    printf("%s\n\n", equals(root0, root1) ? "true" : "false");

    freeBinaryTree(root0);
    freeBinaryTree(root1);
}

void testEqualsForDifferentLengthsNotEquals() {
    TreeNode root0 = NULL;
    TreeNode root1 = NULL;

    root1 = insert(root1,2);
    root1 = insert(root1,4);

    root0 = insert(root0, 2);
    root0 = insert(root0, 4);
    root0 = insert(root0, 5);

    printf("TESTING EQUALS FOR DIFFERENT LENGTH NOT EQUAL: \n");
    printf("%s\n\n", equals(root0, root1) ? "true" : "false");

    freeBinaryTree(root0);
    freeBinaryTree(root1);
}

void testContainsPathWithSumForNull() {
    TreeNode root0 = NULL;
    printf("TESTING containsPathWithSum FOR NULL: \n");
    printf("%s\n\n", containsPathWithSum(root0, 5) ? "true" : "false");
    freeBinaryTree(root0);
}
void testContainsPathWithSumForSumInRightBranch() {
    TreeNode root0 = NULL;

    root0 = insert(root0, 5);
    root0 = insert(root0, 10);
    root0 = insert(root0, 4);
    root0 = insert(root0, -8);

    printf("TESTING containsPathWithSum FOR CONTAINS SUM IN RIGHT BRANCH: \n");
    printf("%s\n\n", containsPathWithSum(root0, 15) ? "true" : "false");

    freeBinaryTree(root0);
}

void testContainsPathWithSumForSumInLeftBranch() {
    TreeNode root0 = NULL;

    root0 = insert(root0, 5);
    root0 = insert(root0, 10);
    root0 = insert(root0, 4);
    root0 = insert(root0, -8);

    printf("TESTING containsPathWithSum FOR CONTAINS SUM IN LEFT BRANCH: \n");
    printf("%s\n\n", containsPathWithSum(root0, 1) ? "true" : "false");

    freeBinaryTree(root0);
}


void testContainsPathWithSumForDoesNotContainSum() {
    TreeNode root0 = NULL;

    root0 = insert(root0, 5);
    root0 = insert(root0, 10);
    root0 = insert(root0, 4);
    root0 = insert(root0, -8);

    printf("TESTING containsPathWithSum FOR DOES NOT CONTAINS SUM: \n");
    printf("%s\n\n", containsPathWithSum(root0, 9) ? "true" : "false");

    freeBinaryTree(root0);
}

void testMirrorForNull() {
    TreeNode root = NULL;

    printf("TESTING mirror FOR NULL\n\n");

    mirror(root);

    printf("Does root contain NULL: %s\n\n", !root ? "true" : "false");

    freeBinaryTree(root);
}

void testMirrorForOneBranch() {
    TreeNode root0 = NULL;

    root0 = insert(root0, 5);
    root0 = insert(root0, 4);
    root0 = insert(root0, -8);

    printf("TESTING mirror FOR ONE BRANCH\n\n");

    printf("Original tree\n");
    printTreeInOrder(root0);
    printf("\n\n");

    mirror(root0);

    printf("After Mirror\n");
    printTreeInOrder(root0);
    printf("\n\n");
    freeBinaryTree(root0);
}

void testMirrorForTwoBranches() {
    TreeNode root = NULL;

    root = insert(root, 5);
    root = insert(root, 4);
    root = insert(root, -8);
    root = insert(root, 10);

    printf("TESTING mirror FOR TWO BRANCHES\n\n");

    printf("Original tree\n");
    printTreeInOrder(root);
    printf("\n\n");

    mirror(root);

    printf("After Mirror\n");
    printTreeInOrder(root);
    printf("\n\n");
}

void testMirrorForTreeBranchHasTwoChildBranches() {
    TreeNode root = NULL;

    root = insert(root, 5);
    root = insert(root, 4);
    root = insert(root, -8);
    root = insert(root, 10);
    root = insert(root, 11);
    root = insert(root, 9);

    printf("TESTING mirror FOR TREE HAS BRANCH WITH TWO CHILD BRANCHES\n\n");

    printf("Original tree\n");
    printTreeInOrder(root);
    printf("\n\n");

    mirror(root);

    printf("After Mirror\n");
    printTreeInOrder(root);
    printf("\n");

    freeBinaryTree(root);
}

int main() {
    
    testEqualsForBothNull();
    testEqualsForOnlyOneNull();
    testEqualsForTreesAreEqual();
    testEqualsForDifferentLengthsNotEquals();

    testContainsPathWithSumForNull();
    testContainsPathWithSumForSumInRightBranch();
    testContainsPathWithSumForSumInLeftBranch();
    testContainsPathWithSumForDoesNotContainSum();

    testMirrorForNull();
    testMirrorForOneBranch();
    testMirrorForTwoBranches();
    testMirrorForTreeBranchHasTwoChildBranches();
    
    return 0;
}
