#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "generator.h"

#define ROWS 10
#define COLS 10

struct Cell** cells;

bool checkIfCellInsideBounaries(int i, int j){
    if(i < 0 || i >= ROWS || j < 0 || j >= COLS)
        return false;

    return true;
}

void addNeighbourCells(struct Cell** cells, int cellIndex, struct FullEdge** edges, int* numOfEdges){
    int i = cells[cellIndex]->i;
    int j = cells[cellIndex]->j;

    float moves[8][3] = {{-1, -1, sqrt(2)}, {-1, 0, 1}, {-1, 1, sqrt(2)}, {0, -1, 1}, {0, 1, 1}, {1, -1, sqrt(2)}, {1, 0, 1}, {1, 1, sqrt(2)}};

    if(cells[cellIndex]->special)
        return;

    for(int k = 0; k < 8; k++){
        int new_i = i + moves[k][0];
        int new_j = j + moves[k][1];

        if(checkIfCellInsideBounaries(new_i, new_j) && !cells[new_i * COLS + new_j]->special){
            
            struct FullEdge* edge = (struct FullEdge*) malloc(sizeof(struct FullEdge));
            edge->src = cellIndex;
            edge->dest = new_i * COLS + new_j;
            edge->cost = moves[k][2];

            edges[*numOfEdges] = edge;
            (*numOfEdges)++;
        }
    }
}

float calculateHeuristics(int i, int j){
    return sqrt(pow(ROWS - 1 - i, 2) + pow(COLS - 1 - j, 2));
}

void printCells(struct Queue* queue, struct Queue* visitedQueue){
    for(int i = 0; i < ROWS; i++){
        printf(" ___");
    }
    printf(" \n");

    for(int i = 0; i < ROWS; i++){
        for(int j = 0; j < COLS; j++){
            printf("| ");
            if(cells[i * COLS + j]->special)
                printf("- ");
            else if(checkIfInQueue(queue, i * COLS + j))
                printf("* ");       
            // else if(checkIfInQueue(visitedQueue, i * COLS + j))
            //     printf(". ");    
            else
                printf("  ");

            if( j == COLS - 1)
                printf("|");
        }
        printf("\n");
        for(int j = 0; j < COLS; j++){
            printf(" ‾‾‾");
        }
        printf("\n");
    }

    printf("\n");
}

struct Graph* generateGraph(){
    cells = (struct Cell**) malloc(ROWS * COLS * sizeof(struct Cell*));

    for(int i = 0; i < ROWS; i++){
        for(int j = 0; j < COLS; j++){
            struct Cell* cell = (struct Cell*) malloc(sizeof(struct Cell)); 
            cell->i = i;
            cell->j = j;
            if((i == 2 && j < 7) || ((i == 5 || i == 7) && (j > 0)))
                cell->special = true;
            else
                cell->special = false;
            
            cells[i * COLS + j] = cell;
        }
    }

    struct FullEdge** edges = (struct FullEdge**) malloc(ROWS * COLS * 9 * sizeof(struct FullEdge*));
    int numOfEdges = 0;

    int numOfNodes = ROWS * COLS;

    float* heuristics = (float*) malloc(numOfNodes * sizeof(float));

    for(int i = 0; i < ROWS * COLS; i++){        
        heuristics[i] = calculateHeuristics(cells[i]->i, cells[i]->j);
        addNeighbourCells(cells, i, edges, &numOfEdges);
    }    

    struct FullEdge* finalEdges = (struct FullEdge*) malloc(numOfEdges * sizeof(struct FullEdge));
    for(int i = 0; i < numOfEdges; i++){
        struct FullEdge* edge = (struct FullEdge*) malloc(sizeof(struct FullEdge));
        edge->src = edges[i]->src;
        edge->dest = edges[i]->dest;
        edge->cost = edges[i]->cost;

        finalEdges[i] = *edge;
    }

    return createGraph(finalEdges, numOfNodes, numOfEdges, heuristics);
}