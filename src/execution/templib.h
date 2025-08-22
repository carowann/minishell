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


int update_env(t_env *envar, char *arg);
int add_env(t_env *envar, char *arg);
t_env *find_env(t_env *envar, char *arg);
int			 env(int fd, t_env *env, int print_all);
t_env		*env_to_list(char **envp);
int			pwd(int fd);
int			check_param_fd(int fd, va_list arg, char c);
int	        echo_exec(char **str, int n_var, int fd);
int         ft_printfd(int fd, const char *format, ...);
int			 execute_cmd(t_cmd *cmd, t_env *envar);
int			is_valid_cmd(char *cmd);
int			 command_select(t_cmd *cmd, int fd, t_env *en);
void		free_env(t_env *head);
int	echo(char	**args, int redirect_fd);
int	export(t_cmd *cmd, int fd, t_env *envar);
#endif