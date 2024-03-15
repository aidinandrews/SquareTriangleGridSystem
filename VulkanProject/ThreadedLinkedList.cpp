#include "ThreadedLinkedList.h"

class BST {
    int key, height;
    BST* left, * right;

public:
    BST();
    BST(int key);
	//~BST();

	BST* insert(BST* BST, int key);
	BST* remove(BST* root, int key);

    void inOrder(BST* root);
};

BST::BST() {
    key = height = 0;
    left = right = nullptr;
}

BST::BST(int key) : key(key) {
    height = 0;
    left = right = nullptr;
}

BST* BST::insert(BST* root, int value) {
    // Actual insertion logic, once we have traversed to the bottom of the tree:
	if (root == nullptr) {
        BST* leaf = new BST(value);
        return leaf;
	}
	// Mildly convoluted insertions using recursion:
    if (value > root->key) {
        root->right = insert(root->right, value);
    }
    if (value < root->key) {
        root->left = insert(root->left, value);
    }
    // Once the leaf has been inserted, all the many insert() calls 
    // that found it will return their own pointers to their parents!
    return root;
}

BST* BST::remove(BST* root, int key) {
    // Base case
    if (root == NULL)
        return root;

    if (root->key > key) {
        root->left = remove(root->left, key); // <- Recursively go deeper into the BST.
        return root; // <- run back out of the BST once finished.
    }
    else if (root->key < key) {
        root->right = remove(root->right, key); // <- Recursively go deeper into the BST.
        return root; // <- run back out of the BST once finished.
    }

    // We reach here when root is the BST to be deleted.

    // If one of the children is empty
    if (root->left == NULL) {
        BST* temp = root->right;
        delete root;
        return temp;
    }
    else if (root->right == NULL) {
        BST* temp = root->left;
        delete root;
        return temp;
    }

    // If both children exist
    else {
        BST* succParent = root;
        // Find successor
        BST* succ = root->right;
        while (succ->left != NULL) { // <- Must go one node to the right then as left as possible to find the node's successor.
            succParent = succ;
            succ = succ->left;
        }

        // Delete successor.  Since successor is always left child of its parent
        // we can safely make successor's right right child as left of its parent.
        // If there is no succ, then assign succ->right to succParent->right
        if (succParent != root)
            succParent->left = succ->right;
        else
            succParent->right = succ->right;

        // Copy Successor Data to root
        root->key = succ->key;

        // Delete Successor and return root
        delete succ;
        return root;
    }
}

void BST::inOrder(BST* root) {
    if (root == nullptr) {
        return;
    }
    // function will always head left, print when it is as left as possible, 
    // then head up and right only when it can't head left.
    inOrder(root->left);
    std::cout << root->key << std::endl;
    inOrder(root->right);
}