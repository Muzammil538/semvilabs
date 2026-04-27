
// Program to perform following binary tree operations.
// i) creation ii) insertion of a node iii) traversal using recursion.

#include <stdio.h>
#include <stdlib.h>

// Structure for a tree node
struct Node {
    int data;
    struct Node* left;
    struct Node* right;
};

// Function to create a new node
struct Node* createNode(int value) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = value;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

// Function to insert a node in BST
struct Node* insert(struct Node* root, int value) {
    // If tree is empty, create root node
    if (root == NULL) {
        return createNode(value);
    }
    
    // Otherwise, go to left or right subtree
    if (value < root->data) {
        root->left = insert(root->left, value);
    }
    else if (value > root->data) {
        root->right = insert(root->right, value);
    }
    
    return root;
}

// Inorder traversal (Left - Root - Right)
void inorder(struct Node* root) {
    if (root != NULL) {
        inorder(root->left);
        printf("%d ", root->data);
        inorder(root->right);
    }
}

// Preorder traversal (Root - Left - Right)
void preorder(struct Node* root) {
    if (root != NULL) {
        printf("%d ", root->data);
        preorder(root->left);
        preorder(root->right);
    }
}

// Postorder traversal (Left - Right - Root)
void postorder(struct Node* root) {
    if (root != NULL) {
        postorder(root->left);
        postorder(root->right);
        printf("%d ", root->data);
    }
}

int main() {
    struct Node* root = NULL;
    int choice, value;
    
    printf("Binary Tree Operations\n");
    printf("1. Insert node\n");
    printf("2. Inorder traversal\n");
    printf("3. Preorder traversal\n");
    printf("4. Postorder traversal\n");
    printf("5. Exit\n");
    
    do {
        printf("\nEnter your choice: ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1:
                printf("Enter value to insert: ");
                scanf("%d", &value);
                root = insert(root, value);
                printf("Node inserted successfully!\n");
                break;
                
            case 2:
                if(root == NULL) {
                    printf("Tree is empty!\n");
                } else {
                    printf("Inorder traversal: ");
                    inorder(root);
                    printf("\n");
                }
                break;
                
            case 3:
                if(root == NULL) {
                    printf("Tree is empty!\n");
                } else {
                    printf("Preorder traversal: ");
                    preorder(root);
                    printf("\n");
                }
                break;
                
            case 4:
                if(root == NULL) {
                    printf("Tree is empty!\n");
                } else {
                    printf("Postorder traversal: ");
                    postorder(root);
                    printf("\n");
                }
                break;
                
            case 5:
                printf("Exiting...\n");
                break;
                
            default:
                printf("Invalid choice! Try again.\n");
        }
    } while(choice != 5);
    
    return 0;
}

