/*
Implementation of a Red-Black tree in c
For this I mostly used the pseudocode from the textbook Introduction to Alogrithms thrid edition as a guide in creating the tree
Inside the main there are 7 different test cases to test this implemention 
*/

#include <stdio.h>
#include <stdlib.h>


typedef enum { RED, BLACK } Color;


typedef struct Node {
   int key;
   struct Node* left;
   struct Node* right;
   struct Node* parent; 
   Color color;
} Node;


typedef struct RBTree {
   Node* root;
} RBTree;


// prototypes
void leftRotate(RBTree* T, Node* x);
void rightRotate(RBTree* T, Node* y);
void rbInsert(RBTree* T, Node* z);
void rbInsertFixup(RBTree* T, Node* z);
void rbDelete(RBTree* T, Node* z);
void rbDeleteFixup(RBTree* T, Node* x);
Node* treeMinimum(Node* x);
void rbTransplant(RBTree* T, Node* u, Node* v);
Node* createNode(int key);
RBTree* createRBTree();


// left rotate
void leftRotate(RBTree* T, Node* x) {
   if (x == NULL) {
       return;
   }
   Node* y = x->right;  
   if (y != NULL) {
       // turn y's left subtree into x's right subtree
       x->right = y->left;
       if (y->left != NULL) {
           y->left->parent = x;
       }
       y->parent = x->parent;
       if (x->parent == NULL) {
           T->root = y;
       } else if (x == x->parent->left) {
           x->parent->left = y;
       } else {
           x->parent->right = y;
       }
       // put x on y's left
       y->left = x;
       x->parent = y;
   }
}


// Right Rotate
void rightRotate(RBTree* T, Node* y) {
   if (y == NULL) {
       return;
   }
   Node* x = y->left;
   if (x != NULL) {
       // turn x's right subtree into y's left subtree
       y->left = x->right;
       if (x->right != NULL) {
           x->right->parent = y;
       }
       x->parent = y->parent;
       if (y->parent == NULL) {
           T->root = x;
       } else if (y == y->parent->right) {
           y->parent->right = x;
       } else {
           y->parent->left = x;
       }
       // put y on x's left
       x->right = y;
       y->parent = x;
   }
}


// Basic rb insertion
void rbInsert(RBTree* T, Node* z) {
   if (z == NULL) {
       return;
   }


   Node* y = NULL;
   Node* x = T->root;


   while (x != NULL) {
       y = x;
       if (z->key < x->key) {
           x = x->left;
       } else {
           x = x->right;
       }
   }


   z->parent = y;
   if (y == NULL) {
       T->root = z;
   } else if (z->key < y->key) {
       y->left = z;
   } else {
       y->right = z;
   }


   z->left = NULL;
   z->right = NULL;
   z->color = RED;


   rbInsertFixup(T, z);
}


// fix the tree after insertion
void rbInsertFixup(RBTree* T, Node* z) {
   if (z == NULL) {
       return;
   }
   while (z->parent != NULL && z->parent->color == RED) {
       if (z->parent == z->parent->parent->left) {
           Node* y = z->parent->parent->right;
           if (y != NULL && y->color == RED) {  
               z->parent->color = BLACK;
               y->color = BLACK;
               z->parent->parent->color = RED;
               z = z->parent->parent;
           } else {
               if (z == z->parent->right) { 
                   z = z->parent;
                   leftRotate(T, z);
               }
               z->parent->color = BLACK;
               z->parent->parent->color = RED;
               rightRotate(T, z->parent->parent);
           }
       } else {
           Node* y = z->parent->parent->left;
           if (y != NULL && y->color == RED) {
               z->parent->color = BLACK;
               y->color = BLACK;
               z->parent->parent->color = RED;
               z = z->parent->parent;
           } else {
               if (z == z->parent->left) {
                   z = z->parent;
                   rightRotate(T, z);
               }
               z->parent->color = BLACK;
               z->parent->parent->color = RED;
               leftRotate(T, z->parent->parent);
           }
       }
   }
   T->root->color = BLACK;
}


// delete a node from the rb tree
void rbDelete(RBTree* T, Node* z) {
   if (z == NULL) {
       return;
   }


   Node* y = z;
   Node* x;
   Color yOriginalColor = y->color;


   if (z->left == NULL) {
       x = z->right;
       rbTransplant(T, z, z->right);
   } else if (z->right == NULL) {
       x = z->left;
       rbTransplant(T, z, z->left);
   } else {
       y = treeMinimum(z->right);
       yOriginalColor = y->color;
       x = y->right;
       if (y->parent == z) {
           x->parent = y;
       } else {
           rbTransplant(T, y, y->right);
           y->right = z->right;
           y->right->parent = y;
       }
       rbTransplant(T, z, y);
       y->left = z->left;
       y->left->parent = y;
       y->color = z->color;
   }
   if (yOriginalColor == BLACK) {
       rbDeleteFixup(T, x);
   }
}


