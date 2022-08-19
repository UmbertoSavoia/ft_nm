TARGET = ft_nm
CC = gcc
CFLAGS = -g#-Wall -Wextra -Werror
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
	$(RM) $(TARGET) ./test/test1.out ./test/test2.out ./test/test1_32.out ./test/test2_32.out

_test:
	$(CC) ./test/test1.c -o ./test/test1.out
	$(CC) ./test/test2.c -o ./test/test2.out
	$(CC) ./test/test1.c -m32 -o ./test/test1_32.out
	$(CC) ./test/test2.c -m32 -o ./test/test2_32.out

re: fclean all

.PHONY: all clean fclean re

