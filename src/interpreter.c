#include "../include/interpreter.h"

static void cancel_int(int sig) { (void) sig; }
static void flush_stdin(void) { char c = 0; while (read(0, &c, 1) > 0 && c != '\n'); }

int parse_bracket_map(const char *buff, int buff_size, size_t *bracket_map, size_t *bracket_stack)
{
    int stack_position = 0;

    for (int i = 0; i < buff_size; i++) {
        if (buff[i] == '[') {
            bracket_stack[stack_position] = (size_t)i;
            stack_position++;
        } else if (buff[i] == ']') {
            if (stack_position == 0) {
                dprintf(2, "[Syntax]: Unbalanced brackets (unexpected ']')\n");
                return -1;
            }
            stack_position--;
            size_t open_i = bracket_stack[stack_position];
            bracket_map[i] = open_i;
            bracket_map[open_i] = (size_t)i;
        }
    }
    if (stack_position != 0) {
        dprintf(2, "[Syntax]: Unbalanced brackets (missing ']')\n");
        return -1;
    }
    return 0;
}

void visualize(unsigned char *stack, int p, int stack_size)
{
    int start = (p > 4) ? (p - 4) : 0;
    int end = (p + 4 < stack_size) ? (p + 4) : (stack_size - 1);

    dprintf(1, "--------");
    for (int i = start; i <= end; i++)
        dprintf(1, "--------");
    dprintf(1, "-\n");
    dprintf(1, "| Index ");
    for (int i = start; i <= end; i++)
        dprintf(1, "| %5d ", i);
    dprintf(1, "|\n");
    dprintf(1, "--------");
    for (int i = start; i <= end; i++)
        dprintf(1, "--------");
    dprintf(1, "-\n");
    dprintf(1, "| Value ");
    for (int i = start; i <= end; i++)
        dprintf(1, "| %5u ", (unsigned int)stack[i]);
    dprintf(1, "|\n");
    dprintf(1, "| ASCII ");
    for (int i = start; i <= end; i++) {
        unsigned char c = stack[i];
        if (c >= 32 && c <= 126)
            dprintf(1, "| %4c  ", c);
        else
            dprintf(1, "|   .   ");
    }
    dprintf(1, "|\n");
    dprintf(1, "--------");
    for (int i = start; i <= end; i++)
        dprintf(1, "--------");
    dprintf(1, "-\n");
    dprintf(1, "        ");
    for (int i = start; i <= end; i++) {
        if (i == p)
            dprintf(1, "    ^   ");
        else
            dprintf(1, "        ");
    }
    dprintf(1, "\n");
}

int check_builtin(char *buff, unsigned char *stack, int p, int stack_size)
{
    if (strcmp(buff, "exit") == 0 || strcmp(buff, "quit") == 0)
        return 1;
    if(strcmp(buff, "visualize") == 0) {
        visualize(stack, p, stack_size);
        return 2;
    }
    return 0;
}

int interpreter(unsigned char *stack, int stack_size)
{
    int p = 0;
    char buff[BUFF_SIZE];
    ssize_t bytes_read = 0;
    size_t *mem = malloc(BUFF_SIZE * 2 * sizeof(size_t));
    if (!mem) return 1;

    size_t *bracket_map = mem;
    size_t *bracket_stack = mem + BUFF_SIZE;

    dprintf(1, "BrainFuck Interpreter V1.0 Stack size: %d\n(Type 'exit' or 'quit' to leave, CTRL+D to exit, CTRL+C cancels input)\n> ", stack_size);

    while (1) {
        bytes_read = read(0, buff, BUFF_SIZE - 1);
        if (bytes_read < 0) {
            if (errno == EINTR) {
                dprintf(1, "\n> ");
                continue;
            }
            break;
        }
        if (bytes_read == 0)
            break;
        buff[bytes_read] = '\0';
        while (bytes_read > 0 && (buff[bytes_read - 1] == '\n' || buff[bytes_read - 1] == '\r')) {
            buff[--bytes_read] = '\0';
        }
        if (bytes_read == 0) {
            dprintf(1, "> ");
            continue;
        }
        int builtin_res = check_builtin(buff, stack, p, stack_size);
        if (builtin_res == 1)
            break;
        if (builtin_res == 2) {
            dprintf(1, "\n> ");
            continue;
        }
        if (parse_bracket_map(buff, bytes_read, bracket_map, bracket_stack) != 0) {
            dprintf(1, "> ");
            continue;
        }
        for (int i = 0; i < bytes_read;) {
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
                case ',': {
                    ssize_t in_read = read(0, &stack[p], 1);
                    if (in_read <= 0)
                        stack[p] = 0;
                    else
                        flush_stdin();
                    i++;
                    break;
                }
                case '[':
                    if (i + 2 < bytes_read && (buff[i + 1] == '-' || buff[i + 1] == '+') && buff[i + 2] == ']') {
                        stack[p] = 0;
                        i += 3;
                    } else if (stack[p] == 0)
                        i = (int)bracket_map[i] + 1;
                    else
                        i++;
                    break;
                case ']':
                    if (stack[p] != 0)
                        i = (int)bracket_map[i] + 1;
                    else
                        i++;
                    break;

                default:
                    i++;
                    break;
            }
        }
        dprintf(1, "\n> ");
    }
    free(mem);
    return 0;
}

unsigned char *init_stack(int size)
{
    unsigned char *stack = calloc(size, sizeof(unsigned char));

    if (!stack)
        return NULL;
    return stack;
}

int main(int ac, char **av)
{
    int status = 0;
    int stack_size = 0;
    unsigned char *stack = NULL;

    signal(SIGINT, cancel_int);
    if (ac > 1) {
        if (strcmp(av[1], "-h") == 0) {
            dprintf(1, "Usage:\n  -h:\t\tprint this page\n  <number>:\tsize of the stack (default: %d)\n", STACK_SIZE);
            return 0;
        }
        stack_size = atoi(av[1]);
    }
    if (stack_size <= 0)
        stack_size = STACK_SIZE;
    stack = init_stack(stack_size);
    if (!stack) {
        dprintf(2, "[Fatal]: Failed to allocate stack memory\n");
        return 1;
    }
    status = interpreter(stack, stack_size);
    free(stack);
    return status;
}
