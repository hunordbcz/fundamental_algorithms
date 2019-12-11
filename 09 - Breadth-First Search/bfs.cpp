#include <stdlib.h>
#include <string.h>
#include "bfs.h"
#include <set>
#include <queue>
using namespace std;

typedef struct _NodeMulti {
    int key;
    queue<struct _NodeMulti*> children;
} multiWay;

int get_neighbors(const Grid *grid, Point p, Point neighb[])
{
    // TODO: fill the array neighb with the neighbors of the point p and return the number of neighbors
    // the point p will have at most 4 neighbors (up, down, left, right)
    // avoid the neighbors that are outside the grid limits or fall into a wall
    // note: the size of the array neighb is guaranteed to be at least 4
    int nrNeighb = 0;
    if (grid->mat[p.row - 1][p.col] == 0) {
        nrNeighb++;
        Point neighbNew = { p.row - 1,p.col };
        neighb[nrNeighb - 1] = neighbNew;
    }
    if (grid->mat[p.row][p.col - 1] == 0) {
        nrNeighb++;
        Point neighbNew = { p.row,p.col - 1 };
        neighb[nrNeighb - 1] = neighbNew;
    }
    if (grid->mat[p.row + 1][p.col] == 0) {
        nrNeighb++;
        Point neighbNew = { p.row + 1,p.col };
        neighb[nrNeighb - 1] = neighbNew;
    }
    if (grid->mat[p.row][p.col + 1] == 0) {
        nrNeighb++;
        Point neighbNew = { p.row,p.col + 1 };
        neighb[nrNeighb - 1] = neighbNew;
    }
    return nrNeighb;
}

int get_neighbors_knight(const Grid* grid, Point p, Point neighb[]) {
    int nrNeighb = 0;
    int dir[] = { -2,-1,1,2 };
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (p.row + dir[i] >= 0 && p.row + dir[i] < grid->rows &&
                p.col + dir[j] >= 0 && p.col + dir[j] < grid->cols &&
                abs(dir[i]) != abs(dir[j]) &&
                grid->mat[p.row + dir[i]][p.col + dir[j]] == 0) {
                    nrNeighb++;
                    Point neighbNew = { p.row + dir[i],p.col + dir[j] };
                    neighb[nrNeighb - 1] = neighbNew;
            }
        }
    }
    return nrNeighb;
}

void grid_to_graph(const Grid *grid, Graph *graph, boolean knight)
{
    //we need to keep the nodes in a matrix, so we can easily refer to a position in the grid
    Node *nodes[MAX_ROWS][MAX_COLS];
    int i, j, k;
    Point neighb[12];

    //compute how many nodes we have and allocate each node
    graph->nrNodes = 0;
    for(i=0; i<grid->rows; ++i){
        for(j=0; j<grid->cols; ++j){
            if(grid->mat[i][j] == 0){
                nodes[i][j] = (Node*)malloc(sizeof(Node));
                memset(nodes[i][j], 0, sizeof(Node)); //initialize all fields with 0/NULL
                nodes[i][j]->position.row = i;
                nodes[i][j]->position.col = j;
                ++graph->nrNodes;
            }else{
                nodes[i][j] = NULL;
            }
        }
    }
    graph->v = (Node**)malloc(graph->nrNodes * sizeof(Node*));
    k = 0;
    for(i=0; i<grid->rows; ++i){
        for(j=0; j<grid->cols; ++j){
            if(nodes[i][j] != NULL){
                graph->v[k++] = nodes[i][j];
            }
        }
    }

    //compute the adjacency list for each node
    for(i=0; i<graph->nrNodes; ++i){
        if (knight) {
            graph->v[i]->adjSize = get_neighbors_knight(grid, graph->v[i]->position, neighb);
        }
        else {
            graph->v[i]->adjSize = get_neighbors(grid, graph->v[i]->position, neighb);
        }
        
        if(graph->v[i]->adjSize != 0){
            graph->v[i]->adj = (Node**)malloc(graph->v[i]->adjSize * sizeof(Node*));
            k = 0;
            for(j=0; j<graph->v[i]->adjSize; ++j){
                if( neighb[j].row >= 0 && neighb[j].row < grid->rows &&
                    neighb[j].col >= 0 && neighb[j].col < grid->cols &&
                    grid->mat[neighb[j].row][neighb[j].col] == 0){
                        graph->v[i]->adj[k++] = nodes[neighb[j].row][neighb[j].col];
                }
            }
            if(k < graph->v[i]->adjSize){
                //get_neighbors returned some invalid neighbors
                graph->v[i]->adjSize = k;
                graph->v[i]->adj = (Node**)realloc(graph->v[i]->adj, k * sizeof(Node*));
            }
        }
    }
}

