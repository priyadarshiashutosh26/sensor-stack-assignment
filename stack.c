#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STACK_SIZE 32

#define ERR_I2C_TIMEOUT        0xE1
#define ERR_SENSOR_DISCONNECT  0xE2
#define ERR_INVALID_DATA       0xE3

typedef struct Node {
    int error_code;
    struct Node *next;
} Node;

typedef struct {
    Node *top;
    Node *bottom;
    int size;
} Stack;

/* Stack functions */
void initStack(Stack *s) {
    s->top = s->bottom = NULL;
    s->size = 0;
}

void push(Stack *s, int code) {
    Node *n = malloc(sizeof(Node));
    if (!n) return;

    n->error_code = code;
    n->next = s->top;
    s->top = n;

    if (s->size == 0)
        s->bottom = n;

    if (s->size < MAX_STACK_SIZE) {
        s->size++;
    } else {
        Node *t = s->top;
        while (t->next != s->bottom)
            t = t->next;

        free(s->bottom);
        s->bottom = t;
        s->bottom->next = NULL;
    }
}

void printStack(Stack *s) {
    Node *t = s->top;
    printf("ERROR STACK (newest → oldest): ");
    while (t) {
        printf("0x%X ", t->error_code);
        t = t->next;
    }
    printf("\n");
}

/* Error parser */
int parse_error(const char *line) {
    if (strstr(line, "I2C timeout"))
        return ERR_I2C_TIMEOUT;
    if (strstr(line, "Sensor disconnected"))
        return ERR_SENSOR_DISCONNECT;
    if (strstr(line, "Invalid sensor data"))
        return ERR_INVALID_DATA;

    return 0;
}

int main() {
    FILE *pipe;
    char line[256];
    Stack errorStack;

    initStack(&errorStack);

    /* Run Python unbuffered */
    pipe = popen("python -u sensor.py", "r");
    if (!pipe) {
        perror("popen failed");
        return 1;
    }

    printf("Listening to sensor errors...\n");

    /* Continuous read */
    while (1) {
        if (fgets(line, sizeof(line), pipe)) {
            int err = parse_error(line);
            if (err) {
                push(&errorStack, err);
                printStack(&errorStack);
            }
        }
    }

    pclose(pipe);
    return 0;
}
