#include <stdio.h>
#include <stdlib.h>

struct Node {
    int data;
    struct Node* left;
    struct Node* right;
};

struct Stack {
    struct Node* node;
    struct Stack* next;
};

struct Queue {
    struct Node* node;
    struct Queue* next;
};

struct Node* createNode(int value) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = value;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

struct Node* insert(struct Node* root, int value) {
    if (root == NULL) {
        return createNode(value);
    }
    
    if (value < root->data) {
        root->left = insert(root->left, value);
    }
    else if (value > root->data) {
        root->right = insert(root->right, value);
    }
    
    return root;
}

// Stack functions for iterative traversals
void push(struct Stack** top, struct Node* node) {
    struct Stack* newStack = (struct Stack*)malloc(sizeof(struct Stack));
    newStack->node = node;
    newStack->next = *top;
    *top = newStack;
}

struct Node* pop(struct Stack** top) {
    if (*top == NULL) {
        return NULL;
    }
    struct Stack* temp = *top;
    struct Node* node = temp->node;
    *top = (*top)->next;
    free(temp);
    return node;
}

int isEmpty(struct Stack* top) {
    return top == NULL;
}

// Queue functions for spiral order
void enqueue(struct Queue** front, struct Queue** rear, struct Node* node) {
    struct Queue* newQueue = (struct Queue*)malloc(sizeof(struct Queue));
    newQueue->node = node;
    newQueue->next = NULL;
    
    if (*rear == NULL) {
        *front = *rear = newQueue;
    } else {
        (*rear)->next = newQueue;
        *rear = newQueue;
    }
}

struct Node* dequeue(struct Queue** front, struct Queue** rear) {
    if (*front == NULL) {
        return NULL;
    }
    struct Queue* temp = *front;
    struct Node* node = temp->node;
    *front = (*front)->next;
    
    if (*front == NULL) {
        *rear = NULL;
    }
    free(temp);
    return node;
}

int isQueueEmpty(struct Queue* front) {
    return front == NULL;
}

// Iterative inorder traversal
void inorderIterative(struct Node* root) {
    struct Stack* stack = NULL;
    struct Node* current = root;
    
    printf("Inorder: ");
    while (current != NULL || !isEmpty(stack)) {
        // Go to leftmost node
        while (current != NULL) {
            push(&stack, current);
            current = current->left;
        }
        
        // Pop and print
        current = pop(&stack);
        printf("%d ", current->data);
        
        // Go to right subtree
        current = current->right;
    }
    printf("\n");
}

// Iterative preorder traversal
void preorderIterative(struct Node* root) {
    if (root == NULL) return;
    
    struct Stack* stack = NULL;
    push(&stack, root);
    
    printf("Preorder: ");
    while (!isEmpty(stack)) {
        struct Node* current = pop(&stack);
        printf("%d ", current->data);
        
        // Push right first so that left is processed first
        if (current->right != NULL) {
            push(&stack, current->right);
        }
        if (current->left != NULL) {
            push(&stack, current->left);
        }
    }
    printf("\n");
}

// Iterative postorder using two stacks
void postorderIterative(struct Node* root) {
    if (root == NULL) return;
    
    struct Stack* stack1 = NULL;
    struct Stack* stack2 = NULL;
    
    push(&stack1, root);
    
    while (!isEmpty(stack1)) {
        struct Node* current = pop(&stack1);
        push(&stack2, current);
        
        if (current->left != NULL) {
            push(&stack1, current->left);
        }
        if (current->right != NULL) {
            push(&stack1, current->right);
        }
    }
    
    printf("Postorder: ");
    while (!isEmpty(stack2)) {
        struct Node* current = pop(&stack2);
        printf("%d ", current->data);
    }
    printf("\n");
}

// Spiral order traversal (zigzag)
void spiralOrder(struct Node* root) {
    if (root == NULL) return;
    
    struct Stack* stack1 = NULL;  // For left to right
    struct Stack* stack2 = NULL;  // For right to left
    
    push(&stack1, root);
    
    printf("Spiral order: ");
    
    while (!isEmpty(stack1) || !isEmpty(stack2)) {
        // Print nodes from stack1 (left to right)
        while (!isEmpty(stack1)) {
            struct Node* current = pop(&stack1);
            printf("%d ", current->data);
            
            // Push children to stack2 in left-right order
            if (current->left != NULL) {
                push(&stack2, current->left);
            }
            if (current->right != NULL) {
                push(&stack2, current->right);
            }
        }
        
        // Print nodes from stack2 (right to left)
        while (!isEmpty(stack2)) {
            struct Node* current = pop(&stack2);
            printf("%d ", current->data);
            
            // Push children to stack1 in right-left order
            if (current->right != NULL) {
                push(&stack1, current->right);
            }
            if (current->left != NULL) {
                push(&stack1, current->left);
            }
        }
    }
    printf("\n");
}

int main() {
    struct Node* root = NULL;
    int n, value, i;
    
    printf("Enter number of nodes to insert: ");
    scanf("%d", &n);
    
    printf("Enter %d elements: ", n);
    for(i = 0; i < n; i++) {
        scanf("%d", &value);
        root = insert(root, value);
    }
    
    printf("\n--- BST Traversals Without Recursion ---\n\n");
    
    inorderIterative(root);
    preorderIterative(root);
    postorderIterative(root);
    spiralOrder(root);
    
    return 0;
}