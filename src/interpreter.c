#include "../include/interpreter.h"
#include <stdio.h>

static void cancel_int(int sig) { (void) sig; }

int interpreter(unsigned char *stack, int stack_size)
{
    int p = 0;
    char buff[BUFF_SIZE];
    ssize_t bytes_read = 0;

    printf("BrainFuck Interpretor V0.0.1 Stack size:%d\n(Press CTRL+D to quit, CTRL+C won't work)\n> ", stack_size);
    fflush(0);
    bytes_read = read(0, buff, BUFF_SIZE);
    while (bytes_read > 0) {
        for (int i = 0; i < bytes_read - 1; i++) {
            switch (buff[i]) {
                case '+':
                    stack[p] += 1;
                    break;
                case '-':
                    stack[p] -= 1;
                    break;
                case '>':
                    if (p < stack_size - 1)
                        p++;
                    break;
                case '<':
                    if (p > 0)
                        p--;
                    break;
                case '.':
                    printf("%c", stack[p]);
                    break;
                case ',':
                    break;
                case '[':
                    //TODO: IMPLEMENT IT
                    break;
                case ']':
                    //TODO: IMPLEMENT IT
                    break;
            }
        }
        printf("\n> ");
        fflush(0);
        bytes_read = read(0, buff, BUFF_SIZE);
    }
    return 0;
}

unsigned char *init_stack(int size)
{
    unsigned char *stack;

    stack = calloc(size, sizeof(unsigned char));
    if (!stack)
        return NULL;
    return stack;
}

int main(int ac, char **av)
{
    int stack_size = 0;
    unsigned char *stack;

    signal(SIGINT, cancel_int);
    if (ac > 1) {
        if (strcmp(av[1], "-h") == 0) {
            printf("Usage:\n  -h:\t\tprint this page\n  <number>:\tsize of the stack (if not privided, default value is 30 000)\n");
            return 0;
        } else {
            stack_size = atoi(av[1]);
        }
    }
    if (stack_size <= 0)
        stack_size = STACK_SIZE;
    if (!(stack = init_stack(stack_size))) return 1;
    interpreter(stack, stack_size);
    free(stack);
    return 0;
}
