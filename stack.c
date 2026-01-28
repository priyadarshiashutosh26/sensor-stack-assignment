#include <stdio.h>
#include <stdlib.h>

#define MAX_STACK_SIZE 32

typedef struct StackNode {
    int error_code;
    struct StackNode *next;
} StackNode;

typedef struct {
    StackNode *top;
    StackNode *bottom;
    int size;
} Stack;

/* Initialize stack */
void stack_init(Stack *stack) {
    stack->top = NULL;
    stack->bottom = NULL;
    stack->size = 0;
}

/* Push with circular overwrite */
void stack_push(Stack *stack, int error_code) {
    StackNode *node = malloc(sizeof(StackNode));
    if (!node) return;

    node->error_code = error_code;
    node->next = stack->top;
    stack->top = node;

    if (stack->size == 0)
        stack->bottom = node;

    if (stack->size < MAX_STACK_SIZE) {
        stack->size++;
    } else {
        StackNode *current = stack->top;
        while (current->next != stack->bottom)
            current = current->next;

        free(stack->bottom);
        stack->bottom = current;
        stack->bottom->next = NULL;
    }
}

/* Pop operation */
int stack_pop(Stack *stack, int *error_code) {
    if (stack->size == 0)
        return -1;

    StackNode *temp = stack->top;
    *error_code = temp->error_code;

    stack->top = temp->next;
    free(temp);
    stack->size--;

    if (stack->size == 0)
        stack->bottom = NULL;

    return 0;
}

/* Print stack */
void stack_print(const Stack *stack) {
    StackNode *current = stack->top;
    printf("\nStack (latest → oldest):\n");

    while (current) {
        printf("Error Code: %d\n", current->error_code);
        current = current->next;
    }
}

/* Load error codes from Python output */
void load_errors_from_file(Stack *stack, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error opening error code file\n");
        return;
    }

    int code;
    while (fscanf(file, "%d", &code) == 1) {
        stack_push(stack, code);
    }

    fclose(file);
}

/* ---------------- Main ---------------- */
int main(void) {
    Stack error_stack;
    stack_init(&error_stack);

    load_errors_from_file(&error_stack, "error_codes.txt");
    stack_print(&error_stack);

    return 0;
}
