#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "queue.h"

void push_back(struct Queue *queue, struct State* state) {
    struct QueueNode *newNode = malloc(sizeof(struct QueueNode));
    if (!newNode) return;

    newNode->state = state; 

    if(queue->head == NULL){
        newNode->next = NULL; 
        queue->head = newNode;
        queue->size++;
        return;
    }

    struct QueueNode* current = queue->head;
    struct QueueNode* prev = NULL;
    bool inserted = false;
    while(current != NULL){
        float fNew = state->cost + state->node->heuristic;
        float f = current->state->cost + current->state->node->heuristic;
        if(f >= fNew){
            if(prev == NULL){
                newNode->next = current;
                queue->head = newNode;
            }else{
                prev->next = newNode;
                newNode->next = current;
            }

            inserted = true;
            break;
        }
        prev = current;
        current = current->next;
    }

    if(!inserted){
        newNode->next = NULL;
        prev->next = newNode;
    }

    queue->size++;
}

struct State* pop_front(struct Queue *queue) {
    struct QueueNode *current, *prev = NULL;
    struct State* retval;

    if (queue->head == NULL) return NULL;

    retval = queue->head->state;
    queue->head = queue->head->next;

    queue->size--;

    return retval;
}

int checkIfInQueue(struct Queue *queue, int cellIndex){
    if(queue == NULL)
        return 0;
    struct QueueNode *current = queue->head;

    while (current != NULL) {
        if(current->state->node->value == cellIndex)
            return 1;
        current = current->next;
    }

    return 0;
}

void print_queue(struct Queue *queue) {
    struct QueueNode *current = queue->head;

    while (current != NULL) {
        printf("%d ", current->state->node->value);
        current = current->next;
    }
    printf("\n");
}

int isEmpty(struct Queue *queue){
    if(queue->head == NULL){
        return 1;
    }else return 0;
}

struct Queue* createQueue(){
    struct Queue* queue = (struct Queue*) malloc(sizeof(struct Queue));
    queue->head = NULL;
    queue->size = 0;

    return queue;
}

// int main(){
//     struct Queue* queue = createQueue();

//     print_queue(queue);

//     return 0;
// }