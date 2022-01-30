#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

void push_back(struct Queue *queue, struct State* state) {
    struct QueueNode *newNode = malloc(sizeof(struct QueueNode));
    if (!newNode) return;

    newNode->state = state; 
    newNode->next = queue->head;

    queue->head = newNode;
    queue->size++;
}

struct State* pop_front(struct Queue *queue) {
    struct QueueNode *current, *prev = NULL;
    struct State* retval;

    if (queue->head == NULL) return NULL;

    current = queue->head;
    while (current->next != NULL) {
        prev = current;
        current = current->next;
    }

    retval = current->state;
    free(current);
    queue->size--;
    
    if (prev)
        prev->next = NULL;
    else
        queue->head = NULL;

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
        printf("%d\n", current->state->node->value);
        current = current->next;
    }
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

//     enqueue(queue, 1);
//     enqueue(queue, 2);
//     enqueue(queue, 3);
//     enqueue(queue, 4);

//     printf("%d", isEmpty(queue));

//     dequeue(queue);
//     dequeue(queue);
//     dequeue(queue);
//     dequeue(queue);

//     printf("%d", isEmpty(queue));
//     return 0;
// }