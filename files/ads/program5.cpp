#include <stdio.h>
#include <stdlib.h>

// Minimum degree (t=3 means order 5)
// Each node can have max 5 children and 4 keys
#define T 3
#define MAX_KEYS (2*T - 1)
#define MIN_KEYS (T - 1)

struct BTreeNode {
    int keys[MAX_KEYS];
    struct BTreeNode* children[MAX_KEYS + 1];
    int numKeys;
    int isLeaf;
};

// Create a new B-tree node
struct BTreeNode* createNode(int isLeaf) {
    struct BTreeNode* newNode = (struct BTreeNode*)malloc(sizeof(struct BTreeNode));
    newNode->numKeys = 0;
    newNode->isLeaf = isLeaf;
    
    for(int i = 0; i <= MAX_KEYS; i++) {
        newNode->children[i] = NULL;
    }
    
    return newNode;
}

// Traverse the B-tree
void traverse(struct BTreeNode* root) {
    if(root == NULL) return;
    
    int i;
    for(i = 0; i < root->numKeys; i++) {
        if(!root->isLeaf) {
            traverse(root->children[i]);
        }
        printf("%d ", root->keys[i]);
    }
    
    if(!root->isLeaf) {
        traverse(root->children[i]);
    }
}

// Search for a key in B-tree
struct BTreeNode* search(struct BTreeNode* root, int key, int* pos) {
    if(root == NULL) return NULL;
    
    int i = 0;
    while(i < root->numKeys && key > root->keys[i]) {
        i++;
    }
    
    if(i < root->numKeys && key == root->keys[i]) {
        *pos = i;
        return root;
    }
    
    if(root->isLeaf) {
        return NULL;
    }
    
    return search(root->children[i], key, pos);
}

// Split a child node
void splitChild(struct BTreeNode* parent, int childIndex) {
    struct BTreeNode* child = parent->children[childIndex];
    struct BTreeNode* newChild = createNode(child->isLeaf);
    newChild->numKeys = MIN_KEYS;
    
    // Copy last (T-1) keys to new node
    for(int i = 0; i < MIN_KEYS; i++) {
        newChild->keys[i] = child->keys[i + T];
    }
    
    // Copy children if not leaf
    if(!child->isLeaf) {
        for(int i = 0; i <= MIN_KEYS; i++) {
            newChild->children[i] = child->children[i + T];
        }
    }
    
    child->numKeys = MIN_KEYS;
    
    // Shift parent's children to make space
    for(int i = parent->numKeys; i >= childIndex + 1; i--) {
        parent->children[i + 1] = parent->children[i];
    }
    
    parent->children[childIndex + 1] = newChild;
    
    // Shift parent's keys to make space
    for(int i = parent->numKeys - 1; i >= childIndex; i--) {
        parent->keys[i + 1] = parent->keys[i];
    }
    
    parent->keys[childIndex] = child->keys[T - 1];
    parent->numKeys++;
}

// Insert non-full node
void insertNonFull(struct BTreeNode* node, int key) {
    int i = node->numKeys - 1;
    
    if(node->isLeaf) {
        // Find position and shift keys
        while(i >= 0 && key < node->keys[i]) {
            node->keys[i + 1] = node->keys[i];
            i--;
        }
        
        node->keys[i + 1] = key;
        node->numKeys++;
    }
    else {
        // Find child to insert into
        while(i >= 0 && key < node->keys[i]) {
            i--;
        }
        i++;
        
        // Check if child is full
        if(node->children[i]->numKeys == MAX_KEYS) {
            splitChild(node, i);
            
            // Decide which child to go to
            if(key > node->keys[i]) {
                i++;
            }
        }
        insertNonFull(node->children[i], key);
    }
}

// Insert a key into B-tree
struct BTreeNode* insert(struct BTreeNode* root, int key) {
    if(root == NULL) {
        root = createNode(1);
        root->keys[0] = key;
        root->numKeys = 1;
        return root;
    }
    
    if(root->numKeys == MAX_KEYS) {
        struct BTreeNode* newRoot = createNode(0);
        newRoot->children[0] = root;
        splitChild(newRoot, 0);
        
        int i = 0;
        if(newRoot->keys[0] < key) {
            i++;
        }
        insertNonFull(newRoot->children[i], key);
        return newRoot;
    }
    
