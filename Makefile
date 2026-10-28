CC = gcc

CFLAGS = -g -Wall -Wextra -Werror

INTERPRETER_SRC = src/interpreter.c
INTERPRETER_OBJ = $(INTERPRETER_SRC:.c=.o)

COMPILER_SRC = src/compiler.c
COMPILER_OBJ = $(COMPILER_SRC:.c=.o)

all: interpreter compiler

interpreter: $(INTERPRETER_OBJ)
	$(CC) $(INTERPRETER_OBJ) -o bf_interpreter $(CFLAGS)

compiler: $(COMPILER_OBJ)
	$(CC) $(COMPILER_OBJ) -o bf_compiler $(CFLAGS)

clean:
	rm -rf $(INTERPRETER_OBJ) $(COMPILER_OBJ) *.a *.o

fclean: clean
	rm -rf bf_interpreter bf_compiler

re: fclean all

.PHONY: all interpreter compiler clean fclean re
