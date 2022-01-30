#include <stdio.h>
#include <stdlib.h>
#include "graph.h"

struct Graph* createGraph(struct FullEdge edges[], int numOfNodes, int numOfEdges, float heuristics[]){

    struct Graph* graph = (struct Graph*) malloc(sizeof(struct Graph));
    graph->numOfNodes = numOfNodes;
    graph->head = (struct Node**) malloc(numOfNodes * sizeof(struct Node*));

    for(int i = 0; i < numOfNodes; i++){
        struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
        newNode->value = i;
        newNode->heuristic = heuristics[i];
        newNode->nextEdge = NULL;

        graph->head[i] = newNode;
    }

    for(int i = 0; i < numOfEdges; i++){
        int src = edges[i].src;
        int dest = edges[i].dest;
        float cost = edges[i].cost;

        struct Edge* newEdge = (struct Edge*)malloc(sizeof(struct Edge));
        newEdge->dest = dest;
        newEdge->cost = cost;

        newEdge->next = graph->head[src]->nextEdge;
        graph->head[src]->nextEdge = newEdge;
    }

    return graph;
}

void printGraph(struct Graph* graph)
{
    for (int i = 0; i < graph->numOfNodes; i++){
        struct Node* ptr = graph->head[i];
        printf("%d(h=%f)  |  ", i, ptr->heuristic);
        struct Edge* edge = ptr->nextEdge;

        while (edge != NULL)
        {
            printf("%d —> %d (%f)\t", i, edge->dest, edge->cost);
            edge = edge->next;
        }

        printf("\n");
    }
}