# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: leoaguia <leoaguia@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/11/02 22:51:07 by leoaguia          #+#    #+#              #
#    Updated: 2025/11/08 16:46:18 by leoaguia         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		:= minishell
CC			:= cc
CFLAGS		:= -Wall -Wextra -Werror -g
LDFLAGS		:= -lreadline

INC_DIR		:= inc
SRC_DIR		:= src
OBJ_DIR		:= obj
LIBFT_DIR	:= libft
LIBFT_A		:= $(LIBFT_DIR)/libft.a
INCS		:= -I$(INC_DIR) -I$(LIBFT_DIR)

SRCS := 	$(SRC_DIR)/main.c				\
			$(SRC_DIR)/repl.c				\
			$(SRC_DIR)/repl_helper.c		\
			$(SRC_DIR)/signals.c			\
			$(SRC_DIR)/shell_init.c			\
			$(SRC_DIR)/exec/exec.c			\
			$(SRC_DIR)/exec/path.c			\
			$(SRC_DIR)/builtins/builtins.c	\
			$(SRC_DIR)/builtins/echo.c		\
			$(SRC_DIR)/builtins/pwd.c		\
			$(SRC_DIR)/builtins/env.c		\
			$(SRC_DIR)/builtins/exit.c		\
			$(SRC_DIR)/builtins/cd.c		\
			$(SRC_DIR)/builtins/export.c	\
			$(SRC_DIR)/builtins/unset.c		\
			$(SRC_DIR)/utils/utils_str.c	\
			$(SRC_DIR)/utils/utils_env.c	\
			$(SRC_DIR)/utils/utils_num.c	\
			$(SRC_DIR)/parse/token.c		\
			$(SRC_DIR)/parse/token_utils.c	\
			$(SRC_DIR)/parse/token_read.c	\
			$(SRC_DIR)/parse/token_buf.c

OBJS := $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

all: $(NAME)

$(LIBFT_A):
	@$(MAKE) -C $(LIBFT_DIR)

$(NAME): $(LIBFT_A) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT_A) $(INCS) $(LDFLAGS) -o $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCS) -c $< -o $@

clean:
	@$(MAKE) -C $(LIBFT_DIR) clean || true
	@rm -rf $(OBJ_DIR)

fclean: clean
	@$(MAKE) -C $(LIBFT_DIR) fclean || true
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