void free_graph(Graph *graph)
{
    if(graph->v != NULL){
        for(int i=0; i<graph->nrNodes; ++i){
            if(graph->v[i] != NULL){
                if(graph->v[i]->adj != NULL){
                    free(graph->v[i]->adj);
                    graph->v[i]->adj = NULL;
                }
                graph->v[i]->adjSize = 0;
                free(graph->v[i]);
                graph->v[i] = NULL;
            }
        }
        free(graph->v);
        graph->v = NULL;
    }
    graph->nrNodes = 0;
}

void bfs(Graph *graph, Node *s, Operation *op)
{
    // TOOD: implement the BFS algorithm on the graph, starting from the node s
    // at the end of the algorithm, every node reachable from s should have the color BLACK
    // for all the visited nodes, the minimum distance from s (dist) and the parent in the BFS tree should be set
    // for counting the number of operations, the optional op parameter is received
    // since op can be NULL (when we are calling the bfs for display purposes), you should check it before counting:
    // if(op != NULL) op->count();
    vector<Node*> queue;
    int first = 0;
    s->color = COLOR_BLACK;
    if (op != NULL) op->count();
    queue.push_back(s);
    if (op != NULL) op->count();
    while (first != queue.size()) {
        if (op != NULL) op->count();
        s = queue.at(first);
        if (op != NULL) op->count();
        first++;
        for (int i = 0; i < s->adjSize; i++) {
            if (op != NULL) op->count();
            if (s->adj[i]->color == COLOR_WHITE) {
                s->adj[i]->color = COLOR_BLACK;
                s->adj[i]->dist = s->dist + 1;
                s->adj[i]->parent = s;
                if (op != NULL) op->count(3);
                queue.push_back(s->adj[i]);
                if (op != NULL) op->count();
            }
        }
        
    }
    if (op != NULL) op->count();
}

void rec_print_bfs_tree(Point* repr, multiWay* root, queue<multiWay*> siblings, int level) {
    if (root == nullptr) return;
    for (int i = 0; i < level; i++) {
        printf("   ");
    }
    printf("(%d,%d)\n", repr[root->key].row, repr[root->key].col);
    //cout << root->key << "\n";
    if (!root->children.empty()) {
        queue< multiWay* > children = root->children;
        multiWay* firstChild = children.front();
        children.pop();
        rec_print_bfs_tree(repr, firstChild, children, level + 1);
    }
    if (!siblings.empty()) {
        multiWay* firstSibling = siblings.front();
        siblings.pop();
        rec_print_bfs_tree(repr, firstSibling, siblings, level);
    }
}

void print_bfs_tree(Graph *graph)
{
    //first, we will represent the BFS tree as a parent array
    int n = 0; //the number of nodes
    int *p = NULL; //the parent array
    Point *repr = NULL; //the representation for each element in p

    //some of the nodes in graph->v may not have been reached by BFS
    //p and repr will contain only the reachable nodes
    int *transf = (int*)malloc(graph->nrNodes * sizeof(int));
    for(int i=0; i<graph->nrNodes; ++i){
        if(graph->v[i]->color == COLOR_BLACK){
            transf[i] = n;
            ++n;
        }else{
            transf[i] = -1;
        }
    }
    if(n == 0){
        //no BFS tree
        free(transf);
        return;
    }

    int err = 0;
    p = (int*)malloc(n * sizeof(int));
    repr = (Point*)malloc(n * sizeof(Node));
    for(int i=0; i<graph->nrNodes && !err; ++i){
        if(graph->v[i]->color == COLOR_BLACK){
            if(transf[i] < 0 || transf[i] >= n){
                err = 1;
            }else{
                repr[transf[i]] = graph->v[i]->position;
                if(graph->v[i]->parent == NULL){
                    p[transf[i]] = -1;
                }else{
                    err = 1;
                    for(int j=0; j<graph->nrNodes; ++j){
                        if(graph->v[i]->parent == graph->v[j]){
                            if(transf[j] >= 0 && transf[j] < n){
                                p[transf[i]] = transf[j];
                                err = 0;
                            }
                            break;
                        }
                    }
                }
            }
        }
    }
    free(transf);
    transf = NULL;

    if(!err){
        // TODO: pretty print the BFS tree
        // the parrent array is p (p[k] is the parent for node k or -1 if k is the root)
        // when printing the node k, print repr[k] (it contains the row and column for that point)
        // you can adapt the code for transforming and printing multi-way trees from the previous labs
        vector < multiWay* > tree;
        tree.reserve(n);
        for (int i = 0; i < n; i++) {
            auto* node = new multiWay;
            node->key = i;
            tree.push_back(node);
        }
        int root = -1;
        for (int i = 1; i < n; i++) {
            if (p[i] != -1) {
                tree[p[i]]->children.push(tree[i]);
            }
            else {
                root = i;
            }
        }
        rec_print_bfs_tree(repr, tree[root], {}, 0);
    }

    if(p != NULL){
        free(p);
        p = NULL;
    }
    if(repr != NULL){
        free(repr);
        repr = NULL;
    }
}

