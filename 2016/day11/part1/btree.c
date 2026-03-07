#include <stdlib.h>
#include <stdio.h>
#include "btree.h"

#define M 10 // Maximum degree of the B-tree

struct BTreeNode {
    int num_keys; // Number of keys currently in the node
    BTreeKeyType keys[M-1]; // Array of keys
    struct BTreeNode *children[M]; // Array of child pointers
    int is_leaf; // True if node is a leaf
};

// Function to create a new node
struct BTreeNode *createNode(int is_leaf) {
    struct BTreeNode *newNode = (struct BTreeNode *)malloc(sizeof(struct BTreeNode));
    if (newNode == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    newNode->num_keys = 0;
    newNode->is_leaf = is_leaf;
    for (int i = 0; i < M; i++) {
        newNode->children[i] = NULL;
    }
    return newNode;
}

// Function to split a full child node
void splitChild(struct BTreeNode *parent, int index) {
    struct BTreeNode *child = parent->children[index];
    struct BTreeNode *newNode = createNode(child->is_leaf);
    
    newNode->num_keys = M/2 - 1;
    
    // Move keys and children to the new node
    for (int i = 0; i < M/2 - 1; i++) {
        newNode->keys[i] = child->keys[i + M/2];
    }
    
    if (!child->is_leaf) {
        for (int i = 0; i < M/2; i++) {
            newNode->children[i] = child->children[i + M/2];
        }
    }
    
    child->num_keys = M/2 - 1;
    
    // Shift parent's children to make space for the new node
    for (int i = parent->num_keys; i > index; i--) {
        parent->children[i + 1] = parent->children[i];
    }
    
    parent->children[index + 1] = newNode;
    
    // Shift parent's keys to insert the middle key from the child
    for (int i = parent->num_keys - 1; i >= index; i--) {
        parent->keys[i + 1] = parent->keys[i];
    }
    
    parent->keys[index] = child->keys[M/2 - 1];
    parent->num_keys++;
}

// Function to insert a key into a non-full node
void insertNonFull(struct BTreeNode *node, BTreeKeyType key) {
    int i = node->num_keys - 1;
    
    if (node->is_leaf) {
        // Insert key into the sorted order
        while (i >= 0 && node->keys[i] > key) {
            node->keys[i + 1] = node->keys[i];
            i--;
        }
        node->keys[i + 1] = key;
        node->num_keys++;
    } else {
        // Find the child to insert the key
        while (i >= 0 && node->keys[i] > key) {
            i--;
        }
        i++;
        
        if (node->children[i]->num_keys == M - 1) {
            // Split child if it's full
            splitChild(node, i);
            
            // Determine which of the two children is the new one
            if (node->keys[i] < key) {
                i++;
            }
        }
        insertNonFull(node->children[i], key);
    }
}

// Function to insert a key into the B-tree
void insert(struct BTreeNode **root, BTreeKeyType key) {
    // printf ("Adding key    %16.16lx\n", key);
    struct BTreeNode *node = *root;

    if (node == NULL) {
        // Create a new root node
        *root = createNode(1);
        (*root)->keys[0] = key;
        (*root)->num_keys = 1;
    } else {
        if (node->num_keys == M - 1) {
            // Split the root if it's full
            struct BTreeNode *new_root = createNode(0);
            new_root->children[0] = node;
            splitChild(new_root, 0);
            *root = new_root;
        }
        insertNonFull(*root, key);
    }
}

// Function to traverse and print the B-tree in-order
struct BTreeNode* findBTreeNode (struct BTreeNode *node, BTreeKeyType key) {
	if (node == NULL)
		return NULL;
	
	int i = node->num_keys - 1;
	while (i >= 0 && node->keys[i] > key) {
		i--;
	}
	if (i >= 0 && node->keys[i] == key) {
    		// printf ("Key           %16.16lx found\n", key);
		return node;
	}
	if (node->is_leaf) { // We cannot go any further
    		// printf ("Key           %16.16lx NOT found\n", key);
		return NULL;
	}
	if (i < 0 )
		return findBTreeNode(node->children[0], key);
	else
		return findBTreeNode(node->children[i+1], key);
}

void traverse(struct BTreeNode *root) {
    if (root != NULL) {
        int i;
        for (i = 0; i < root->num_keys; i++) {
            if (!root->is_leaf) printf("( ");
            traverse(root->children[i]);
            if (!root->is_leaf) printf(" )");
            printf("%16lx ", root->keys[i]);
        }
        if (!root->is_leaf) printf("( ");
        traverse(root->children[i]);
        if (!root->is_leaf) printf(")");
    }
}

