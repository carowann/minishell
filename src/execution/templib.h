#ifndef TEMPLIB_H
# define TEMPLIB_H


# include <stddef.h>
# include <stdlib.h>
# include <limits.h>
# include <unistd.h>
# include <stdint.h>
# include <stdarg.h>
# include <fcntl.h>
# include "../../libft/libft.h"


int	check_param_fd(int fd, va_list arg, char c);
int	        echo_exec(char *str, int n_var, int fd);
int         ft_printfd(int fd, const char *format, ...);

#endif