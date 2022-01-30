struct Cell{
    int i, j;
    bool special;
};

struct Graph* generateGraph();
void printCells(struct Queue* queue, struct Queue* visitedQueue);