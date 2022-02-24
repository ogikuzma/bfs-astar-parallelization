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

struct Queue* aStar(struct Graph* graph, struct Queue* visitedQueue, int startNodeValue, int endNodeValue){
    struct Queue *queue = createQueue();

    struct Node* startNode = graph->head[startNodeValue];
    struct Node* endNode = graph->head[endNodeValue];

    struct State* initialState = (struct State*) malloc(sizeof(struct State));
    initialState->node = startNode;
    initialState->cost = 0;
    initialState->parent = NULL;

    bool visited[graph->numOfNodes];
    #pragma omp parallel for
    for(int i = 0; i < graph->numOfNodes; i++){
        visited[i] = false;
    } 

    push_back(queue, initialState);

    bool found = false;
    struct Queue* path;

    while(!isEmpty(queue) && !found){
        struct State* bestState = pop_front(queue);

        // int sum = 0;
        // for(int i = 0; i < 50000000; i++){
        //     sum++;
        // }
        
        push_back(visitedQueue, bestState);
        
        struct Node* currentNode = bestState->node;

        if(currentNode == endNode){
            path = reconstructPath(bestState);
            found = true;
        }

        if(visited[bestState->node->value])
            continue;
        else
            visited[bestState->node->value] = true;

        struct Edge* edge = currentNode->nextEdge;
        #pragma omp parallel
        {
            #pragma omp single
            {
                while(edge != NULL){
                    #pragma omp task shared(queue)
                    {
                        struct State* possibleState = (struct State*) malloc(sizeof(struct State));
                        possibleState->node = graph->head[edge->dest];
                        possibleState->cost = bestState->cost + edge->cost;
                        possibleState->parent = bestState;

                        #pragma omp critical
                        {
                            push_back(queue, possibleState);
                        }
                    }

                    edge = edge->next;
                } 
            }
            #pragma omp barrier
        }        
    }

    if(!found) return NULL;
    else return path;
}

int main(){
    struct Graph* graph = generateGraph();
    struct Queue* visitedQueue = createQueue();
    // printGraph(graph);

    double start = omp_get_wtime();
    struct Queue* path = aStar(graph, visitedQueue, 0, 99);
    double end = omp_get_wtime();

    printf("Time elapsed: %.2lfs\n", end - start);
    // print_queue(path);
    printCells(path, visitedQueue);

    if(path == NULL)
        printf("No path!\n");
 
    return 0;
}