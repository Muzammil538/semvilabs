#include <stdio.h>
#include <stdlib.h>

struct Node {
    int data;
    struct Node* left;
    struct Node* right;
    int height;
};

// Function to get height of a node
int getHeight(struct Node* node) {
    if (node == NULL)
        return 0;
    return node->height;
}

// Function to get maximum of two numbers
int max(int a, int b) {
    return (a > b) ? a : b;
}

// Create a new node
struct Node* createNode(int value) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = value;
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->height = 1;  // New node is added at leaf
    return newNode;
}

// Right rotate subtree rooted with y
struct Node* rightRotate(struct Node* y) {
    struct Node* x = y->left;
    struct Node* T2 = x->right;
    
    // Perform rotation
    x->right = y;
    y->left = T2;
    
    // Update heights
    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
    
    return x;
}

// Left rotate subtree rooted with x
struct Node* leftRotate(struct Node* x) {
    struct Node* y = x->right;
    struct Node* T2 = y->left;
    
    // Perform rotation
    y->left = x;
    x->right = T2;
    
    // Update heights
    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
    
    return y;
}

// Get balance factor of a node
int getBalance(struct Node* node) {
    if (node == NULL)
        return 0;
    return getHeight(node->left) - getHeight(node->right);
}

// Insert a node into AVL tree
struct Node* insert(struct Node* node, int value) {
    // Step 1: Normal BST insertion
    if (node == NULL)
        return createNode(value);
    
    if (value < node->data)
        node->left = insert(node->left, value);
    else if (value > node->data)
        node->right = insert(node->right, value);
    else
        return node;  // Duplicate values not allowed
    
    // Step 2: Update height of current node
    node->height = 1 + max(getHeight(node->left), getHeight(node->right));
    
    // Step 3: Get balance factor to check imbalance
    int balance = getBalance(node);
    
    // Step 4: If unbalanced, there are 4 cases
    
    // Left Left Case
    if (balance > 1 && value < node->left->data)
        return rightRotate(node);
    
    // Right Right Case
    if (balance < -1 && value > node->right->data)
        return leftRotate(node);
    
    // Left Right Case
    if (balance > 1 && value > node->left->data) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }
    
    // Right Left Case
    if (balance < -1 && value < node->right->data) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }
    
    return node;
}

// Function to find node with minimum value
struct Node* minValueNode(struct Node* node) {
    struct Node* current = node;
    while (current->left != NULL)
        current = current->left;
    return current;
}

// Delete a node from AVL tree
struct Node* deleteNode(struct Node* root, int value) {
    if (root == NULL)
        return root;
    
    // Step 1: Normal BST deletion
    if (value < root->data)
        root->left = deleteNode(root->left, value);
    else if (value > root->data)
        root->right = deleteNode(root->right, value);
    else {
        // Node to be deleted found
        if (root->left == NULL || root->right == NULL) {
            struct Node* temp = root->left ? root->left : root->right;
            
            if (temp == NULL) {
                temp = root;
                root = NULL;
            } else {
                *root = *temp;
            }
            free(temp);
        } else {
            struct Node* temp = minValueNode(root->right);
            root->data = temp->data;
            root->right = deleteNode(root->right, temp->data);
        }
    }
    
    if (root == NULL)
        return root;
    
    // Step 2: Update height
    root->height = 1 + max(getHeight(root->left), getHeight(root->right));
    
    // Step 3: Get balance factor
    int balance = getBalance(root);
    
    // Step 4: Balance the tree
    
    // Left Left Case
    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);
    
    // Left Right Case
    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }
    
    // Right Right Case
    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);
    
    // Right Left Case
    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }
    
    return root;
}

// Inorder traversal
void inorder(struct Node* root) {
    if (root != NULL) {
        inorder(root->left);
        printf("%d ", root->data);
        inorder(root->right);
    }
}

// Preorder traversal (shows heights too)
void preorder(struct Node* root) {
    if (root != NULL) {
        printf("%d(h=%d) ", root->data, root->height);
        preorder(root->left);
        preorder(root->right);
    }
}

int main() {
    struct Node* root = NULL;
    int n, value, i, choice;
    
    printf("======= AVL TREE CONSTRUCTION =======\n");
    printf("1. Insert nodes\n");
    printf("2. Delete a node\n");
    printf("3. Display traversals\n");
    printf("4. Exit\n");
    
    do {
        printf("\nEnter choice: ");
        scanf("%d", &choice);
        
        if(choice == 1) {
            printf("How many nodes to insert? ");
            scanf("%d", &n);
            printf("Enter %d values: ", n);
            for(i = 0; i < n; i++) {
                scanf("%d", &value);
                root = insert(root, value);
            }
            printf("\n--- AVL Tree Created ---\n");
            printf("Inorder: ");
            inorder(root);
            printf("\nPreorder (with heights): ");
            preorder(root);
            printf("\n");
        }
        else if(choice == 2) {
            if(root == NULL) {
                printf("Tree is empty!\n");
            } else {
                printf("Enter value to delete: ");
                scanf("%d", &value);
                root = deleteNode(root, value);
                printf("After deletion:\n");
                printf("Inorder: ");
                inorder(root);
                printf("\nPreorder (with heights): ");
                preorder(root);
                printf("\n");
            }
        }
        else if(choice == 3) {
            if(root == NULL) {
                printf("Tree is empty!\n");
            } else {
                printf("Inorder: ");
                inorder(root);
                printf("\nPreorder (with heights): ");
                preorder(root);
                printf("\n");
            }
        }
        else if(choice == 4) {
            printf("Exiting...\n");
        }
        else {
            printf("Invalid choice!\n");
        }
    } while(choice != 4);
    
    return 0;
}