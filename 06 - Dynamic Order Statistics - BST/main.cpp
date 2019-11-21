/**
 *  Author: Hunor Debreczeni
 *
 *  Group: 30423
 *
 *      *Build Tree*
 *          Logic:
 *              Used a Divide Et Impera algorith, started from the middle, and kept going left/right.
 *              For initializing the size of the current root, we used the size of the array, since that's the size we need.
 *          Complexity:
 *              Since we go through each element from 1 to n, it takes O(n) time to do it.
 *
 *      *OS-Select* ( Select the i-th smallest element )
 *          Logic:
 *              For the current order of the element, we can get the left child's size + 1, since the root has only smaller
 *               elements on it's left and + itself makes the left child's size + 1-th smallest element.
 *          Complexity:
 *              Since it goes through the levels of the tree until it finds the value, it takes O(log n) time. In our
 *               analysis, since we do it for n times, it takes O(n*log n), that's why it's slower than the build function.
 *
 *      *OS-Delete* ( Delete the i-th smallest element )
 *          Logic:
 *              For finding the element we use the same technique as the OS-Select, but while going deeper in the tree,
 *               we decrement the current node's size, because we will remove an element under of it.
 *              For deleting the element we use the BST Delete method with finding it's successor if needed.
 *          Complexity:
 *              The select method takes up O(log n) as mentioned above + the delete method also needs O(log n) because we
 *               need to find the successor for the swap if needed.
 *              This makes up to O(2*log n) = O(log n). In our analysis, we do it n times, so it also takes O(n*log n)
 *
 *      *Chart*
 *          The fastest one is the build of the BST, because it's a linear function, takes O(n)
 *          Afterwards comes the OS-Select method.
 *          The OS-Delete does approximately 2 times more operations than the OS-Select, thus it has the same complexity
 *           as the OS-Select. It's because the OS-Delete algorithm also includes the OS-Select algorithm in itself.
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
    op.count(2);
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
        currentOrder = 1;
    } else {
        currentOrder = root->left->size + 1;
    }

    if (currentOrder == i) {
        return root;
    }
    if (i < currentOrder) {
        return osSelect(root->left, i, op);
    }
    return osSelect(root->right, i - currentOrder, op);
}

BTNode *osDelete(BTNode *root, int i, Operation op) {
    op.count();
    if (root == nullptr) return root;

    int currentOrder;
    op.count();
    if (root->left == nullptr) {
        currentOrder = 1;
    } else {
        currentOrder = root->left->size + 1;
    }

    if (currentOrder > i) {
        //Element in the left part
        op.count();
        root->size--;
        root->left = osDelete(root->left, i, op);
        return root;
    }
    if (currentOrder < i) {
        //Element in the right part
        op.count();
        root->size--;
        root->right = osDelete(root->right, i - currentOrder, op);
        return root;
    }

    //Element found, delete it
    op.count();
    if (root->left == nullptr) {
        op.count();//1 or 0 nodes
        BTNode *temp = root->right;
        delete root;
        return temp;
    }
    op.count();
    if (root->right == nullptr) {    //1 or 0 nodes
        op.count();
        BTNode *temp = root->left;
        delete root;
        return temp;
    }                                //2 nodes
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
    delete swapWith;
    return root;
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
        Operation buildOp = profiler.createOperation("Build", n);
        Operation selectOp = profiler.createOperation("Select", n);
        Operation deleteOp = profiler.createOperation("Delete", n);
        Operation totalOp = profiler.createOperation("Total", n);
        for (int repeat = 0; repeat < 5; repeat++) {

            BTNode *temp = buildTree(n, buildOp);
            int nTemp = n + 1;
            for (int i = 0; i < n; i++) {
                int index = rand() % nTemp-- + 1;
                osSelect(temp, index, selectOp);
                osDelete(temp, index, deleteOp);
            }
        }
        totalOp.count(buildOp.get() + selectOp.get() + deleteOp.get());
    }
    profiler.divideValues("Build", 5);
    profiler.divideValues("Select", 5);
    profiler.divideValues("Delete", 5);
    profiler.divideValues("Total", 5);
    profiler.createGroup("Group", "Build", "Select", "Delete", "Total");
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