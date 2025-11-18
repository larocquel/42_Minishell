# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: davmendo <davmendo@student.42porto.com>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/28 17:39:32 by leoaguia          #+#    #+#              #
#    Updated: 2025/11/18 19:28:08 by davmendo         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#==============================================================================#
#                         NAMES & PATHS & OBJECTS                              #
#==============================================================================#

NAME = libft.a

INCLUDES_DIR = .

PRINTF_SRCS = \
./ft_printf/ft_printf.c \
./ft_printf/ft_put_char.c \
./ft_printf/ft_put_digit.c \
./ft_printf/ft_put_ptr.c \
./ft_printf/ft_put_str.c \

GNL_SRCS = ./get_next_line/get_next_line.c

SRCS = \
./utils/ft_atoi.c \
./utils/ft_bzero.c \
./utils/ft_calloc.c \
./utils/ft_isalnum.c \
./utils/ft_isalpha.c \
./utils/ft_isascii.c \
./utils/ft_isdigit.c \
./utils/ft_isprint.c \
./utils/ft_itoa.c \
./utils/ft_lstadd_back.c \
./utils/ft_lstadd_front.c \
./utils/ft_lstclear.c \
./utils/ft_lstdelone.c \
./utils/ft_lstiter.c \
./utils/ft_lstlast.c \
./utils/ft_lstmap.c \
./utils/ft_lstnew.c \
./utils/ft_lstsize.c \
./utils/ft_memchr.c \
./utils/ft_memcmp.c \
./utils/ft_memcpy.c \
./utils/ft_memmove.c \
./utils/ft_memset.c \
./utils/ft_putchar_fd.c \
./utils/ft_putendl_fd.c \
./utils/ft_putnbr_fd.c \
./utils/ft_putstr_fd.c \
./utils/ft_split.c \
./utils/ft_strchr.c \
./utils/ft_strdup.c \
./utils/ft_striteri.c \
./utils/ft_strjoin.c \
./utils/ft_strlcat.c \
./utils/ft_strlcpy.c \
./utils/ft_strlen.c \
./utils/ft_strmapi.c \
./utils/ft_strncmp.c \
./utils/ft_strcmp.c \
./utils/ft_strnstr.c \
./utils/ft_strrchr.c \
./utils/ft_strtrim.c \
./utils/ft_substr.c \
./utils/ft_tolower.c \
./utils/ft_toupper.c

PRINTF_OBJS = $(PRINTF_SRCS:.c=.o)
GNL_OBJS = $(GNL_SRCS:.c=.o)
OBJS = $(SRCS:.c=.o)

#==============================================================================#
#                               COMPILATION                                    #
#==============================================================================#
CC = cc

CFLAGS = -Wall -Werror -Wextra

#==============================================================================#
#                                  RULES                                       #
#==============================================================================#

all: $(NAME)
$(NAME): $(OBJS) $(PRINTF_OBJS) $(GNL_OBJS)
	ar rcs $(NAME) $(OBJS) $(PRINTF_OBJS) $(GNL_OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -I $(INCLUDES_DIR) -c $< -o $@

clean:
	rm -rf $(OBJS) $(PRINTF_OBJS) $(GNL_OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean
	make

.PHONY: all clean fclean re
