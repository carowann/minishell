# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: cwannhed <cwannhed@student.42firenze.it>   +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/15 15:51:55 by cwannhed          #+#    #+#              #
#    Updated: 2025/10/16 18:33:22 by cwannhed         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a

CC = cc
CFLAGS = -Wall -Wextra -Werror -g
AR = ar rcs
RM = rm -f
HDR = libft.h
OBJ_DIR = objs

SRCS = \
		ft_isalnum.c			\
		ft_isalpha.c			\
		ft_isascii.c			\
		ft_isdigit.c			\
		ft_isprint.c			\
		ft_isupper.c			\
		ft_strlen.c			\
		ft_tolower.c			\
		ft_toupper.c			\
		ft_calloc.c			\
		ft_memset.c			\
		ft_bzero.c			\
		ft_memcpy.c			\
		ft_memmove.c			\
		ft_memchr.c			\
		ft_memcmp.c			\
		ft_strchr.c			\
		ft_strcmp.c			\
		ft_strrchr.c			\
		ft_strncmp.c			\
		ft_strnstr.c			\
		ft_strlcpy.c			\
		ft_strlcat.c			\
		ft_strdup.c			\
		ft_substr.c			\
		ft_strjoin.c			\
		ft_strjoin3.c			\
		ft_strtrim.c			\
		ft_split.c			\
		ft_atoi.c			\
		ft_itoa.c			\
		ft_strmapi.c			\
		ft_striteri.c			\
		ft_putchar_fd.c			\
		ft_putstr_fd.c			\
		ft_putendl_fd.c			\
		ft_putnbr_fd.c			\
		ft_putnbr_hex_fd.c		\
		ft_putptr_fd.c			\
		ft_putnbr_u_fd.c		\
		ft_lstnew_bonus.c		\
		ft_lstadd_front_bonus.c		\
		ft_lstsize_bonus.c		\
		ft_lstlast_bonus.c		\
		ft_lstdelone_bonus.c		\
		ft_lstadd_back_bonus.c		\
		ft_lstclear_bonus.c		\
		ft_lstiter_bonus.c		\
		ft_lstmap_bonus.c		\
		ft_printf.c			\
		ft_isspace.c			\
		ft_strndup.c			\
		get_next_line.c			\
		get_next_line_bonus.c

OBJS = $(SRCS:%.c=$(OBJ_DIR)/%.o)

all: $(NAME)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: %.c $(HDR) | $(OBJ_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	@$(AR) $(NAME) $(OBJS)

clean:
	@rm -rf $(OBJ_DIR)

fclean: clean
	@$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
