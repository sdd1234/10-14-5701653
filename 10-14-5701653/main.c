#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define _CRT_SECURE_NO_WARNINGS

typedef struct TreeNode {
    int data;
    struct TreeNode* left;
    struct TreeNode* right;
    struct TreeNode* parent;
} TreeNode;

TreeNode* createNode(int data) {
    TreeNode* newNode = (TreeNode*)malloc(sizeof(TreeNode));
    if (newNode == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    newNode->data = data;
    newNode->left = newNode->right = newNode->parent = NULL;
    return newNode;
}

void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void printTree(TreeNode* root) {
    if (!root) return;

    TreeNode* queue[1000];
    int front = 0, rear = 0;

    queue[rear++] = root;

    while (front < rear) {
        TreeNode* current = queue[front++];
        printf("%d ", current->data);

        if (current->left) queue[rear++] = current->left;
        if (current->right) queue[rear++] = current->right;
    }
    printf("\n");
}

void heapifyUp(TreeNode* node, int* movements, TreeNode* root) {
    while (node->parent && node->data > node->parent->data) {
        swap(&node->data, &node->parent->data);
        (*movements)++;
        printTree(root);
        node = node->parent;
    }
}

void heapifyDown(TreeNode* node, int* movements, TreeNode* root) {
    while (true) {
        TreeNode* largest = node;
        TreeNode* left = node->left;
        TreeNode* right = node->right;

        if (left && left->data > largest->data)
            largest = left;

        if (right && right->data > largest->data)
            largest = right;

        if (largest != node) {
            swap(&node->data, &largest->data);
            (*movements)++;
            printTree(root);
            node = largest;
        }
        else {
            break;
        }
    }
}

void InsertMaxHeapTree(TreeNode** root, int data, int* movements, bool showOutput) {
    if (*root == NULL) {
        *root = createNode(data);
        if (showOutput) printTree(*root);
        return;
    }

    TreeNode* queue[1000];
    int front = 0, rear = 0;

    queue[rear++] = *root;

    while (front < rear) {
        TreeNode* current = queue[front++];

        if (current->left == NULL) {
            current->left = createNode(data);
            current->left->parent = current;
            if (showOutput) printTree(*root);
            heapifyUp(current->left, movements, *root);
            break;
        }
        else {
            queue[rear++] = current->left;
        }

        if (current->right == NULL) {
            current->right = createNode(data);
            current->right->parent = current;
            if (showOutput) printTree(*root);
            heapifyUp(current->right, movements, *root);
            break;
        }
        else {
            queue[rear++] = current->right;
        }
    }
}

TreeNode* findLastNode(TreeNode* root) {
    if (root == NULL) return NULL;

    TreeNode* queue[1000];
    int front = 0, rear = 0;

    queue[rear++] = root;
    TreeNode* last = NULL;

    while (front < rear) {
        last = queue[front++];

        if (last->left) queue[rear++] = last->left;
        if (last->right) queue[rear++] = last->right;
    }

    return last;
}

void deleteRoot(TreeNode** root, int* movements) {
    if (*root == NULL) {
        printf("Ʈ���� ��� �ֽ��ϴ�.\n");
        return;
    }

    TreeNode* lastNode = findLastNode(*root);
    if (lastNode == *root) {
        free(*root);
        *root = NULL;
        return;
    }

    swap(&(*root)->data, &lastNode->data);
    (*movements)++;

    if (lastNode->parent->left == lastNode)
        lastNode->parent->left = NULL;
    else
        lastNode->parent->right = NULL;

    free(lastNode);

    printTree(*root);
    heapifyDown(*root, movements, *root);
}

void printLevelOrder(TreeNode* root) {
    if (root == NULL) {
        printf("Ʈ���� ��� �ֽ��ϴ�.\n");
        return;
    }

    TreeNode* queue[1000];
    int level[1000];
    int front = 0, rear = 0;

    queue[rear] = root;
    level[rear++] = 1;

    int currentLevel = 1;

    printf("[%d] ", currentLevel);

    while (front < rear) {
        TreeNode* current = queue[front];
        int nodeLevel = level[front++];

        if (nodeLevel > currentLevel) {
            currentLevel = nodeLevel;
            printf("\n[%d] ", currentLevel);
        }

        printf("%d ", current->data);

        if (current->left != NULL) {
            queue[rear] = current->left;
            level[rear++] = nodeLevel + 1;
        }
        if (current->right != NULL) {
            queue[rear] = current->right;
            level[rear++] = nodeLevel + 1;
        }
    }
    printf("\n");
}

void runUserInterface(TreeNode** root) {
    while (true) {
        char choice;
        int data, movements;
        printf("---------------------\n");
        printf("| i : ��� �߰�     |\n");
        printf("| d : ��Ʈ ����     |\n");
        printf("| p : ������ ���   |\n");
        printf("| c : ����          |\n");
        printf("---------------------\n\n");
        printf("�޴� �Է�: ");
        scanf_s(" %c", &choice, 1);

        switch (choice) {
        case 'i':
            printf("�߰��� �� �Է�: ");
            scanf_s("%d", &data);
            movements = 0;
            InsertMaxHeapTree(root, data, &movements, true);
            printf("��尡 �̵��� Ƚ��: %d\n", movements);
            break;
        case 'd':
            movements = 0;
            deleteRoot(root, &movements);
            printf("��尡 �̵��� Ƚ��: %d\n", movements);
            break;
        case 'p':
            printf("Ʈ�� ������ ���\n");
            printLevelOrder(*root);
            break;
        case 'c':
            return;
        default:
            printf("�߸��� �����Դϴ�.\n");
        }
    }
}

TreeNode* GenerateMaxHeapTree(int inputData[], int size, int* movements) {
    TreeNode* root = NULL;

    for (int i = 0; i < size; i++) {
        InsertMaxHeapTree(&root, inputData[i], movements, false);
    }

    return root;
}

void freeTree(TreeNode* root) {
    if (root == NULL) return;

    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

int main() {
    int inputData[] = { 90, 89, 70, 36, 75, 63, 65, 21, 18, 15 };
    int size = sizeof(inputData) / sizeof(inputData[0]);
    int movements = 0;

    TreeNode* root = GenerateMaxHeapTree(inputData, size, &movements);

    runUserInterface(&root);

    freeTree(root);

    return 0;
}
