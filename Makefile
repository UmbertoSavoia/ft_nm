TARGET = ft_nm
CC = gcc
CFLAGS = #-Wall -Wextra -Werror
RM = rm -rf

SRC = $(wildcard ./src/*.c)
OBJ = $(SRC:.c=.o)

%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(TARGET) ./test/test1.out ./test/test2.out

_test:
	$(CC) ./test/test1.c -o ./test/test1.out
	$(CC) ./test/test2.c -o ./test/test2.out

re: fclean all

.PHONY: all clean fclean re

