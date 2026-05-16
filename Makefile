NAME = codexion

CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread

SRC_DIR = src
SRC = $(SRC_DIR)/coder_routine.c \
      $(SRC_DIR)/codexion.c \
      $(SRC_DIR)/errors.c \
      $(SRC_DIR)/heap.c \
      $(SRC_DIR)/init.c \
      $(SRC_DIR)/memory.c \
      $(SRC_DIR)/monitor_routine.c \
      $(SRC_DIR)/parser.c \
      $(SRC_DIR)/routine_helpers.c \
      $(SRC_DIR)/utils.c

OBJ = $(SRC:.c=.o)
HEADER = header/codexion.h

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

%.o: %.c $(HEADER)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re