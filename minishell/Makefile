# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: leoaguia <leoaguia@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/11/17 15:49:02 by leoaguia          #+#    #+#              #
#    Updated: 2025/11/19 17:40:24 by leoaguia         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

###############################################################
# Nome do binário final
###############################################################
NAME		= minishell

###############################################################
# Compilador e flags
#  -Wall -Wextra -Werror → flags obrigatórias da 42
###############################################################
CC			= cc
CFLAGS		= -Wall -Wextra -Werror

# Flags da Readline (lib obrigatória para o minishell)
RLFLAGS		= -lreadline

###############################################################
# Diretórios
# SRC_DIR → onde ficam os .c do minishell
# OBJ_DIR → onde serão colocados os .o
# LIBFT_DIR → onde está tua libft
###############################################################
SRC_DIR		= src
OBJ_DIR		= obj
LIBFT_DIR	= libft

###############################################################
# Caminho completo do arquivo libft.a que iremos linkar
###############################################################
LIBFT		= $(LIBFT_DIR)/libft.a

###############################################################
# Includes principais:
#  - include/ contém minishell.h
#  - libft/ contém libft.h
###############################################################
INCLUDES	= -Iinclude -I$(LIBFT_DIR)

###############################################################
# Lista de todos os .c do projeto (adiciona mais conforme criar)
###############################################################
SRC			= $(SRC_DIR)/main.c		\
			  $(SRC_DIR)/signals.c	\
			  $(SRC_DIR)/lexer.c	\
			  $(SRC_DIR)/parser.c

###############################################################
# Conversão automática:
#  src/arquivo.c → obj/arquivo.o
###############################################################
OBJ			= $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

###############################################################
# Regra principal: compilar tudo
#  - Primeiro compila a libft
#  - Depois compila o minishell
###############################################################
all: $(LIBFT) $(NAME)

###############################################################
# Construir a libft
#  -C libft → entra no diretório libft e roda o make lá dentro
###############################################################
$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

###############################################################
# Linkar o minishell
#  $(OBJ) → todos os .o do minishell
#  $(LIBFT) → linka a libft
#  $(RLFLAGS) → linka a Readline
###############################################################
$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LIBFT) $(RLFLAGS) -o $(NAME)

###############################################################
# Regra para compilar qualquer .c em obj/
#  - Cria obj/ se não existir
#  - Compila o .c como .o
###############################################################
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

###############################################################
# Limpeza básica
#  - Apaga todos os .o
#  - Roda clean dentro da libft
###############################################################
clean:
	rm -rf $(OBJ_DIR)
	$(MAKE) -C $(LIBFT_DIR) clean

###############################################################
# Limpeza total
#  - Apaga minishell
#  - Apaga tudo relacionado na libft
###############################################################
fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean

###############################################################
# Reconstrução completa
###############################################################
re: fclean all
