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
    int numOfLevels = (endNode->value+1) / 10;
    struct Queue** levels = (struct Queue**) malloc(numOfLevels * sizeof(struct Queue*));
    for(int i = 0; i < numOfLevels; i++){
        levels[i] = malloc(sizeof(struct Queue*));
        levels[i] = createQueue();
    }

    omp_lock_t** level_locks = (omp_lock_t**) malloc(numOfLevels * sizeof(omp_lock_t*));

    #pragma omp parallel for  
    for(int i = 0; i < numOfLevels; i++){
        level_locks[i] = malloc(sizeof(omp_lock_t*));
        omp_init_lock(level_locks[i]);
    }

    int currentLevel = 0;

    struct State* initialState = (struct State*) malloc(sizeof(struct State));
    initialState->node = startNode;
    initialState->cost = 0;
    initialState->parent = NULL;

    // struct Queue* queue = createQueue();

    push_back(levels[0], initialState);
    // push_back(queue, initialState);
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
                push_back(visitedQueue, currentState);
            }

            // int sum = 0;
            // for(int i = 0; i < 50000000; i++){
            //     sum++;
            // }

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

                                #pragma omp critical
                                // push_back(queue, nextState);
                                omp_set_lock(level_locks[currentLevel+1]);
                                    push_back(levels[currentLevel+1], nextState);
                                omp_unset_lock(level_locks[currentLevel+1]);

                                visited[edge->dest] = true;
                            }                            
                        }
                        
                        edge = edge->next;
                    }
                }
                #pragma omp barrier
            }

        }

        currentLevel++;
        if(currentLevel >= numOfLevels - 1){
            levels = realloc(levels, (numOfLevels+1)*sizeof(struct Queue*));
            level_locks = realloc(level_locks, (numOfLevels+1)*sizeof(struct omp_lock_t*));

            levels[numOfLevels] = malloc(sizeof(struct Queue*));
            levels[numOfLevels] = createQueue();
            level_locks[numOfLevels] = malloc(sizeof(omp_lock_t*));
            omp_init_lock(level_locks[numOfLevels]);
            numOfLevels++;
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
    struct Queue* path = bfs(graph, visitedQueue, 0, 999);
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