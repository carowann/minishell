# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: cwannhed <cwannhed@student.42firenze.it>   +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/08/11 12:30:56 by cwannhed          #+#    #+#              #
#    Updated: 2025/08/21 17:01:01 by cwannhed         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

GREEN = \033[0;32m
BLUE = \033[0;34m
RED = \033[0;31m
RESET = \033[0m

SUPPRESSION_FILE = readline.supp

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

INCLUDES = -I./includes  -I./$(LIBFT_DIR)

SRC =	src/main.c				\
	src/parsing/cleanup.c			\
	src/parsing/debug.c			\
	src/parsing/operator_state_handler.c	\
	src/parsing/parse_commands.c		\
	src/parsing/parser.c			\
	src/parsing/parsing_utils.c		\
	src/parsing/state_machine.c		\
	src/parsing/token_utils.c		\
	src/parsing/var_state_handler.c 	\
	src/parsing/var_expansion.c		\
	src/parsing/var_expansion_utils.c


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

fclean: clean clean_valgrind
	@make  --no-print-directory -C $(LIBFT_DIR) fclean
	@echo "$(RED)Removing $(NAME)...$(RESET)"
	@rm -f $(NAME)
	@echo "$(RED)Full clean completed!$(RESET)"

re: fclean all

$(SUPPRESSION_FILE):
	@echo "$(BLUE)Creating readline suppression file...$(RESET)"
	@echo "# Suppress only readline() leaks" > $(SUPPRESSION_FILE)
	@echo "{" >> $(SUPPRESSION_FILE)
	@echo "   readline_leaks_only" >> $(SUPPRESSION_FILE)
	@echo "   Memcheck:Leak" >> $(SUPPRESSION_FILE)
	@echo "   ..." >> $(SUPPRESSION_FILE)
	@echo "   fun:readline" >> $(SUPPRESSION_FILE)
	@echo "}" >> $(SUPPRESSION_FILE)
	@echo "$(GREEN)Suppression file created: $(SUPPRESSION_FILE)$(RESET)"

valgrind: $(NAME) $(SUPPRESSION_FILE)
	@echo "$(BLUE)Running valgrind with readline suppression...$(RESET)"
	valgrind --leak-check=full --show-leak-kinds=all \
		--suppressions=$(SUPPRESSION_FILE) \
		--track-origins=yes \
		./$(NAME)

clean_valgrind:
	@rm -f $(SUPPRESSION_FILE)
	@echo "$(RED)Valgrind files cleaned$(RESET)"

.PHONY: all clean fclean re valgrind clean_valgrind
