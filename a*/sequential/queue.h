struct Queue {
    struct QueueNode* head;
    int size;
};

struct QueueNode {
  struct State* state;
  struct QueueNode *next;
};

void push_back(struct Queue *queue, struct State* state);
void pop(struct Queue *queue, struct State* state);
void print_queue(struct Queue *queue);
int isEmpty(struct Queue *queue);
struct Queue* createQueue();
int checkIfInQueue(struct Queue *queue, int cellIndex);