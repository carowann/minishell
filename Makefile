# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: cwannhed <cwannhed@student.42firenze.it>   +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/08/11 12:30:56 by cwannhed          #+#    #+#              #
#    Updated: 2025/09/25 16:24:35 by cwannhed         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
TEST_NAME = parser_tester

# Colors
GREEN = \033[0;32m
BLUE = \033[0;34m
RED = \033[0;31m
YELLOW = \033[0;33m
CYAN = \033[0;36m
RESET = \033[0m

SUPPRESSION_FILE = readline.supp

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

INCLUDES = -I./includes -I./$(LIBFT_DIR)

SRC =	src/main_utils.c			\
	src/main.c				\
	src/parsing/cleanup.c			\
	src/parsing/debug.c			\
	src/parsing/tokens_to_cmds_handlers.c	\
	src/parsing/tokens_to_cmds_operators.c	\
	src/parsing/tokens_to_cmds_utils.c	\
	src/parsing/tokens_to_cmds.c		\
	src/parsing/parser.c			\
	src/parsing/parser_utils.c		\
	src/parsing/state_machine_extra.c	\
	src/parsing/state_machine_handlers.c	\
	src/parsing/state_machine.c		\
	src/parsing/token_merger.c		\
	src/parsing/token_utils.c		\
	src/parsing/var_expansion.c		\
	src/parsing/var_expansion_utils.c	\
	src/execution/echo_builtin.c			\
	src/execution/env_builtin.c			\
	src/env/environment.c			\
	src/execution/cleanup.c			\
	src/execution/exit.c			\
	src/execution/export.c			\
	src/execution/execution_builtin.c	\
	src/execution/execution_external.c	\
	src/execution/execution.c		\
	src/execution/printfd.c			\
	src/execution/pwd.c			\
	src/execution/unset.c			\
	src/execution/pipesman.c		\
	src/execution/exec_utils.c		\
	src/execution/cd_builtin.c 		\
	src/execution/heredoc.c			\
	src/execution/extra.c			\
	src/execution/pipe_utils.c		\
	src/execution/execve_utils.c	\
	src/execution/heredoc_utils.c	\
	src/execution/signals.c		

# Flags
CFLAGS = -g -Wall -Werror -Wextra
CFLAGS_DEBUG = $(CFLAGS) -DDEBUG
LIBS = -lreadline

# Source files per test (escludi main.c)
TEST_SRCS = $(filter-out src/main.c, $(SRC))

all: $(NAME)

$(LIBFT):
	@make --no-print-directory -C $(LIBFT_DIR)

$(NAME): $(LIBFT) $(SRC)
	@echo "$(BLUE)Compiling $(NAME)...$(RESET)"
	@cc $(SRC) $(INCLUDES) $(CFLAGS) $(LIBFT) $(LIBS) -o $(NAME)
	@echo "$(GREEN)$(NAME) compiled successfully!$(RESET)"
	@echo "$(CYAN)Run with: ./$(NAME)$(RESET)"

debug: $(LIBFT) $(SRC)
	@echo "$(BLUE)Compiling $(NAME) with DEBUG...$(RESET)"
	@cc $(SRC) $(INCLUDES) $(CFLAGS_DEBUG) $(LIBFT) $(LIBS) -o $(NAME)
	@echo "$(GREEN)$(NAME) compiled with debug mode!$(RESET)"

$(TEST_NAME): $(LIBFT) $(TEST_SRCS) tests/$(TEST_NAME).c
	@echo "$(BLUE)Compiling $(TEST_NAME)...$(RESET)"
	@cc tests/$(TEST_NAME).c $(TEST_SRCS) $(INCLUDES) $(CFLAGS) $(LIBFT) $(LIBS) -o $(TEST_NAME)
	@echo "$(GREEN)$(TEST_NAME) compiled successfully!$(RESET)"

test: $(TEST_NAME)
	@echo "$(CYAN)Running parser tests...$(RESET)"
	@./$(TEST_NAME)

test-verbose: $(TEST_NAME)
	@echo "$(CYAN)Running parser tests (verbose)...$(RESET)"
	@./$(TEST_NAME) -v

clean:
	@make --no-print-directory -C $(LIBFT_DIR) clean
	@echo "$(YELLOW)Cleaning $(NAME)...$(RESET)"
	@rm -f $(TEST_NAME)
	@echo "$(RED)Clean completed!$(RESET)"

fclean: clean clean_valgrind
	@make --no-print-directory -C $(LIBFT_DIR) fclean
	@echo "$(RED)Removing $(NAME)...$(RESET)"
	@rm -f $(NAME)
	@echo "$(RED)Full clean completed!$(RESET)"

re: fclean all

$(SUPPRESSION_FILE):
	@echo "# Suppress all readline/history leaks" > $(SUPPRESSION_FILE)
	@echo "{" >> $(SUPPRESSION_FILE)
	@echo "   readline_library_leaks" >> $(SUPPRESSION_FILE)
	@echo "   Memcheck:Leak" >> $(SUPPRESSION_FILE)
	@echo "   ..." >> $(SUPPRESSION_FILE)
	@echo "   obj:*/libreadline.so*" >> $(SUPPRESSION_FILE)
	@echo "}" >> $(SUPPRESSION_FILE)

valgrind: $(NAME) $(SUPPRESSION_FILE)
	@echo "$(BLUE)Running valgrind with readline suppression...$(RESET)"
	valgrind --leak-check=full --show-leak-kinds=all \
		--suppressions=$(SUPPRESSION_FILE) \
		--track-origins=yes \
		--quiet \
		./$(NAME)

clean_valgrind:
	@rm -f $(SUPPRESSION_FILE)
	@echo "$(RED)Valgrind files cleaned$(RESET)"

# Targets di utilità
help:
	@echo "$(CYAN)Available targets:$(RESET)"
	@echo "  $(GREEN)all$(RESET)          - Build minishell"
	@echo "  $(GREEN)debug$(RESET)        - Build with debug symbols and DEBUG flag"
	@echo "  $(GREEN)test$(RESET)         - Build and run parser tests"
	@echo "  $(GREEN)test-verbose$(RESET) - Run tests with debug output visible"
	@echo "  $(GREEN)valgrind$(RESET)     - Run with valgrind (leak check)"
	@echo "  $(GREEN)clean$(RESET)        - Clean object files"
	@echo "  $(GREEN)fclean$(RESET)       - Full clean (remove executables)"
	@echo "  $(GREEN)re$(RESET)           - Rebuild everything"

.PHONY: all debug test test-verbose old-test clean fclean re valgrind clean_valgrind help
