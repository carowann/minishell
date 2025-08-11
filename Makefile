# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: cwannhed <cwannhed@student.42firenze.it    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/08/11 12:30:56 by cwannhed          #+#    #+#              #
#    Updated: 2025/08/11 13:28:34 by cwannhed         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

GREEN = \033[0;32m
BLUE = \033[0;34m
RED = \033[0;31m
RESET = \033[0m

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

INCLUDES = -I./includes  -I./$(LIBFT_DIR)

SRC =	src/main.c		\

FLAGS = -g
FLAGS += -Wall -Werror -Wextra
LIBS = -lreadline

all: $(NAME)

$(LIBFT):
	@make --no-print-directory -C $(LIBFT_DIR)

$(NAME): $(LIBFT) $(SRC)
	@echo "$(BLUE)Compiling $(NAME)...$(RESET)"
	@cc $(SRC) $(INCLUDES) $(FLAGS) $(LIBFT) $(LIBS) -o $(NAME)
	@echo "$(GREEN)$(NAME) compiled successfully!$(RESET)"
	@echo "$(CYAN)Run with: ./$(NAME)$(RESET)"

clean:
	@make --no-print-directory -C $(LIBFT_DIR) clean
	@echo "$(YELLOW)Cleaning $(NAME)...$(RESET)"
	@echo "$(RED)Clean completed!$(RESET)"

fclean: clean
	@make  --no-print-directory -C $(LIBFT_DIR) fclean
	@echo "$(RED)Removing $(NAME)...$(RESET)"
	@rm -f $(NAME)
	@echo "$(RED)Full clean completed!$(RESET)"

re: fclean all

.PHONY: all clean fclean re