    insertNonFull(root, key);
    return root;
}

// Get predecessor (largest key in left subtree)
int getPredecessor(struct BTreeNode* node) {
    while(!node->isLeaf) {
        node = node->children[node->numKeys];
    }
    return node->keys[node->numKeys - 1];
}

// Get successor (smallest key in right subtree)
int getSuccessor(struct BTreeNode* node) {
    while(!node->isLeaf) {
        node = node->children[0];
    }
    return node->keys[0];
}

// Borrow from left sibling
void borrowFromLeft(struct BTreeNode* node, int index) {
    struct BTreeNode* child = node->children[index];
    struct BTreeNode* leftSibling = node->children[index - 1];
    
    // Shift child's keys right
    for(int i = child->numKeys - 1; i >= 0; i--) {
        child->keys[i + 1] = child->keys[i];
    }
    
    // Shift child's children right if not leaf
    if(!child->isLeaf) {
        for(int i = child->numKeys; i >= 0; i--) {
            child->children[i + 1] = child->children[i];
        }
    }
    
    child->keys[0] = node->keys[index - 1];
    child->numKeys++;
    
    if(!child->isLeaf) {
        child->children[0] = leftSibling->children[leftSibling->numKeys];
    }
    
    node->keys[index - 1] = leftSibling->keys[leftSibling->numKeys - 1];
    leftSibling->numKeys--;
}

// Borrow from right sibling
void borrowFromRight(struct BTreeNode* node, int index) {
    struct BTreeNode* child = node->children[index];
    struct BTreeNode* rightSibling = node->children[index + 1];
    
    child->keys[child->numKeys] = node->keys[index];
    child->numKeys++;
    
    if(!child->isLeaf) {
        child->children[child->numKeys] = rightSibling->children[0];
    }
    
    node->keys[index] = rightSibling->keys[0];
    
    // Shift right sibling's keys left
    for(int i = 1; i < rightSibling->numKeys; i++) {
        rightSibling->keys[i - 1] = rightSibling->keys[i];
    }
    
    // Shift right sibling's children left if not leaf
    if(!rightSibling->isLeaf) {
        for(int i = 1; i <= rightSibling->numKeys; i++) {
            rightSibling->children[i - 1] = rightSibling->children[i];
        }
    }
    
    rightSibling->numKeys--;
}

// Merge child nodes
void mergeChildren(struct BTreeNode* node, int index) {
    struct BTreeNode* leftChild = node->children[index];
    struct BTreeNode* rightChild = node->children[index + 1];
    
    // Pull down key from parent
    leftChild->keys[MIN_KEYS] = node->keys[index];
    
    // Copy keys from right child
    for(int i = 0; i < rightChild->numKeys; i++) {
        leftChild->keys[i + T] = rightChild->keys[i];
    }
    
    // Copy children from right child if not leaf
    if(!leftChild->isLeaf) {
        for(int i = 0; i <= rightChild->numKeys; i++) {
            leftChild->children[i + T] = rightChild->children[i];
        }
    }
    
    leftChild->numKeys += rightChild->numKeys + 1;
    
    // Shift parent's keys left
    for(int i = index; i < node->numKeys - 1; i++) {
        node->keys[i] = node->keys[i + 1];
    }
    
    // Shift parent's children left
    for(int i = index + 1; i < node->numKeys; i++) {
        node->children[i] = node->children[i + 1];
    }
    
    node->numKeys--;
    free(rightChild);
}

// Delete a key from B-tree
struct BTreeNode* deleteKey(struct BTreeNode* root, int key) {
    if(root == NULL) return NULL;
    
    int i = 0;
    while(i < root->numKeys && key > root->keys[i]) {
        i++;
    }
    
