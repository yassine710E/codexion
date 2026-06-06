# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ychabane <ychabane@student.1337.ma>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/05/21 16:16:27 by ychabane          #+#    #+#              #
#    Updated: 2026/05/22 04:08:16 by ychabane         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = codexion

CFLAGS = -Wall -Wextra -Werror -pthread 

SRC = main.c parsing.c data_initialization_1.c mutex_and_cond_init.c routines_1.c routines_2.c routines_3.c min_heap_operations.c operations.c routine_helpers_fun_1.c routine_helpers_fun_2.c data_initialization_2.c main_utils.c ft_atoi.c

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
