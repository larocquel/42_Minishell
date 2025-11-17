# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: leoaguia <leoaguia@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/11/17 15:49:02 by leoaguia          #+#    #+#              #
#    Updated: 2025/11/17 22:11:29 by leoaguia         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= minishell

CC			= cc
CFLAGS		= -Wall -Wextra -Werror
RLFLAGS		= -lreadline

INCLUDES	= -Iinclude

SRC_DIR		= src
SRC			=			\
$(SRC_DIR)/main.c		\
$(SRC_DIR)/signals.c	\

OBJ_DIR		= obj
# Substitui src/arquivo.c por obj/arquivo.o
OBJ			= $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

all: $(NAME)

# Regra principal: compila o minishell
$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(RLFLAGS) -o $(NAME)

# Regra para compilar qualquer .c em obj/
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all
