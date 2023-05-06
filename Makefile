CC = cc
CFLAGS = -Wall -Wextra -Werror -D BUFFER_SIZE=42 -g3
SRC = test.c get_next_line.c get_next_line_utils.c
OBJ = $(SRC:.c=.o)
EXEC = test_gnl

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(EXEC)

re: fclean all

test: re
	 ./test_gnl

.PHONY: all clean fclean re