int shortest_path(Graph *graph, Node *start, Node *end, Node *path[])
{
    // TODO: compute the shortest path between the nodes start and end in the given graph
    // the nodes from the path, should be filled, in order, in the array path
    // the number of nodes filled in the path array should be returned
    // if end is not reachable from start, return -1
    // note: the size of the array path is guaranteed to be at least 1000
    bfs(graph, end, NULL);
    if (!start->dist) {
        return -1;
    }
    Node* current = start;
    while (current->parent != NULL) {
        path[start->dist - current->dist] = current;
        current = current->parent;
    }
    return start->dist;
}


void performance()
{
    int n, i, j;
    Profiler p("bfs");
    srand(time(NULL));

    // vary the number of edges
    for(n=1000; n<=4500; n+=100){
        Operation op = p.createOperation("bfs-edges", n);
        Graph graph;
        graph.nrNodes = 100;
        //initialize the nodes of the graph
        graph.v = (Node**)malloc(graph.nrNodes * sizeof(Node*));
        for(i=0; i<graph.nrNodes; ++i){
            graph.v[i] = (Node*)malloc(sizeof(Node));
            memset(graph.v[i], 0, sizeof(Node));
        }
        // TODO: generate n random edges
        // make sure the generated graph is connected
        vector<set<int>> adj(graph.nrNodes);
        for (i = 1; i < graph.nrNodes; i++) {
            int a = rand() % i, b = i;
            adj[a].insert(b);
            adj[b].insert(a);
        }
        while (i <= n) {
            int a = rand() % graph.nrNodes, b = rand() % graph.nrNodes;
            /*
                Check if the random values are different and are not inserted already
            */
            if (a != b && !adj[a].count(b) && !adj[b].count(a)){
                adj[a].insert(b);
                adj[b].insert(a);
                i++;
            }
        }
        for (i = 0; i < graph.nrNodes; i++) {
            graph.v[i]->adj = (Node**)malloc(adj[i].size() * sizeof(Node*));
            set<int>::iterator it = adj[i].begin();
            while (it != adj[i].end()) {
                graph.v[i]->adj[graph.v[i]->adjSize++] = graph.v[*it];
                it++;
            }
        }

        bfs(&graph, graph.v[0], &op);
        free_graph(&graph);
    }

    // vary the number of vertices
    for(n=100; n<=200; n+=10){
        Operation op = p.createOperation("bfs-vertices", n);
        Graph graph;
        graph.nrNodes = n;
        //initialize the nodes of the graph
        graph.v = (Node**)malloc(graph.nrNodes * sizeof(Node*));
        for(i=0; i<graph.nrNodes; ++i){
            graph.v[i] = (Node*)malloc(sizeof(Node));
            memset(graph.v[i], 0, sizeof(Node));
        }
        // TODO: generate 4500 random edges
        // make sure the generated graph is connected

        vector<set<int>> adj(graph.nrNodes);
        for (i = 1; i < graph.nrNodes; i++) {
            int a = rand() % i, b = i;
            adj[a].insert(b);
            adj[b].insert(a);
        }
        while (i <= 4500) {
            int a = rand() % graph.nrNodes, b = rand() % graph.nrNodes;
            /*
                Check if the random values are different and are not inserted already
            */
            if (a != b && !adj[a].count(b) && !adj[b].count(a)) {
                adj[a].insert(b);
                adj[b].insert(a);
                i++;
            }
        }
        for (i = 0; i < graph.nrNodes; i++) {
            graph.v[i]->adj = (Node**)malloc(adj[i].size() * sizeof(Node*));
            set<int>::iterator it = adj[i].begin();
            while (it != adj[i].end()) {
                graph.v[i]->adj[graph.v[i]->adjSize++] = graph.v[*it];
                it++;
            }
        }

        bfs(&graph, graph.v[0], &op);
        free_graph(&graph);
    }

    p.showReport();
}