    if(i < root->numKeys && key == root->keys[i]) {
        // Key found in this node
        if(root->isLeaf) {
            // Case 1: Key in leaf node
            for(int j = i; j < root->numKeys - 1; j++) {
                root->keys[j] = root->keys[j + 1];
            }
            root->numKeys--;
        }
        else {
            // Case 2: Key in internal node
            if(root->children[i]->numKeys >= T) {
                int pred = getPredecessor(root->children[i]);
                root->keys[i] = pred;
                root->children[i] = deleteKey(root->children[i], pred);
            }
            else if(root->children[i + 1]->numKeys >= T) {
                int succ = getSuccessor(root->children[i + 1]);
                root->keys[i] = succ;
                root->children[i + 1] = deleteKey(root->children[i + 1], succ);
            }
            else {
                mergeChildren(root, i);
                root->children[i] = deleteKey(root->children[i], key);
            }
        }
    }
    else {
        // Key not found in this node
        if(root->isLeaf) {
            printf("Key %d not found!\n", key);
            return root;
        }
        
        int flag = (i == root->numKeys);
        
        if(root->children[i]->numKeys < T) {
            // Borrow or merge
            if(i > 0 && root->children[i - 1]->numKeys >= T) {
                borrowFromLeft(root, i);
            }
            else if(i < root->numKeys && root->children[i + 1]->numKeys >= T) {
                borrowFromRight(root, i);
            }
            else {
                if(i < root->numKeys) {
                    mergeChildren(root, i);
                }
                else {
                    mergeChildren(root, i - 1);
                    i--;
                }
            }
        }
        
        if(flag && i > root->numKeys) {
            root->children[i] = deleteKey(root->children[i - 1], key);
        }
        else {
            root->children[i] = deleteKey(root->children[i], key);
        }
    }
    
    // If root has 0 keys, make its child the new root
    if(root->numKeys == 0) {
        struct BTreeNode* newRoot = root->children[0];
        free(root);
        return newRoot;
    }
    
    return root;
}

// Print tree structure (for visualization)
void printTree(struct BTreeNode* root, int level) {
    if(root == NULL) return;
    
    printf("Level %d: ", level);
    for(int i = 0; i < root->numKeys; i++) {
        printf("%d ", root->keys[i]);
    }
    printf("\n");
    
    if(!root->isLeaf) {
        for(int i = 0; i <= root->numKeys; i++) {
            printTree(root->children[i], level + 1);
        }
    }
}

int main() {
    struct BTreeNode* root = NULL;
    int choice, key, n;
    
    printf("========== B-TREE CONSTRUCTION (Order 5, t=3) ==========\n");
    printf("1. Insert single key\n");
    printf("2. Insert multiple keys\n");
    printf("3. Delete a key\n");
    printf("4. Search a key\n");
    printf("5. Traverse (inorder)\n");
    printf("6. Print tree structure\n");
    printf("7. Exit\n");
    
    do {
        printf("\nEnter choice: ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1:
                printf("Enter key to insert: ");
                scanf("%d", &key);
                root = insert(root, key);
                printf("Key %d inserted!\n", key);
                break;
                
            case 2:
                printf("How many keys to insert? ");
                scanf("%d", &n);
                printf("Enter %d keys: ", n);
                for(int i = 0; i < n; i++) {
                    scanf("%d", &key);
                    root = insert(root, key);
                }
                printf("All keys inserted!\n");
                break;
                
            case 3:
                if(root == NULL) {
                    printf("Tree is empty!\n");
                } else {
                    printf("Enter key to delete: ");
                    scanf("%d", &key);
                    root = deleteKey(root, key);
                    printf("Key %d deleted (if existed)!\n", key);
                }
                break;
                
            case 4:
                if(root == NULL) {
                    printf("Tree is empty!\n");
                } else {
                    printf("Enter key to search: ");
                    scanf("%d", &key);
                    int pos;
                    struct BTreeNode* result = search(root, key, &pos);
                    if(result) {
                        printf("Key %d found!\n", key);
                    } else {
                        printf("Key %d not found!\n", key);
                    }
                }
                break;
                
            case 5:
                if(root == NULL) {
                    printf("Tree is empty!\n");
                } else {
                    printf("Inorder traversal: ");
                    traverse(root);
                    printf("\n");
                }
                break;
                
            case 6:
                if(root == NULL) {
                    printf("Tree is empty!\n");
                } else {
                    printf("\n--- Tree Structure ---\n");
                    printTree(root, 0);
                }
                break;
                
            case 7:
                printf("Exiting...\n");
                break;
                
            default:
                printf("Invalid choice!\n");
        }
    } while(choice != 7);
    
    return 0;
}