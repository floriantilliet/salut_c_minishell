CC = cc
CFLAGS = -Wall -Wextra -Werror -g3
SRC = $(wildcard *.c) $(wildcard lexing/*.c) $(wildcard builtins/*.c) $(wildcard execute/*.c)
OBJ = $(SRC:.c=.o)
EXECUTABLE = minishell
LIBFT = libft/libft.a
all: $(EXECUTABLE)

$(LIBFT) :
	make -C libft bonus

$(EXECUTABLE): $(OBJ) $(LIBFT)
	$(CC) $(CFLAGS) -o $@ $^ -lreadline -Llibft -lft

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	make -C libft clean
	rm -f $(OBJ)

fclean: clean
	make -C libft fclean
	rm -f $(EXECUTABLE)

re: fclean all

.PHONY: all clean fclean re
