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

typedef struct s_grb_node {
    void *ptr;
    struct s_grb_node *next;
} t_grb_node;

typedef struct	s_env
{
	char			*value;
	struct s_env	*next;
}				t_env;

int env(int fd, t_env *env);
t_env	*env_to_list(char **envp);
int env(int fd, t_env *env);
t_env	*env_to_list(char **envp);
int		pwd(int fd);
int	check_param_fd(int fd, va_list arg, char c);
int	        echo_exec(char **str, int n_var, int fd);
int         ft_printfd(int fd, const char *format, ...);
int execute_cmd(t_cmd *cmd, t_env *envar);
int	is_valid_cmd(char *cmd);
int command_select(t_cmd *cmd, int fd, t_env *en);
void	free_env(t_env *head);
#endif