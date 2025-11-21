# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: leoaguia <leoaguia@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/11/17 15:49:02 by leoaguia          #+#    #+#              #
#    Updated: 2025/11/21 14:30:54 by leoaguia         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#									NOME										#

NAME		= minishell


#								COMPILATION										#

CC			= cc
CFLAGS		= -Wall -Wextra -Werror
RLFLAGS		= -lreadline


#								DIRECTORIES										#

SRC_DIR		= src
OBJ_DIR		= obj
LIBFT_DIR	= libft


#							LIBFT = libft.a path								#

LIBFT		= $(LIBFT_DIR)/libft.a


#		Includes: - include/ for minishell.h and - libft/ contém libft.h		#

INCLUDES	= -Iinclude -I$(LIBFT_DIR)


#								ALL .c FILES									#

SRC			= $(SRC_DIR)/main.c		\
			  $(SRC_DIR)/signals.c	\
			  $(SRC_DIR)/lexer.c	\
			  $(SRC_DIR)/parser.c


#					CONVERSION: src/arquivo.c → obj/arquivo.o					#

OBJ			= $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)


#					MAIN RULE (FIRST: LIBFT SCND: MINISHELL)					#

all: $(LIBFT) $(NAME)


#								BUILD LIBFT										#

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)


#									LINK										#
#							$(OBJ) → ALL .o FROM MINISHELL						#
#							$(LIBFT) → LINK LIBFT								#
#							$(RLFLAGS) → LINK READLINE							#

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LIBFT) $(RLFLAGS) -o $(NAME)


#							RULE TO BUILD ANY .c IN obj/						#
#							- CREATES obj/ IF DOES NOT EXIST					#
#							- COMPILE THE .c AS .o								#

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@


#								BASIC CLEANING									#
#								- DELETE ALL .o FILES							#
#								- RUNS clean INSIDE LIBFT						#

clean:
	rm -rf $(OBJ_DIR)
	$(MAKE) -C $(LIBFT_DIR) clean


#								TOTAL CLEANING									#
#								- DELETE MINISHELL								#
#								- DELETE ANYTHING RELATED TO LIBFT				#

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean


#								FULL REBUILD									#
re: fclean all
