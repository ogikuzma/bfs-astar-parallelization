#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <omp.h>

#include "graph.c"
#include "queue.c"
#include "generator.c"

struct Queue* reconstructPath(struct State* bestState){
    struct Queue* path = createQueue();
    while(bestState->parent != NULL) {
        push_back(path, bestState);
        bestState = bestState->parent;
    }
    push_back(path, bestState);

    return path;
}

struct Queue* bfs(struct Graph* graph, struct Queue* visitedQueue, int startNodeIndex, int endNodeIndex){
    struct Queue *queue = createQueue();

    struct Node* startNode = graph->head[startNodeIndex];
    struct Node* endNode = graph->head[endNodeIndex];
    
    bool visited[graph->numOfNodes];
    for(int i = 0; i < graph->numOfNodes; i++){
        visited[i] = false;
    }

    struct State* initialState = (struct State*) malloc(sizeof(struct State));
    initialState->node = startNode;
    initialState->cost = 0;
    initialState->parent = NULL;

    push_back(queue, initialState);

    while(!isEmpty(queue)){
        struct State* currentState = pop_front(queue);
        push_back(visitedQueue, currentState);

        if(currentState->node->value == endNode->value){
            return reconstructPath(currentState);
        }

        if(visited[currentState->node->value])
            continue;
        else
            visited[currentState->node->value] = true;

        struct Edge* edge = currentState->node->nextEdge;
        while (edge != NULL)
        {                
            struct State* nextState = (struct State*) malloc(sizeof(struct State));
            nextState->node = graph->head[edge->dest];
            nextState->cost = 0;
            nextState->parent = currentState;

            push_back(queue, nextState);
            edge = edge->next;
        }
    }

    return NULL;
}

int main(){
    struct Graph* graph = generateGraph();
    struct Queue* visitedQueue = createQueue();

    double start = omp_get_wtime();
    struct Queue* path = bfs(graph, visitedQueue, 0, 99);
    double end = omp_get_wtime();

    printf("Time elapsed: %.2lfs\n", end - start);
    // printGraph(graph);
    // print_queue(path);
    printCells(path, visitedQueue);

    if(path == NULL)
        printf("No path!\n");
 
    return 0;
}