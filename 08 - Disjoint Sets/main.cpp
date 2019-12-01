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
    if (tree[i]->parent == tree[i]) {
        return tree[i];
    }
    return findSet(tree, tree[i]->parent->key, op);
}

void unionSet(vector<DSNode *> tree, int x, int y, Operation op) {
    op.count();
    if (x == y) {
        return;
    }
    op.count(2);
    DSNode *nodeA = findSet(tree, x, op);
    DSNode *nodeB = findSet(tree, y, op);
    op.count();
    if (nodeA == nodeB) return;
    op.count();
    if (nodeA->rank > nodeB->rank) {
        op.count();
        nodeB->parent = nodeA;
    } else {
        op.count();
        if (nodeA->rank < nodeB->rank) {
            nodeA->parent = nodeB;
            op.count();
        } else {
            op.count();
            nodeB->parent = nodeA;
            nodeA->rank++;
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

        unionSet(tree, a, b, dummy);
        cout << "\nEach Set after Random Union " << "(" << a << "," << b << "):\n";
        printVectorSet(tree);
        cout << "Random Set " << c << " :";
        printSet(findSet(tree, c, dummy));
    }
}

Graph *generateConnectedGraph(int n) {
    auto *graph = new struct Graph;
    set <pair<int, int>> addedEdges;
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
        } else {
            edge->vertex.first = b;
            edge->vertex.second = a;
            edgePair = {b, a};
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
        int first = edge.vertex.first, second = edge.vertex.second;
        if (findSet(tree, first, findOp) != findSet(tree, second, findOp)) {
            resultGraph->edges.push_back(edge);
            unionSet(tree, first, second, unionOp);
        }
    }
}

void runTests() {
    Graph *graph = generateConnectedGraph(10);
    for (int n = 100; n <= 10000; n += 100) {
        Operation findOp = profiler.createOperation("Find", n);
        Operation makeOp = profiler.createOperation("Make", n);
        Operation unionOp = profiler.createOperation("Union", n);
        Operation totalOp = profiler.createOperation("Total", n);
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