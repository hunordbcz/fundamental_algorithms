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
    set<_Node *> adj;
    int color;

    bool onStack = false;
    int disc = -1;
    int low = -1;
} Node;

Node *newNode(int key) {
    auto *node = new Node;

    node->key = key;
    node->color = COLOR_WHITE;
    return node;
}

vector<Node *> generateConnectedGraph(int nrNodes, int nrEdges) {
    int i;
    vector<Node *> graph;
    for (i = 0; i < nrNodes; i++) {
        graph.push_back(newNode(i));
    }
    i = 0;
    while (i < nrEdges) {
        int a = rand() % nrNodes, b = rand() % nrNodes;
        if (a != b && !graph[a]->adj.count(graph[b])) {
            graph[a]->adj.insert(graph[b]);
            i++;
        }
    }
    return graph;
}

void dfs(Node *root) {
    root->color = COLOR_BLACK;
    cout << root->key << endl;
    set<Node *>::iterator i;
    for (i = root->adj.begin(); i != root->adj.end(); ++i) {
        Node *element = *i;
        if (element->color != COLOR_BLACK) {
            dfs(element);
        }
    }
}

void tarjan(Node *u, vector<Node *> &stack, int &index) {

    u->disc = u->low = ++index;
    stack.push_back(u);
    u->onStack = true;

    set<Node *>::iterator i;
    for (i = u->adj.begin(); i != u->adj.end(); i++) {
        Node *element = *i;
        if (element->disc == -1) {
            tarjan(element, stack, index);
            u->low = min(u->low, element->low);
        } else if (element->onStack) {
            u->low = min(u->low, element->disc);
        }
    }

    Node *popped;
    if(u->low == u->disc){
        while(stack.back()!= u){
            popped = stack.back();
            cout<<popped->key<< " ";
            popped->onStack = false;
            stack.pop_back();
        }
        popped = stack.back();
        cout<<popped->key<<endl;
        popped->onStack=false;
        stack.pop_back();
    }

}

void scc(vector<Node *> graph) {
    vector<Node *> stack;
    int index = 0;
    for (auto & i : graph) {
        if (i->disc == -1) {
            tarjan(i, stack, index);
        }
    }
}

void printGraphAdj(vector<Node*> graph){
    cout<<"Graph with adjacency nodes:\n";
    for(int i =0;i<graph.size();i++){
        cout<<i<<": ";
        set<Node *>::iterator j;
        for (j = graph[i]->adj.begin(); j != graph[i]->adj.end(); j++) {
            Node *element = *j;
            cout<<element->key<<" ";
        }
        cout<<endl;
    }
}

int main() {
    srand(time(nullptr));
    vector<Node *> graph = generateConnectedGraph(5, 8);
    printGraphAdj(graph);
    cout<<"\nDepth-First Search:\n";
    dfs(graph[0]);
    cout<<"\nSCC\n";
    scc(graph);
    return 0;
}