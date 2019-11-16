#include <iostream>
#include <random>
#include <time.h>

using namespace std;

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

BTNode *buildBST(int low, int high) {
    if (low > high) return nullptr;
    int mid = (low + high) / 2;
    BTNode *root = newNode(mid, high - low + 1);
    root->left = buildBST(low, mid - 1);
    root->right = buildBST(mid + 1, high);
    return root;
}

BTNode *buildTree(int n) {
    if (!n) return nullptr;
    return buildBST(1, n);
}

BTNode *osSelect(BTNode *root, int i) {
    if (root == nullptr) return nullptr;

    int currentOrder;
    if (root->left == nullptr ) {
        if(root->right == nullptr){
            currentOrder = 1;
        }else{
            currentOrder = root->right->size;
        }
    } else {
        currentOrder = root->left->size + 1;
    }

    if (currentOrder == i) {
        return root;
    } else if (i < currentOrder) {
        return osSelect(root->left, i);
    } else {
        return osSelect(root->right, i - currentOrder);
    }
}

BTNode *osDelete(BTNode *root, int i) {
    if (root == nullptr) return nullptr;

    int currentOrder;
    if (root->left == nullptr ) {
        if(root->right == nullptr){
            currentOrder = 1;
        }else{
            currentOrder = root->right->size;
        }
    } else {
        currentOrder = root->left->size + 1;
    }

    if (currentOrder > i) {
        //Element in the left part
        root->size--;
        root->left = osDelete(root->left, i);
        return root;
    } else if (currentOrder < i) {
        //Element in the right part
        root->size--;
        root->right = osDelete(root->right, i - currentOrder);
        return root;
    } else {
        //Element found, delete it
        if (root->left == nullptr) {            //1 or 0 nodes
            BTNode *temp = root->right;
            free(root);
            return temp;
        } else if (root->right == nullptr) {    //1 or 0 nodes
            BTNode *temp = root->left;
            free(root);
            return temp;
        } else {                                //2 nodes
            BTNode *swapWithParent, *swapWith = root->right;
            do {
                swapWith->size--;
                swapWithParent = swapWith;
                swapWith = swapWith->left;
            } while (swapWith->left != nullptr);

            root->key = swapWith->key;
            root->size--;
            swapWithParent->left = nullptr;
            free(swapWith);
            return root;
        }
    }
}

void runTests() {
    for (int i = 100; i <= 10000; i += 100) {
        for (int repeat = 0; repeat < 5; repeat++) {

        }
    }
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

void demo(int n) {
    srand (time(NULL));

    BTNode *root = buildTree(n);
    printPreOrder(root, 0);
    cout<<endl;

    int index;
    for (int i = 0; i < 3; i++) {
        index = rand() % n + 1;
        cout<<index<<endl;
        printPreOrder(osSelect(root, index), 0);
        cout<<endl;
    }
    index = rand() % n + 1;
//    root = osDelete(root, index);
    printPreOrder(root, 0);
    cout<<endl;
}

int main() {
    demo(11);
//    runTests();
    return 0;
}