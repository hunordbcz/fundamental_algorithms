/**
 *  Author: Hunor Debreczeni
 *  Group: 30423
 *
 *  Assignment: Implement correctly and efficiently:
 *                  - the depth­first search algorithm
 *                  - Tarjan algorithm for detecting strongly connected components
 *                  - topological sorting
 *
 *      *Depth-First Search*:
 *          Algorithm: Goes deep through the adjacent nodes, then take the other children
 *          Complexity:
 *              It depends mostly on the edges, because the node from where we start may have no adjacent nodes.
 *              This way, the algorithm will have a complexity of O(1).
 *              Otherwise, if we get 'lucky', the complexity depends both on the vertexes and the edges,
 *               so it will be O(V + E).
 *
 *      *Tarjan Algorithm*:
 *          Logic:
 *              Finds all the strongly connected graphs ( there is a path between each node ).
 *              The program indexes each node with the smallest node that it can reach. If we finish marking the
 *               nodes and can't go further, we print the nodes from the stack until we reach the original node.
 *          Complexity:
 *              Goes through each edge for all the vertexes, so it has an O( V + E ) complexity.
 *
 *      *Topological Sort*:
 *          Logic:
 *              Same as DFS with the only difference that it goes through each node and we save the path of the tree
 *               in a stack, which will be the result of the topological sort at the end.
 *              In case the graph has a loop, it can't be sorted topologically. For this check I used Tarjan's algorithm,
 *               to check if there's a loop or not. In this case, the result will be an empty list.
 *          Complexity:
 *              Because it's a Directed Acyclic Graph, we don't have a problem with not having adjacent nodes,
 *               so it's of O( V + E ).
 *
 *      *Chart*:
 *          On both cases, with random edges and with random vertexes, we can clearly see, that the running time is
 *           of O( V + E ). For each vertex we set it's color and for each edge we check if the other node was colored
 *           already or not. We have many edges and many vertexes so our chance for the node to have zero adjacent nodes
 *           is very small, and so on for the next one, and so on.
 *
 */

#include <iostream>
#include <set>
#include "Profiler.h"

using namespace std;

Profiler profiler("Depth-First Search");

enum {
    COLOR_WHITE = 0,
    COLOR_BLACK
};

typedef struct _Node {
    int key{};
    set<_Node *> adj;
    int color{};

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

void dfs(Node *root, Operation op) {
    op.count();
    root->color = COLOR_BLACK;
    cout << root->key << endl;
    set<Node *>::iterator i;
    for (i = root->adj.begin(); i != root->adj.end(); ++i) {
        Node *element = *i;
        op.count();
        if (element->color != COLOR_BLACK) {
            dfs(element, op);
        }
    }
}

void tarjanRec(Node *u, vector<Node *> &stack, int &index, bool &hasLoop, bool print) {

    u->disc = u->low = ++index;
    stack.push_back(u);
    u->onStack = true;

    set<Node *>::iterator i;
    for (i = u->adj.begin(); i != u->adj.end(); i++) {
        Node *element = *i;
        if (element->disc == -1) {
            tarjanRec(element, stack, index, hasLoop, print);
            u->low = min(u->low, element->low);
        } else if (element->onStack) {
            u->low = min(u->low, element->disc);
        }
    }

    Node *popped;
    if (u->low == u->disc) {
        while (stack.back() != u) {
            hasLoop = true;
            popped = stack.back();
            if(print) {
                cout << popped->key << " ";
            }
            popped->onStack = false;
            stack.pop_back();
        }
        popped = stack.back();
        if(print) {
            cout << popped->key << endl;
        }
        popped->onStack = false;
        stack.pop_back();
    }
}

bool tarjan(vector<Node *> graph, bool print) {
    bool hasLoop = false;
    vector<Node *> stack;
    int index = 0;
    for (auto &i : graph) {
        if (i->disc == -1) {
            tarjanRec(i, stack, index, hasLoop, print);
        }
    }
    return hasLoop;
}

void printGraphAdj(vector<Node *> graph) {
    cout << "Graph with adjacency nodes:\n";
    for (int i = 0; i < graph.size(); i++) {
        cout << i << ": ";
        set<Node *>::iterator j;
        for (j = graph[i]->adj.begin(); j != graph[i]->adj.end(); j++) {
            Node *element = *j;
            cout << element->key << " ";
        }
        cout << endl;
    }
}

void topologicalSortRec(Node *node, vector<Node *> &stack) {
    node->color = COLOR_BLACK;
    set<Node *>::iterator j;
    for (j = node->adj.begin(); j != node->adj.end(); j++) {
        Node *element = *j;
        if (element->color != COLOR_BLACK) {
            topologicalSortRec(element, stack);
        }
    }
    stack.push_back(node);
}

void topologicalSort(vector<Node *> graph) {
    cout<<"\nTopological sort:\n";
    if(tarjan(graph,false)){
        cout<<"\nLoop inside, graph -> it's not a DAG ( Can't run Topological Sort )\n";
        return;
    }
    vector<Node *> stack;
    for (auto &i : graph) {
        if (i->color != COLOR_BLACK) {
            topologicalSortRec(i, stack);
        }
    }

    while (!stack.empty()){
        cout<<stack.back()->key<<" ";
        stack.pop_back();
    }
    cout<<endl;
}

void runTests() {
    for (int i = 1000; i <= 5000; i += 100) {
        Operation op = profiler.createOperation("Fixed Nodes - 100", i);
        cout << i << endl;
        vector<Node *> graph = generateConnectedGraph(100, i);
        dfs(graph[rand() % 100], op);
    }
    for (int i = 100; i <= 200; i += 10) {
        Operation op = profiler.createOperation("Fixed Edges - 9000", i);
        cout << i << endl;
        vector<Node *> graph = generateConnectedGraph(i, 9000);
        dfs(graph[rand() % i], op);
    }
    profiler.showReport();
}

vector<Node *> getDAG() {
    int i;
    vector<Node *> graph;
    for (i = 0; i < 8; i++) {
        graph.push_back(newNode(i));
    }
    graph[0]->adj.insert(graph[1]);
    graph[0]->adj.insert(graph[2]);
    graph[1]->adj.insert(graph[2]);
    graph[1]->adj.insert(graph[3]);
    graph[2]->adj.insert(graph[3]);
    graph[2]->adj.insert(graph[5]);
    graph[3]->adj.insert(graph[4]);
    graph[7]->adj.insert(graph[6]);

    return graph;
}

void demo() {
    cout << "\nDemo:\n";
    Operation dummy = profiler.createOperation("dummy", 0);
    vector<Node *> graph = generateConnectedGraph(5, 8);
    vector<Node *> graphRand = generateConnectedGraph(5, 8);
    vector<Node *> graphDAG = getDAG();

    printGraphAdj(graph);

    cout << "\nDepth-First Search from node 0:\n";
    dfs(graph[0], dummy);

    cout << "\nStrongly Connected Components:\n";
    tarjan(graph,true);

    cout<<"\nTopological Sort with a random graph:\n";
    printGraphAdj(graphDAG);
    topologicalSort(graphRand);

    cout<<"\nLet's try with a DAG now\n";
    printGraphAdj(graphDAG);
    topologicalSort(graphDAG);
}

int main() {
    srand(time(nullptr));
    runTests();
    demo();
    return 0;
}