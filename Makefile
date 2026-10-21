CC = gcc

CFLAGS = -g -Wall -Wextra -Werror

INTERPRETER_SRC = src/interpreter.c

INTERPRETER_OBJ = $(INTERPRETER_SRC:.c=.o)

all: interpreter

interpreter: $(INTERPRETER_OBJ)
	$(CC) $(INTERPRETER_OBJ) -o bf_interpreter $(CFLAGS)

clean:
	rm -rf $(INTERPRETER_OBJ) *.a *.o

fclean: clean
	rm -rf bf_interpreter

re: fclean all

.PHONY: all interpreter clean fclean re
