NAME = codexion

CFLAGS = -Wall -Wextra -Werror -pthread

SRC = main.c parsing.c set_data.c free_instructions.c min_heap_operations.c

OBJ = $(SRC:.c=.o)

CC = cc

all : $(NAME)

$(NAME) : $(OBJ) 
	$(CC) $(CFLAGS)  $(OBJ) -o $(NAME)

clean :
	rm -rf $(OBJ)

fclean : clean
	rm -rf $(NAME)

re : fclean all 
