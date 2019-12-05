/**
 *  Author: Hunor Debreczeni
 *  Group: 30423
 *
 *  Problem spec.: Implement correctly and efficiently the base operations for disjoint set and the Kruskal’s algorithm
 *
 *      *Make-Set*
 *          Makes a vector with n nodes.
 *          It has a complexity of O(n) in the Kruskal algorithm
 *
 *      *Find*
 *          Recursively finds the parent of a node
 *          It has a complexity of O(n*log n) in the Kruskal algorithm
 *
 *      *Union*
 *          Finds the parents of both the given values and if they differ, make the parent of the one with the smaller
 *           rank the other one.
 *          It has a complexity of O(n*log n) in the Kruskal algorithm
 *
 *      *Kruskal*
 *          Takes a graph, sort the edges according to their weight.
 *          For each edge, starting from the smallest one, checks if the two vertexes are in a set or not. If not, then
 *           make them be. We do this until we reach n-1 edges, because that means that we have the MST.
 */

#include <iostream>
#include <vector>
#include <time.h>
#include <set>
#include "Profiler.h"

using namespace std;

Profiler profiler("Disjoint Sets");

typedef struct DSNode {
    int key;
    int rank;
    struct DSNode *parent;
} DSNode;

typedef struct Edge {
    pair<int, int> vertex;
    int weight;
} Edge;

typedef struct Graph {
    int nrVertices;
    vector<Edge> edges;
} Graph;

DSNode *newNode(int key) {
    auto *node = new DSNode;
    node->key = key;
    node->rank = 0;
    node->parent = node;
    return node;
}

vector<DSNode *> makeSet(int n, Operation op) {
    vector<DSNode *> tree;
    tree.reserve(n);
    for (int i = 0; i < n; i++) {
        op.count(3);
        tree.push_back(newNode(i));
    }
    return tree;
}

DSNode *findSet(vector<DSNode *> tree, int i, Operation op) {
    op.count();
    if (tree[i]->parent != tree[i]) {
//        return findSet(tree, tree[i]->parent->key, op);
        op.count();
        tree[i]->parent = findSet(tree, tree[i]->parent->key, op);   // Another method
    }
    return tree[i]->parent;
}

bool unionSet(vector<DSNode *> tree, int x, int y, Operation unionOp, Operation findOp) {
    unionOp.count(2);
    DSNode *nodeA = findSet(tree, x, findOp);
    DSNode *nodeB = findSet(tree, y, findOp);
    unionOp.count();
    if (nodeA == nodeB) return false;
    unionOp.count();
    if (nodeA->rank > nodeB->rank) {
        unionOp.count();
        nodeB->parent = nodeA;
        return true;
    } else {
        unionOp.count();
        if (nodeA->rank < nodeB->rank) {
            nodeA->parent = nodeB;
            unionOp.count();
            return true;
        } else {
            unionOp.count();
            nodeB->parent = nodeA;
            nodeA->rank++;
            return true;
        }
    }
}

void printSet(DSNode *node) {
    cout << node->key << "(" << node->rank << ") ";
    if (node->parent != node) {
        printSet(node->parent);
    }
}

void printVectorSet(vector<DSNode *> tree) {
    for (auto &i : tree) {
        printSet(i);
        cout << endl;
    }
    cout << endl;
}

void exemplifyCorrectness(int n) {
    Operation dummy = profiler.createOperation("Dummy", 0);
    vector<DSNode *> tree = makeSet(n, dummy);
    for (int i = 0; i < n; i++) {
        int a = rand() % n, b = rand() % n, c = rand() % n;

        unionSet(tree, a, b, dummy, dummy);
        cout << "\nEach Set after Random Union " << "(" << a << "," << b << "):\n";
        printVectorSet(tree);
        cout << "Random Set " << c << " :";
        printSet(findSet(tree, c, dummy));
    }
}

Graph *generateConnectedGraph(int n) {
    auto *graph = new struct Graph;
    set <pair<int, int>> addedEdges;
    addedEdges.insert({-1,-1});
    pair<int, int> edgePair;
    graph->nrVertices = n;
    int i;
    for (i = 1; i < n; i++) {
        Edge *edge = new struct Edge;
        int a = rand() % i, b = i;
        edge->weight = rand() % 10000;
        edge->vertex.first = a;
        edge->vertex.second = b;
        edgePair = {a, b};
        addedEdges.insert(edgePair);
        graph->edges.push_back(*edge);
    }
    while (i <= 4 * n) {
        Edge *edge = new struct Edge;
        edge->weight = rand() % 10000;
        int a = rand() % n, b = rand() % n;
        if (a < b) {
            edge->vertex.first = a;
            edge->vertex.second = b;
            edgePair = {a, b};
        } else if (a > b) {
            edge->vertex.first = b;
            edge->vertex.second = a;
            edgePair = {b, a};
        } else {
            edgePair = {-1, -1};
        }
        if (addedEdges.insert(edgePair).second) {
            graph->edges.push_back(*edge);
            i++;
        }
    }

    return graph;
}

bool compareByWeight(Edge &a, Edge &b) {
    return a.weight < b.weight;
}

void kruskal(Graph *graph, int n, Operation findOp, Operation makeOp, Operation unionOp) {
    vector<DSNode *> tree = makeSet(n, makeOp);

    auto *resultGraph = new struct Graph;
    resultGraph->nrVertices = n;

    sort(graph->edges.begin(), graph->edges.end(), compareByWeight);

    for (auto &edge : graph->edges) {
        if (resultGraph->edges.size() == n - 1) {
            break;
        }
        int first = findSet(tree, edge.vertex.first, findOp)->key,
                second = findSet(tree, edge.vertex.second, findOp)->key;
        if (first != second) {
            resultGraph->edges.push_back(edge);
            unionSet(tree, first, second, unionOp, findOp);
        }
    }
}

void runTests() {
    for (int n = 100; n <= 10000; n += 100) {
        cout << n << endl;
        Operation findOp = profiler.createOperation("Find", n);
        Operation makeOp = profiler.createOperation("Make", n);
        Operation unionOp = profiler.createOperation("Union", n);
        Operation totalOp = profiler.createOperation("Total", n);
        Graph *graph = generateConnectedGraph(n);
        kruskal(graph, n, findOp, makeOp, unionOp);
        totalOp.count(findOp.get() + makeOp.get() + unionOp.get());
    }
    profiler.createGroup("Effort", "Union", "Find", "Make", "Total");
    profiler.showReport();
}

int main() {
    srand(time(nullptr));
    runTests();
    exemplifyCorrectness(10);
    return 0;
}