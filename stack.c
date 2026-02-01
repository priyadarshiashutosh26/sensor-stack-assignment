#include <stdio.h>
#include <stdlib.h>

#define MAX_STACK_SIZE 32

/* Error Codes */
#define ERR_I2C_TIMEOUT        1
#define ERR_SENSOR_DISCONNECT  2
#define ERR_INVALID_DATA       3

/* Stack Node */
typedef struct Node {
    int error_code;
    struct Node *next;
} Node;

/* Stack Control */
typedef struct {
    Node *top;
    Node *bottom;
    int size;
} Stack;

/* Initialize stack */
void initStack(Stack *s) {
    s->top = NULL;
    s->bottom = NULL;
    s->size = 0;
}

/* Push with circular overwrite */
void push(Stack *s, int error_code) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (!newNode) return;

    newNode->error_code = error_code;
    newNode->next = s->top;
    s->top = newNode;

    if (s->size == 0)
        s->bottom = newNode;

    if (s->size < MAX_STACK_SIZE) {
        s->size++;
    } else {
        /* Remove oldest (bottom) */
        Node *temp = s->top;
        while (temp->next != s->bottom)
            temp = temp->next;

        free(s->bottom);
        s->bottom = temp;
        s->bottom->next = NULL;
    }
}

/* Pop operation */
int pop(Stack *s) {
    if (s->size == 0)
        return -1;

    Node *temp = s->top;
    int code = temp->error_code;

    s->top = temp->next;
    free(temp);
    s->size--;

    if (s->size == 0)
        s->bottom = NULL;

    return code;
}

/* Display stack */
void printStack(Stack *s) {
    Node *temp = s->top;
    printf("Stack (Top -> Bottom): ");
    while (temp) {
        printf("%d ", temp->error_code);
        temp = temp->next;
    }
    printf("\n");
}

/* Example usage */
int main() {
    Stack errorStack;
    initStack(&errorStack);

    /* Simulated sensor errors */
    push(&errorStack, ERR_I2C_TIMEOUT);
    push(&errorStack, ERR_INVALID_DATA);
    push(&errorStack, ERR_SENSOR_DISCONNECT);

    printStack(&errorStack);

    printf("Popped error: %d\n", pop(&errorStack));
    printStack(&errorStack);

    return 0;
}
