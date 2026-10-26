#include "../include/interpreter.h"

static void cancel_int(int sig) { (void) sig; }
static void flush_stdin(void) { int c; while ((c = getchar()) != '\n' && c != EOF); }

size_t *parse_bracket_map(char *buff, int buff_size)
{
    size_t *bracket_map = malloc(buff_size * sizeof(size_t));
    if (!bracket_map) return NULL;

    size_t *bracket_stack = malloc(buff_size * sizeof(size_t));
    if (!bracket_stack) { free(bracket_map); return NULL; }

    int stack_position = 0;

    for (int i = 0; i < buff_size; i++) {
        if (buff[i] == '[') {
            bracket_stack[stack_position] = i;
            stack_position++;
        } else if (buff[i] == ']') {
            if (stack_position == 0) {
                dprintf(2, "[ERROR]: Unbalanced brackets (unexpected ']')\n");
                free(bracket_map);
                free(bracket_stack);
                return NULL;
            }
            stack_position--;
            size_t open_i = bracket_stack[stack_position];
            bracket_map[i] = open_i;
            bracket_map[open_i] = i;
        }
    }
    if (stack_position != 0) {
        dprintf(2, "[ERROR]: Unbalanced brackets (missing ']')\n");
        free(bracket_map);
        free(bracket_stack);
        return NULL;
    }
    free(bracket_stack);
    return bracket_map;
}

int interpreter(unsigned char *stack, int stack_size)
{
    int p = 0;
    char buff[BUFF_SIZE];
    ssize_t bytes_read = 0;
    size_t *bracket_map;

    dprintf(1, "BrainFuck Interpretor V0.1 Stack of size: %d\n(Press CTRL+D to quit, CTRL+C won't work)\n> ", stack_size);
    bytes_read = read(0, buff, BUFF_SIZE);
    while (bytes_read > 0) {
        if ((bracket_map = parse_bracket_map(buff, bytes_read)) == NULL) {
            dprintf(2, "[ERROR]: Parsing failed, restart the interpreter\n");
            return 1;
        }
        for (int i = 0; i < bytes_read - 1;) {
            switch (buff[i]) {
                case '+':
                    stack[p] += 1;
                    i++;
                    break;
                case '-':
                    stack[p] -= 1;
                    i++;
                    break;
                case '>':
                    if (p < stack_size - 1)
                        p++;
                    i++;
                    break;
                case '<':
                    if (p > 0)
                        p--;
                    i++;
                    break;
                case '.':
                    dprintf(1, "%c", stack[p]);
                    i++;
                    break;
                case ',':
                    ssize_t in_read = read(0, &stack[p], 1);
                    if (in_read <= 0)
                        stack[p] = 0;
                    else
                        flush_stdin();
                    i++;
                    break;
                case '[':
                    if (stack[p] == 0)
                        i = bracket_map[i] + 1;
                    else
                        i++;
                    break;
                case ']':
                    if (stack[p] != 0)
                        i = bracket_map[i] + 1;
                    else
                        i++;
                    break;
                default:
                    i++;
                    break;
            }
        }
        if (bracket_map)
            free(bracket_map);
        dprintf(1, "\n> ");
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
    int status = 0;
    int stack_size = 0;
    unsigned char *stack;

    signal(SIGINT, cancel_int);
    if (ac > 1) {
        if (strcmp(av[1], "-h") == 0) {
            dprintf(1, "Usage:\n  -h:\t\tprint this page\n  <number>:\tsize of the stack (if not privided, default value is 30 000)\n");
            return 0;
        } else {
            stack_size = atoi(av[1]);
        }
    }
    if (stack_size <= 0)
        stack_size = STACK_SIZE;
    if (!(stack = init_stack(stack_size)))
        return 1;
    status = interpreter(stack, stack_size);
    if (stack)
        free(stack);
    return status;
}
