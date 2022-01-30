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
    int numOfNodes = graph->numOfNodes;

    struct Node* startNode = graph->head[startNodeIndex];
    struct Node* endNode = graph->head[endNodeIndex];
    
    bool visited[numOfNodes];    
    #pragma omp parallel for firstprivate(numOfNodes)
    for(int i = 0; i < numOfNodes; i++){
        visited[i] = false;
    }

    int currentLevel = 0;

    struct State* initialState = (struct State*) malloc(sizeof(struct State));
    initialState->node = startNode;
    initialState->cost = 0;
    initialState->parent = NULL;

    struct Queue* queue = createQueue();

    push_back(queue, initialState);
    visited[startNodeIndex] = true;

    struct Queue* path;
    bool cancel = false;

    while(!isEmpty(queue) && !cancel){

        struct State* currentState;
        currentState = pop_front(queue);
        push_back(visitedQueue, currentState);

        // int sum = 0;
        // for(int i = 0; i < 50000000; i++){
        //     sum++;
        // }

        if(currentState->node->value == endNodeIndex){
            path = reconstructPath(currentState);
            cancel = true;
        }

        struct Edge* edge = currentState->node->nextEdge;
        #pragma omp parallel
        {
            #pragma omp single
            {
                while (edge != NULL)
                {
                    #pragma omp task
                    {
                        if(!visited[edge->dest]){
                            struct State* nextState = (struct State*) malloc(sizeof(struct State));
                            nextState->node = graph->head[edge->dest];
                            nextState->cost = 0;
                            nextState->parent = currentState;

                            #pragma omp critical
                            push_back(queue, nextState);

                            visited[edge->dest] = true;
                        }                            
                    }
                    
                    edge = edge->next;
                }
            }
            #pragma omp barrier
        }
    }

    if(cancel) 
        return path;
    else 
        return NULL;
}

int main(){
    struct Graph* graph = generateGraph();
    struct Queue* visitedQueue = createQueue();

    double start = omp_get_wtime();
    struct Queue* path = bfs(graph, visitedQueue, 0, 899);
    double end = omp_get_wtime();

    printf("Time elapsed: %.2lfs\n", end - start);
    // printGraph(graph);
    // print_queue(path);
    // print_queue(visitedQueue);
    printCells(path, visitedQueue);

    if(path == NULL)
        printf("No path!\n");
 
    return 0;
}