// fix up the tree after a deletion
void rbDeleteFixup(RBTree* T, Node* x) {
   if (x == NULL) {
       return;
   }

   while (x != T->root && x->color == BLACK) {
       if (x == x->parent->left) {
           Node* w = x->parent->right;
           if (w->color == RED) {
               w->color = BLACK;
               x->parent->color = RED;
               leftRotate(T, x->parent);
               w = x->parent->right;
           }
           if (w->left->color == BLACK && w->right->color == BLACK) {
               w->color = RED;
               x = x->parent;
           } else {
               if (w->right->color == BLACK) {
                   w->left->color = BLACK;
                   w->color = RED;
                   rightRotate(T, w);
                   w = x->parent->right;
               }
               w->color = x->parent->color;
               x->parent->color = BLACK;
               w->right->color = BLACK;
               leftRotate(T, x->parent);
               x = T->root;
           }
       } else {
           Node* w = x->parent->left;
           if (w->color == RED) {
               w->color = BLACK;
               x->parent->color = RED;
               rightRotate(T, x->parent);
               w = x->parent->left;
           }
           if (w->right->color == BLACK && w->left->color == BLACK) {
               w->color = RED;
               x = x->parent;
           } else {
               if (w->left->color == BLACK) {
                   w->right->color = BLACK;
                   w->color = RED;
                   leftRotate(T, w);
                   w = x->parent->left;
               }
               w->color = x->parent->color;
               x->parent->color = BLACK;
               w->left->color = BLACK;
               rightRotate(T, x->parent);
               x = T->root;
           }
       }
   }
   x->color = BLACK;
}


// find minimum node
Node* treeMinimum(Node* x) {
   if (x == NULL) {
       return NULL;
   }
   while (x->left != NULL) {
       x = x->left;
   }
   return x;
}


// transplant one subtree for another
void rbTransplant(RBTree* T, Node* u, Node* v) {
   if (u == NULL) {
       return;
   }
   if (u->parent == NULL) {
       T->root = v;
   } else if (u == u->parent->left) {
       u->parent->left = v;
   } else {
       u->parent->right = v;
   }
   if (v != NULL) {
       v->parent = u->parent;
   }
}


// create a new node
Node* createNode(int key) {
   Node* newNode = (Node*)malloc(sizeof(Node));
   if (newNode == NULL) {
       return NULL;
   }
   newNode->key = key;
   newNode->left = NULL;
   newNode->right = NULL;
   newNode->parent = NULL;
   newNode->color = RED;
   return newNode;
}


// create a new Red-Black tree
RBTree* createRBTree() {
   RBTree* T = (RBTree*)malloc(sizeof(RBTree));
   if (T == NULL) {
       return NULL;
   }
   T->root = NULL;
   return T;
}


int main() {
   RBTree* T = createRBTree();

   // Test Case 1: Insert nodes and check root
   Node* n1 = createNode(10);
   Node* n2 = createNode(20);
   Node* n3 = createNode(15);
   rbInsert(T, n1);
   rbInsert(T, n2);
   rbInsert(T, n3);
   printf("Root of the tree after insertions: %d\n", T->root->key);


   // Test Case 2: Delete a node with no children (leaf node)
   rbDelete(T, n2);  // Delete node 20 (leaf)
   printf("Root of the tree after deleting 20 (leaf node): %d\n", T->root->key);


   // Test Case 3: Insert more nodes (nodes with children)
   Node* n4 = createNode(5);
   Node* n5 = createNode(25);
   rbInsert(T, n4);  // Insert node 5
   rbInsert(T, n5);  // Insert node 25
   printf("Root of the tree after inserting 5 and 25: %d\n", T->root->key);


   // Test Case 4: Insert more nodes to trigger tree balancing
   Node* n6 = createNode(30);
   Node* n7 = createNode(35);
   rbInsert(T, n6);
   rbInsert(T, n7);
   printf("Root of the tree after inserting 30 and 35: %d\n", T->root->key);


   // Test Case 5: Delete a node with two children (this should trigger tree rebalancing)
   rbDelete(T, n1);  // Delete node 10 (with two children)
   printf("Root of the tree after deleting 10 (node with two children): %d\n", T->root->key);


   // Test Case 6: Delete a node with one child
   rbDelete(T, n4);  // Delete node 5 (with one child)
   printf("Root of the tree after deleting 5 (node with one child): %d\n", T->root->key);


   // Test Case 7: Delete the root node and check if tree rebalances correctly
   rbDelete(T, T->root);  // Delete the root node
   if (T->root != NULL) {
       printf("Root of the tree after deleting the root node: %d\n", T->root->key);
   } else {
       printf("The tree is empty after deleting the root node.\n");
   }
   return 0;
}
