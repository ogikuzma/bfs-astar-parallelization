struct Graph{
    int numOfNodes;
    struct Node** head;
};

struct Node{
    int value;
    float heuristic;
    struct Edge* nextEdge;
};

struct Edge{
    int dest;
    float cost;
    struct Edge* next;
};

struct FullEdge{
    int src, dest;
    float cost;
};

struct State{
    struct Node* node;
    struct State* parent;
    float cost;
};

struct Graph* createGraph(struct FullEdge edges[], int numOfNodes, int numOfEdges, float heuristics[]);
void printGraph(struct Graph* graph);