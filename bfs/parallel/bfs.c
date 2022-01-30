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

    // int numOfLevels = (int) log2(endNodeIndex);
    int numOfLevels = 30;
    struct Queue** levels = (struct Queue**) malloc(numOfLevels * sizeof(struct Queue*));
    for(int i = 0; i < numOfLevels; i++){
        levels[i] = malloc(sizeof(struct Queue*));
    }

    omp_lock_t level_locks[numOfLevels];  
    #pragma omp parallel for  
    for(int i = 0; i < numOfLevels; i++){
        omp_init_lock(&level_locks[i]);
    }

    int currentLevel = 0;

    struct State* initialState = (struct State*) malloc(sizeof(struct State));
    initialState->node = startNode;
    initialState->cost = 0;
    initialState->parent = NULL;

    push_back(levels[currentLevel], initialState);
    visited[startNodeIndex] = true;

    struct Queue* path;
    bool cancel = false;

    while(!isEmpty(levels[currentLevel]) && !cancel){
        #pragma omp parallel for firstprivate(startNode, endNode), shared(currentLevel, cancel)
        for(int i = 0; i < levels[currentLevel]->size; i++){

            struct State* currentState;
            #pragma omp critical
            {
                currentState = pop_front(levels[currentLevel]);
            }

            if(currentState->node->value == endNodeIndex){
                #pragma omp critical
                {
                    path = reconstructPath(currentState);
                    cancel = true;
                }
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

                                omp_set_lock(&level_locks[currentLevel+1]);
                                    push_back(levels[currentLevel+1], nextState);
                                omp_unset_lock(&level_locks[currentLevel+1]);

                                visited[edge->dest] = true;
                            }                            
                        }
                        
                        edge = edge->next;
                    }
                }
            }
        }

        currentLevel++;
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