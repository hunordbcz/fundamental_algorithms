/*
 *  Author: Hunor Debreczeni
 *
 *  Group: 30423
 *
 */

#include <iostream>
#include <ctime>
#include "Profiler.h"

using namespace std;

Profiler profiler("Dynamic Order Statistics");

typedef struct BinaryTreeNode {
    int key;
    int size;
    struct BinaryTreeNode *left;
    struct BinaryTreeNode *right;
} BTNode;

BTNode *newNode(int key, int size) {
    auto *node = new BTNode;
    node->key = key;
    node->size = size;
    return node;
}

BTNode *buildBST(int low, int high, Operation op) {
    op.count();
    if (low > high) return nullptr;
    op.count();
    int mid = (low + high) / 2;
    op.count();
    BTNode *root = newNode(mid, high - low + 1);
    op.count();
    root->left = buildBST(low, mid - 1, op);
    op.count();
    root->right = buildBST(mid + 1, high, op);
    return root;
}

BTNode *buildTree(int n, Operation op) {
    if (!n) return nullptr;
    op.count();
    return buildBST(1, n, op);
}

BTNode *osSelect(BTNode *root, int i, Operation op) {
    op.count();
    if (root == nullptr) return root;

    int currentOrder;
    op.count();
    if (root->left == nullptr) {
        op.count();
        if (root->right == nullptr) {
            currentOrder = 1;
        } else {
            currentOrder = root->right->size;
        }
    } else {
        currentOrder = root->left->size + 1;
    }

    if (currentOrder == i) {
        return root;
    } else if (i < currentOrder) {
        return osSelect(root->left, i, op);
    } else {
        return osSelect(root->right, i - currentOrder, op);
    }
}

BTNode *osDelete(BTNode *root, int i, Operation op) {
    op.count();
    if (root == nullptr) return root;

    int currentOrder;
    op.count();
    if (root->left == nullptr) {
        op.count();
        if (root->right == nullptr) {
            currentOrder = 1;
        } else {
            currentOrder = root->right->size;
        }
    } else {
        currentOrder = root->left->size + 1;
    }

    if (currentOrder > i) {
        //Element in the left part
        op.count();
        root->size--;
        root->left = osDelete(root->left, i, op);
        return root;
    } else if (currentOrder < i) {
        //Element in the right part
        op.count();
        root->size--;
        root->right = osDelete(root->right, i - currentOrder, op);
        return root;
    } else {
        //Element found, delete it
        op.count();
        if (root->left == nullptr) {
            op.count();//1 or 0 nodes
            BTNode *temp = root->right;
            free(root);
            return temp;
        } else {
            op.count();
            if (root->right == nullptr) {    //1 or 0 nodes
                op.count();
                BTNode *temp = root->left;
                free(root);
                return temp;
            } else {                                //2 nodes
                op.count(2);
                BTNode *swapWithParent = root, *swapWith = root->right;
                while (swapWith->left != nullptr) {
                    op.count(4);
                    swapWith->size--;
                    swapWithParent = swapWith;
                    swapWith = swapWith->left;
                }
                op.count();
                root->key = swapWith->key;
                op.count();
                root->size--;
                op.count();
                if (swapWithParent != root) {
                    op.count();
                    swapWithParent->left = swapWith->right;
                } else {
                    op.count();
                    root->right = swapWith->right;
                }
                free(swapWith);
                return root;
            }
        }
    }
}

BTNode *copyBT(BTNode *root) {
    if (root == nullptr) return nullptr;
    auto *temp = new BTNode;
    temp->key = root->key;
    temp->size = root->size;
    temp->left = copyBT(root->left);
    temp->left = copyBT(root->right);
    return temp;
}

void printPreOrder(BTNode *root, int level) {
    if (root == nullptr) return;
    cout << "|";
    for (int i = 0; i < level; i++) {
        cout << "---";
    }
    cout << root->key << "(" << root->size << ")\n";
    if (root->left != nullptr) {
        printPreOrder(root->left, level + 1);
    }
    if (root->right != nullptr) {
        printPreOrder(root->right, level + 1);
    }
}

void print(BTNode *root) {
    printPreOrder(root, 0);
    cout << endl;
}

void runTests() {
    for (int n = 100; n <= 10000; n += 100) {
        for (int repeat = 0; repeat < 5; repeat++) {
            Operation buildOp = profiler.createOperation("Build", n);
            Operation selectOp = profiler.createOperation("Select", n);
            Operation deleteOp = profiler.createOperation("Delete", n);
            BTNode *temp = buildTree(n, buildOp);
            for (int i = 0; i < n; i++) {
                int index = rand() % n + 1;
                osSelect(temp, index, selectOp);
                osDelete(temp, index, deleteOp);
            }
        }
    }
    profiler.divideValues("Build", 5);
    profiler.divideValues("Select", 5);
    profiler.divideValues("Delete", 5);
    profiler.createGroup("Group", "Build", "Select", "Delete");
    profiler.showReport();
}

void demo(int n) {
    srand(time(nullptr));
    Operation dummy = profiler.createOperation("Dummy", 0);
    cout << "Tree after Build:" << endl;
    BTNode *root = buildTree(n, dummy);
    print(root);

    int index;
    for (int i = 0; i < 3; i++) {
        index = rand() % n + 1;
        cout << "Selected: " << index << ". element" << endl;
        print(osSelect(root, index, dummy));
    }
    index = rand() % n + 1;;
    cout << "To be deleted: " << index << ". element" << endl;
    osDelete(root, index, dummy);
    print(root);
}

int main() {
    runTests();
    demo(11);
    return 0;
}