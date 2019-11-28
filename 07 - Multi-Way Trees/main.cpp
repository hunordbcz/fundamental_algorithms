/**
 *  Author: Hunor Debreczeni
 *  Group: 30423
 *
 *  Transformations between different representations
 *
 *      *Parental Representation -> Multi-Way Tree*
 *          Structure Used for Multi-Way Tree:
 *              key ->
 *                  used to remember the node key
 *              vector of pointers to the other nodes ->
 *                  used to remember the children of the node
 *          Algorithm:
 *              First we initialize a vector of size n with their key and a vector of children with no elements in it.
 *              We go through all of the elements of the array with the parental representation.
 *              If we find an element that's value is not -1 ( = not the root node ), then we push the given index node
 *               to the parent node's children vector.
 *              If we find the value -1, then we know which one is the root value, so we save it
 *          Additional Space:
 *              We don't use any additional space, because we initialize the vector, but later those values become the
 *               actual elements in the Multi Way Tree
 *              In conclusion, the space complexity is O(n)
 *          Complexity:
 *              We go through each value once, so it makes the time complexity O(n)
 *      *Multi-Way Tree -> Binary Tree*
 *          Structure used:
 *              Default Binary Tree structure
 *          Algorithm:
 *              We start from the root. The left node of the tree will be the children of consecutive nodes and the right,
 *               the root's siblings. In order to remember the siblings, we pass to the recursive call the siblings also,
 *               and we pop each time the current sibling, so we can always take the first sibling.
 *          Additional Space:
 *              There is no additional space used, only the structure is used and it's size.
 *          Complexity:
 *              We recursively go through each element, so it makes an O(n) time complexity.
 *              Space complexity is also O(n).
 */

#include <iostream>
#include <vector>

using namespace std;

typedef struct multiWayTreeNode {
    int key;
    vector<struct multiWayTreeNode *> children;
} multiWayTreeNode;

typedef struct binaryTreeNode {
    int key;
    struct binaryTreeNode *left;
    struct binaryTreeNode *right;
} binaryTreeNode;

multiWayTreeNode *pop_front(vector<multiWayTreeNode *> &vect) {
    if (vect.size()) {
        multiWayTreeNode *response = vect[0];
        vect.erase(vect.begin());
        return response;
    }
    exit(-1);
}

multiWayTreeNode *newMultiWayNode(int key) {
    auto *node = new multiWayTreeNode;
    node->key = key;
    return node;
}

binaryTreeNode *newBinaryNode(int key) {
    auto *node = new binaryTreeNode;
    node->key = key;
    node->left = nullptr;
    node->right = nullptr;
    return node;
}

vector<multiWayTreeNode *> initialize(int n) {
    vector < multiWayTreeNode * > tree;
    tree.reserve(n);
    for (int i = 0; i < n; i++) {
        tree.push_back(newMultiWayNode(i));
    }
    return tree;
}

multiWayTreeNode *parentToMultiWay(vector<int> parentRepr) {
    vector < multiWayTreeNode * > tree = initialize(parentRepr.size());
    int root = -1;
    for (int i = 1; i < parentRepr.size(); i++) {
        if (parentRepr[i] != -1) {
            tree[parentRepr[i]]->children.push_back(tree[i]);
        } else {
            root = i;
        }
    }
    if (root == -1) {
        return nullptr;
    }
    return tree[root];
}

binaryTreeNode *multiWayToBinary(multiWayTreeNode *root, vector<multiWayTreeNode *> siblings) {
    if (!root) {
        return nullptr;
    }
    binaryTreeNode *temp = newBinaryNode(root->key);
    if (!siblings.empty()) {
        multiWayTreeNode *firstSibling = pop_front(siblings);
        temp->right = multiWayToBinary(firstSibling, siblings);
    }
    if (!root->children.empty()) {
        vector < multiWayTreeNode * > children = root->children;
        multiWayTreeNode *firstChild = pop_front(children);
        temp->left = multiWayToBinary(firstChild, children);
    }
    return temp;
}

void printPreBinary(binaryTreeNode *root, int level) {
    if (root == nullptr) return;
    for (int i = 0; i < level; i++) {
        if (i == level - 1) {
            cout << "|__";
        } else {
            cout << "   ";
        }
    }
    cout << root->key << "\n";
    if (root->left != nullptr) {
        printPreBinary(root->left, level + 1);
    }
    if (root->right != nullptr) {
        printPreBinary(root->right, level + 1);
    }
}

void printPreMulti(multiWayTreeNode *root, vector<multiWayTreeNode *> siblings, int level) {
    if (root == nullptr) return;
    for (int i = 0; i < level; i++) {
        if (i == level - 1) {
            cout << "|__";
        } else {
            cout << "   ";
        }
    }
    cout << root->key << "\n";
    if (!root->children.empty()) {
        vector < multiWayTreeNode * > children = root->children;
        multiWayTreeNode *firstChild = pop_front(children);
        printPreMulti(firstChild, children, level + 1);
    }
    if (!siblings.empty()) {
        multiWayTreeNode *firstSibling = pop_front(siblings);
        printPreMulti(firstSibling, siblings, level);
    }
}

void printParental(vector<int> data) {
    cout << "\nParental Representation Tree:\n";
    for (int i: data) {
        if (i != 0) {
            cout << i << " ";
        }
    }
    cout << endl;
    for (int i = 1; i < data.size(); i++) {
        cout << i << " ";
    }
    cout << endl;
}

void printMulti(multiWayTreeNode *root) {
    cout << "\nMulti Way Tree:\n";
    printPreMulti(root, {}, 0);
    cout << endl;
}

void printBinary(binaryTreeNode *root) {
    cout << "\nBinary Tree:\n";
    printPreBinary(root, 0);
    cout << endl;
}

void demo() {
    vector<int> parentRepr = {0, 2, 7, 5, 2, 7, 7, -1, 5, 2};
    multiWayTreeNode *multiWayRoot = parentToMultiWay(parentRepr);
    binaryTreeNode *binaryRoot = multiWayToBinary(multiWayRoot, {});
    printParental(parentRepr);
    printMulti(multiWayRoot);
    printBinary(binaryRoot);
}

int main() {
    demo();
    return 0;
}