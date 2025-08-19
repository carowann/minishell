FILES = echo.c
OBJ = $(FILES:.c=.o)
NAME = MiniTest
LIBFT_PATH = ../../libft/
LIBFT_LIB = $(LIBFT_PATH)libft.a
CFLAGS = -Wall -Wextra -Werror -g
CC = cc
RM = rm -f

%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $<

all: adittionallibs $(NAME)

adittionallibs:
	@make -C $(LIBFT_PATH) all

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LIBFT_LIB) -o $(NAME)
	
clean:
	make -C $(LIBFT_PATH) clean
	${RM} $(OBJ)

fclean: clean
	make -C $(LIBFT_PATH) fclean
	${RM} $(NAME)

re: fclean all

.PHONY: all clean fclean re
