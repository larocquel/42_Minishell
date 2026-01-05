# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lla-rocq <lla-rocq@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/11/17 15:49:02 by leoaguia          #+#    #+#              #
#    Updated: 2026/01/05 23:01:07 by lla-rocq         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#									NAME										#

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

SRC			= $(SRC_DIR)/01_main/main.c					\
			  $(SRC_DIR)/01_main/cleanup.c				\
			  $(SRC_DIR)/02_parsing/lexer.c				\
			  $(SRC_DIR)/02_parsing/lexer_utils.c		\
			  $(SRC_DIR)/02_parsing/parser.c			\
			  $(SRC_DIR)/02_parsing/parser_utils.c		\
			  $(SRC_DIR)/02_parsing/expand.c			\
			  $(SRC_DIR)/02_parsing/expand_utils.c		\
			  $(SRC_DIR)/03_exec/exec.c					\
			  $(SRC_DIR)/03_exec/exec_simple.c			\
			  $(SRC_DIR)/03_exec/exec_utils.c			\
			  $(SRC_DIR)/04_builtins/builtins.c			\
			  $(SRC_DIR)/04_builtins/builtins_ops.c		\
			  $(SRC_DIR)/04_builtins/builtins_utils.c	\
			  $(SRC_DIR)/05_redirect/redirect.c			\
			  $(SRC_DIR)/05_redirect/heredoc.c			\
			  $(SRC_DIR)/06_env/env.c					\
			  $(SRC_DIR)/06_env/env_utils.c				\
			  $(SRC_DIR)/07_signals/signals.c

#					CONVERSION: src/arquivo.c → obj/arquivo.o					#

OBJ			= $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)


#					MAIN RULE (FIRST: LIBFT SCND: MINISHELL)					#

all: $(LIBFT) $(NAME)

#						Supression of readline leaks rule						#
sup:
	echo "{\n\
   leak readline\n\
   Memcheck:Leak\n\
   ...\n\
   fun:readline\n\
}\n\
{\n\
   leak add_history\n\
   Memcheck:Leak\n\
   ...\n\
   fun:add_history\n\
}" > readline.supp


#								BUILD LIBFT										#
$(LIBFT): sup
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
	mkdir -p $(dir $@)
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
	rm -f $(NAME) readline.supp
	$(MAKE) -C $(LIBFT_DIR) fclean


#								FULL REBUILD									#
re: fclean all
