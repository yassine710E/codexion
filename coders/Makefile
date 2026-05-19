NAME = codexion

CFLAGS = -g -Wall -Wextra -Werror -pthread 

SRC = main.c parsing.c data_initialization.c mutex_and_cond_init.c routines.c min_heap_operations.c operations.c routine_helpers_fun.c

OBJ = $(SRC:.c=.o)

CC = cc

all : $(NAME)

$(NAME) : $(OBJ) codexion.h
	$(CC) $(CFLAGS)  $(OBJ) -o $(NAME)

clean :
	rm -rf $(OBJ)

fclean : clean
	rm -rf $(NAME)

re : fclean all 
