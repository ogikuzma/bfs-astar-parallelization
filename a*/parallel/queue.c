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

void pop(struct Queue *queue, struct State* state) {
    struct QueueNode *current = queue->head;
    struct QueueNode *prev = NULL;

    if(current->next == NULL){
        if(current->state == state){
            queue->head = NULL;
            free(current);

            queue->size--;
            return;
        }

        return;
    }

    while(current != NULL){
        if(current->state == state){
            if(prev == NULL)
                queue->head = current->next;
            else
                prev->next = current->next;
                
            free(current);

            queue->size--;
            return;
        }
        prev = current;
        current = current->next;
    }
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