#include <iostream>
#include <set>
#include "Profiler.h"

using namespace std;

enum {
    COLOR_WHITE = 0,
    COLOR_GRAY,
    COLOR_BLACK
};

typedef struct _Node {
    int key;
    vector<_Node*> adj;
    int color;
} Node;

void dfs(Node *root){

}

vector<Node*> generateConnectedGraph(int nrNodes, int nrEdges) {
    int i;
    vector<set < int>> adj(nrNodes);
    vector<Node*> graph(nrNodes);
    for (i = 1; i < nrNodes; i++) {
        int a = rand() % i;
        graph[i]->key=i;
        graph[i]->adj.push_back(graph[a]);
//        adj[a].insert(a);
    }
    while (i <= nrEdges) {
        int a = rand() % nrNodes, b = rand() % nrNodes;
        if (a != b && !adj[a].count(b) && !adj[b].count(a)) {
            graph[a]->adj.push_back(graph[b]);
            i++;
        }
    }

    return graph;
}

int main() {
    int a = 10;
    vector<Node*> graph = generateConnectedGraph(10, 30);
}