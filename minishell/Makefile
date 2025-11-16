# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: leoaguia <leoaguia@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/11/02 22:51:07 by leoaguia          #+#    #+#              #
#    Updated: 2025/11/16 19:32:58 by leoaguia         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #



NAME		:= minishell
CC			:= cc
CFLAGS		:= -Wall -Wextra -Werror -g
LDFLAGS		:= -lreadline

INC_DIR		:= src/00_inc
SRC_DIR		:= src
OBJ_DIR		:= obj
LIBFT_DIR	:= libft
LIBFT_A		:= $(LIBFT_DIR)/libft.a
INCS		:= -I$(INC_DIR) -I$(LIBFT_DIR)

SRCS := 	$(SRC_DIR)/01_main/main.c			\
			$(SRC_DIR)/01_main/ms_init.c		\
			$(SRC_DIR)/01_main/ms_loop.c		\
			$(SRC_DIR)/01_main/ms_loop_helper.c	\
			$(SRC_DIR)/02_token/token.c			\
			$(SRC_DIR)/02_token/token_utils.c	\
			$(SRC_DIR)/02_token/token_read.c	\
			$(SRC_DIR)/02_token/token_buf.c		\
			$(SRC_DIR)/03_parse/parse.c			\
			$(SRC_DIR)/03_parse/parse_utils.c	\
			$(SRC_DIR)/03_parse/parse_redir.c	\
			$(SRC_DIR)/03_parse/parse_cmd.c		\
			$(SRC_DIR)/04_exec/exec.c			\
			$(SRC_DIR)/04_exec/path.c			\
			$(SRC_DIR)/05_builtins/builtins.c	\
			$(SRC_DIR)/05_builtins/echo.c		\
			$(SRC_DIR)/05_builtins/pwd.c		\
			$(SRC_DIR)/05_builtins/env.c		\
			$(SRC_DIR)/05_builtins/exit.c		\
			$(SRC_DIR)/05_builtins/cd.c			\
			$(SRC_DIR)/05_builtins/export.c		\
			$(SRC_DIR)/05_builtins/unset.c		\
			$(SRC_DIR)/06_signals/signals.c		\
			$(SRC_DIR)/07_utils/utils_str.c		\
			$(SRC_DIR)/07_utils/utils_env.c		\
			$(SRC_DIR)/07_utils/utils_num.c


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
